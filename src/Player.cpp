
#include <iostream>
#include <cmath>
#include "Player.h"

#define MIN_STIC_MOVEMENT_TO_MOVE 15.0f


Player::Player(sf::Vector2f position): controller(this), muvement() {
    texture.loadFromFile("res/new_hero.png");
    sprite.setTexture(texture, true);
    sprite.setPosition(position);
    sprite.setScale(0.5, 0.5);
    sprite.setColor(sf::Color::Yellow);
    sprite.setOrigin(100, 124);
}


void Player::update(std::list<sf::Event> &events) {
    controller.update(events);
    sprite.move(sf::Vector2f(sf::Vector2f(muvement).x * speed, sf::Vector2f(muvement).y * speed));
    muvement.length = 0; // больше двигаться не надо тк мы передвинулись.

    float r = std::asin(direction.y); // угол, соответствующий вектору направления
    if (direction.x < 0) r = M_PI - r;
    sprite.setRotation(r / M_PI * 180);

    std::cout << r / M_PI * 180 << '\n';
}


void Player::auto_drow(sf::RenderWindow &window) {
    window.draw(sprite);
}


Player::Controller::Controller(Player *owner): owner(owner) {}


void Player::Controller::update(std::list<sf::Event> &events) {
    static sf::Vector2f last_movement_vector(0, 0);
    sf::Vector2f movement_vector(0.0f, 0.0f);
    std::pair update_last(false, false); // чтобы не забывать последний вектор движения если не двигались стики(по x и y)

    for (auto &i : events) {
        if (i.type == sf::Event::JoystickMoved) {
            switch (i.joystickMove.axis) {
                case sf::Joystick::Axis::X:
                    movement_vector.x = i.joystickMove.position;
                    update_last.first = true;
                    break;

                case sf::Joystick::Axis::Y:
                    movement_vector.y = i.joystickMove.position;
                    update_last.second = true;
                    break;
            }
        }
    }

    // получаем вектор направления
    if (abs(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::U)) + abs(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::V)) > MIN_STIC_MOVEMENT_TO_MOVE * 2) {
        owner->direction.x = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::U);
        owner->direction.y = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::V);

        // нормализуем вектор направления
        float len = std::sqrt(owner->direction.x * owner->direction.x + owner->direction.y * owner->direction.y);
        owner->direction.x /= len;
        owner->direction.y /= len;
    }

    if (abs(movement_vector.x) >= MIN_STIC_MOVEMENT_TO_MOVE)
        owner->muvement.add(sf::Vector2f(movement_vector.x * clock.getElapsedTime().asSeconds(), 0));
    else if (movement_vector.x == 0 and abs(last_movement_vector.x) >= MIN_STIC_MOVEMENT_TO_MOVE)
        owner->muvement.add(sf::Vector2f((movement_vector.x = last_movement_vector.x) * clock.getElapsedTime().asSeconds(), 0));
    else
        movement_vector.x = 0;


    if (abs(movement_vector.y) >= MIN_STIC_MOVEMENT_TO_MOVE)
        owner->muvement.add(sf::Vector2f(0, movement_vector.y * clock.getElapsedTime().asSeconds()));
    else if (movement_vector.y == 0 and abs(last_movement_vector.y) >= MIN_STIC_MOVEMENT_TO_MOVE)
        owner->muvement.add(sf::Vector2f(0, (movement_vector.y = last_movement_vector.y) * clock.getElapsedTime().asSeconds()));
    else
        movement_vector.y = 0;

    if (update_last.first) last_movement_vector.x = movement_vector.x;
    if (update_last.second) last_movement_vector.y = movement_vector.y;
    clock.restart();
}
