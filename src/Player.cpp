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
    sprite.setOrigin(57, 72);

    // кружок используется для обработки столкновений со стенами
    circleShape.setRadius(36);
    circleShape.setOrigin(18, 18);
    circleShape.setPosition(sprite.getPosition().x - 15, sprite.getPosition().y - 15);
}


void Player::update(list<Sprite> &walls, vector<Bullet> &bullets, list<Event> &events) {
    for (int i = 0; i < bullets.size(); i++) {
        if (intersects(bullets[i].sprite.getGlobalBounds())) {
            helth -= bullets[i].damage;
            bullets.erase(bullets.begin() + i); // удаляем пулю
        }
    }

    if (helth <= 0) return; // не обрабатываем движения мёртвого игрока

    // сохраняем то что есть сейчас, чтобы откатиться если движение невозможно
    auto p = sprite.getPosition();

    controller.update(events, bullets);
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
    window.draw(sprite);
}


bool Player::intersects(const Rect<float> &rectangle) {
    return circleShape.getGlobalBounds().intersects(rectangle);
}


void Player::shoot(Bullet &bullet) {
    bullet.sprite.setRotation(sprite.getRotation());
    bullet.sprite.move(direction.x * 75, direction.y * 75);
}


Player::Controller::Controller(Player *owner): owner(owner) {}


void Player::Controller::update(list<Event> &events, vector<Bullet> &bullets) {
    // получаем вектор движения
    Vector2f movement_vector(Joystick::getAxisPosition(joysticID, Joystick::Axis::X), Joystick::getAxisPosition(joysticID, Joystick::Axis::Y));
    if (movement_vector.x * movement_vector.x + movement_vector.y * movement_vector.y < MIN_STIC_MOVEMENT_TO_MOVE * MIN_STIC_MOVEMENT_TO_MOVE)
        movement_vector = Vector2f(0, 0);

    // двигаем персоонажа
    owner->muvement.add(Vector2f(movement_vector.x * clock.getElapsedTime().asSeconds(), movement_vector.y * clock.getElapsedTime().asSeconds()));

    // получаем вектор направления
    if (abs(Joystick::getAxisPosition(joysticID, Joystick::Axis::U)) + abs(Joystick::getAxisPosition(joysticID, Joystick::Axis::V)) > MIN_STIC_MOVEMENT_TO_MOVE * 2) {
        owner->direction.x = Joystick::getAxisPosition(joysticID, Joystick::Axis::U);
        owner->direction.y = Joystick::getAxisPosition(joysticID, Joystick::Axis::V);

        // нормализуем вектор направления
        float len = sqrt(owner->direction.x * owner->direction.x + owner->direction.y * owner->direction.y);
        owner->direction.x /= len;
        owner->direction.y /= len;
    }

    clock.restart();


    // далее обработка клавишь

    for (Event& it : events) {
        if (it.joystickButton.button == 5) {
            Bullet new_bullet(owner->get_position(), Muvement(owner->direction), 10);
            owner->shoot(new_bullet);
            bullets.emplace_back(new_bullet);
        }
    }
}
