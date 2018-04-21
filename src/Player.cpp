
#include <iostream>
#include "Player.h"

#define MIN_STIC_MOVEMENT_TO_MOVE 15.0f


Player::Player(sf::Vector2f position): controller(this), muvement() {
    texture.loadFromFile("res/hero_body.jpg");
    sprite.setTexture(texture);
    sprite.setPosition(position);
}


void Player::update(std::list<sf::Event> &events) {
    controller.update(events);
    sprite.move(sf::Vector2f(sf::Vector2f(muvement).x * speed, sf::Vector2f(muvement).y * speed));
    muvement.length = 0; // больше двигаться не надо тк мы передвинулись.
}


void Player::auto_drow(sf::RenderWindow &window) {
    window.draw(sprite);
    std::cout << sprite.getPosition().x << ", " << sprite.getPosition().y << std::endl;
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
