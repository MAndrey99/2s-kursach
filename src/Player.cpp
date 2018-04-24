#include "header.h"
#include "Player.h"

#define MIN_STIC_MOVEMENT_TO_MOVE 15.0f


Player::Player(Vector2f position): controller(this), muvement() {
    texture.loadFromFile("res/new_hero.png");
    sprite.setTexture(texture);

    sprite.setTextureRect(IntRect(17, 50, 200, 150));
    sprite.setPosition(position);
    sprite.setScale(0.5, 0.5);
    sprite.setColor(Color::Yellow);
    sprite.setOrigin(55, 72);

    // кружок используется для обработки столкновений со стенами
    circleShape.setRadius(36);
    circleShape.setOrigin(18, 18);
    circleShape.setPosition(sprite.getPosition().x - 15, sprite.getPosition().y - 15);
}


void Player::update(list<Event> &events, list<Sprite> &walls) {
    // сохраняем то что есть сейчас, чтобы откатиться если движение невозможно
    auto p = sprite.getPosition();

    controller.update(events);
    sprite.move(Vector2f(Vector2f(muvement).x * speed, Vector2f(muvement).y * speed));
    circleShape.setPosition(sprite.getPosition().x - 15, sprite.getPosition().y - 15);
    muvement.length = 0; // больше двигаться не надо тк мы передвинулись.

    float new_r = asin(direction.y); // угол, соответствующий вектору направления
    if (direction.x < 0) new_r = M_PI - new_r;
    sprite.setRotation(new_r / M_PI * 180);

    for (Sprite& i : walls) { // проверяем нет ли пересичений со стенами
        if (i.getGlobalBounds().intersects(circleShape.getGlobalBounds())) {
            sprite.setPosition(p);
            circleShape.setPosition(sprite.getPosition().x - 15, sprite.getPosition().y - 15);
            return;
        }
    }
}


void Player::auto_drow() {
//    window.draw(circleShape);
    window.draw(sprite);
}


Player::Controller::Controller(Player *owner): owner(owner) {}


void Player::Controller::update(list<Event> &events) {
    static Vector2f last_movement_vector(0, 0);
    Vector2f movement_vector(0.0f, 0.0f);
    pair update_last(false, false); // чтобы не забывать последний вектор движения если не двигались стики(по x и y)

    for (auto &i : events) {
        if (i.type == Event::JoystickMoved) {
            switch (i.joystickMove.axis) {
                case Joystick::Axis::X:
                    movement_vector.x = i.joystickMove.position;
                    update_last.first = true;
                    break;

                case Joystick::Axis::Y:
                    movement_vector.y = i.joystickMove.position;
                    update_last.second = true;
                    break;
            }
        }
    }

    // получаем вектор направления
    if (abs(Joystick::getAxisPosition(joysticID, Joystick::Axis::U)) + abs(Joystick::getAxisPosition(joysticID, Joystick::Axis::V)) > MIN_STIC_MOVEMENT_TO_MOVE * 2) {
        owner->direction.x = Joystick::getAxisPosition(joysticID, Joystick::Axis::U);
        owner->direction.y = Joystick::getAxisPosition(joysticID, Joystick::Axis::V);

        // нормализуем вектор направления
        float len = sqrt(owner->direction.x * owner->direction.x + owner->direction.y * owner->direction.y);
        owner->direction.x /= len;
        owner->direction.y /= len;
    }

    if (abs(movement_vector.x) >= MIN_STIC_MOVEMENT_TO_MOVE)
        owner->muvement.add(Vector2f(movement_vector.x * clock.getElapsedTime().asSeconds(), 0));
    else if (movement_vector.x == 0 and abs(last_movement_vector.x) >= MIN_STIC_MOVEMENT_TO_MOVE)
        owner->muvement.add(Vector2f((movement_vector.x = last_movement_vector.x) * clock.getElapsedTime().asSeconds(), 0));
    else
        movement_vector.x = 0;


    if (abs(movement_vector.y) >= MIN_STIC_MOVEMENT_TO_MOVE)
        owner->muvement.add(Vector2f(0, movement_vector.y * clock.getElapsedTime().asSeconds()));
    else if (movement_vector.y == 0 and abs(last_movement_vector.y) >= MIN_STIC_MOVEMENT_TO_MOVE)
        owner->muvement.add(Vector2f(0, (movement_vector.y = last_movement_vector.y) * clock.getElapsedTime().asSeconds()));
    else
        movement_vector.y = 0;

    if (update_last.first) last_movement_vector.x = movement_vector.x;
    if (update_last.second) last_movement_vector.y = movement_vector.y;
    clock.restart();
}
