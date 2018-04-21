#pragma once


#include <list>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Muvement.h"

class Player {
    class Controller {
        Player* owner;
        sf::Clock clock;

    public:
        Controller(Player*);

        void update(std::list<sf::Event>&);
    } controller;

    float speed = 1.0f;
    int helth = 100;
    sf::Texture texture;
    sf::Sprite sprite;

public:
    Muvement muvement; // все передвижение сохраняется сдесь. Спрайт двигается после каждого апдейта.

    Player(sf::Vector2f position);

    void update(std::list<sf::Event>&);
    void auto_drow(sf::RenderWindow&);
};

