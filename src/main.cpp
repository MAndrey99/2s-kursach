#include <SFML/Graphics.hpp>
#include <iostream>
#include <unistd.h>
#include "Player.h"

#define WINDOW_SIZE_X 1000
#define WINDOW_SIZE_Y 500

using namespace std;




sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), "Game", sf::Style::Close);
Player player(sf::Vector2f(100, 100));


void drow_scene() {
    player.auto_drow(window);
}


int main()
{
    sf::Event event{};
    list<sf::Event> controller_events;

    window.setFramerateLimit(30);


    while (window.isOpen()) {

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (event.type == sf::Event::JoystickMoved and event.joystickMove.position) {
                controller_events.emplace_back(event);
            }
        }

        player.update(controller_events);
        controller_events.clear();

        drow_scene();
        window.display();
        window.clear(sf::Color::Cyan);
    }


    return 0;
}