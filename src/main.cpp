#include "header.h"
#include "Player.h"
#include "Field.h"

RenderWindow window(VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), "Game", Style::Close);
Field field;


int main()
{
    Event event{};
    list<Event> controller_events;

    window.setFramerateLimit(30);


    while (window.isOpen()) {

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();

            if (event.type == Event::JoystickMoved) {
                controller_events.emplace_back(event);
            }
        }

        field.player.update(controller_events, field.walls);
        controller_events.clear();

        field.drow_scene();
        window.display();
        window.clear(Color::Cyan);
    }


    return 0;
}