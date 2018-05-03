#include "header.h"
#include "Player.h"

#define MIN_STIC_MOVEMENT_TO_MOVE 15.0f

Player::Player(Vector2f position, Color color, nam joysticID, Vector2f direction): sprite(HERO_TEXTURE),
                                                                                   controller(this, joysticID),
                                                                                   muvement() {
    sprite.setTextureRect(IntRect(17, 50, 200, 150));
    sprite.setPosition(position);
    sprite.setScale(0.5, 0.5);
    sprite.setColor(color);
    sprite.setOrigin(57, 72);

    health_scale_line.setPosition(sprite.getGlobalBounds().left + 10,
                                    sprite.getGlobalBounds().top + 110);
    health_scale_line.setSize(Vector2f(HEALTH_LINE_X_LENGTH, HEALTH_LINE_Y_LENGTH));
    health_scale_line.setFillColor(Color::Red);

    // кружок используется для обработки столкновений со стенами
    circleShape.setRadius(30);
    circleShape.setOrigin(15, 15);
    circleShape.setPosition(sprite.getPosition().x - 15, sprite.getPosition().y - 15);

    set_direction(direction);
}


bool Player::update(list<Sprite> &walls, vector<Bullet> &bullets, list<Event> &events) {
    for (int i = 0; i < bullets.size(); i++) {
        if (circleShape.getGlobalBounds().intersects(bullets[i].sprite.getGlobalBounds())) {
            if (helth > 0)
                helth -= bullets[i].damage;
            bullets.erase(bullets.begin() + i); // удаляем пулю
        }
    }

    if (helth <= 0) return false; // не обрабатываем движения мёртвого игрока

    // сохраняем то что есть сейчас, чтобы откатиться если движение невозможно
    auto p = sprite.getPosition();

    controller.update(events, bullets);
    sprite.move(Vector2f(Vector2f(muvement).x * speed, Vector2f(muvement).y * speed));
    circleShape.setPosition(sprite.getPosition().x - 15, sprite.getPosition().y - 15);
    muvement.length = 0; // больше двигаться не надо тк мы передвинулись.

    set_direction(direction);

    for (Sprite& i : walls) { // проверяем нет ли пересичений со стенами
        if (i.getGlobalBounds().intersects(circleShape.getGlobalBounds())) {
            sprite.setPosition(p);
            circleShape.setPosition(sprite.getPosition().x - 15, sprite.getPosition().y - 15);
            return true;
        }
    }

    return true;
}


void Player::auto_drow() {
//    window.draw(circleShape);
    window.draw(sprite);

    health_scale_line.setPosition(sprite.getGlobalBounds().left + 10,
                                  sprite.getGlobalBounds().top + 110);
    health_scale_line.setSize(Vector2f(HEALTH_LINE_X_LENGTH * helth / 100, HEALTH_LINE_Y_LENGTH));

    window.draw(health_scale_line);
}


bool Player::intersects(const FloatRect &rectangle) {
    return circleShape.getGlobalBounds().intersects(rectangle);
}


void Player::shoot(vector<Bullet> &bullets) {
    Vector2f t(-direction.y / direction.x, 1);
    if (direction.x * t.y - direction.y * t.x < 0)
        t = Vector2f(-t.x, -t.y);
    t = Muvement(t).get_direction(); // t и owner->direction - ортонормированный базис. t поможет сдвинуть пулю к дулу

    bullets.emplace_back(Bullet(Vector2f(get_position().x + t.x*15, get_position().y + t.y*15), Muvement(direction), 10));
    bullets.back().sprite.setRotation(sprite.getRotation());
    bullets.back().sprite.move(direction.x * 75, direction.y * 75);
}


void Player::to_position(Vector2f position, Vector2f direction) {
    helth = 100.0f;
    sprite.setPosition(position);
    circleShape.setPosition(sprite.getPosition().x - 15, sprite.getPosition().y - 15);
    set_direction(direction);
    controller.clock.restart();
}


void Player::set_direction(Vector2f direction) {
    float new_r = asin(direction.y); // угол в радианах, соответствующий вектору направления
    if (direction.x < 0) new_r = M_PI - new_r;
    sprite.setRotation(new_r / M_PI * 180);
    this->direction = direction;
}


Player::Controller::Controller(Player *owner, nam joysticID): owner(owner), joysticID(joysticID) {}


void Player::Controller::update(list<Event> &events, vector<Bullet> &bullets) {
    // получаем вектор движения
    Vector2f movement_vector(Joystick::getAxisPosition(joysticID, Joystick::Axis::X), Joystick::getAxisPosition(joysticID, Joystick::Axis::Y));
    if (movement_vector.x * movement_vector.x + movement_vector.y * movement_vector.y < MIN_STIC_MOVEMENT_TO_MOVE * MIN_STIC_MOVEMENT_TO_MOVE)
        movement_vector = Vector2f(0, 0);

    // двигаем персоонажа
    owner->muvement.add(Vector2f(movement_vector.x * clock.getElapsedTime().asSeconds(), movement_vector.y * clock.getElapsedTime().asSeconds()));

    // получаем вектор направления
    if (squ(Joystick::getAxisPosition(joysticID, Joystick::Axis::U)) + squ(Joystick::getAxisPosition(joysticID, Joystick::Axis::R)) > squ(MIN_STIC_MOVEMENT_TO_MOVE)) {
        owner->direction.x = Joystick::getAxisPosition(joysticID, Joystick::Axis::U);
        owner->direction.y = Joystick::getAxisPosition(joysticID, Joystick::Axis::R);

        // нормализуем вектор направления
        float len = sqrt(owner->direction.x * owner->direction.x + owner->direction.y * owner->direction.y);
        owner->direction.x /= len;
        owner->direction.y /= len;
    }

    clock.restart();


    Event::JoystickButtonEvent event;
    // далее обработка клавишь
    for (Event& it : events) {
        event = it.joystickButton;
        if (event.joystickId != joysticID) continue;

        switch (event.button) {
            case 5:
                owner->shoot(bullets);
                break;
        }
    }
}
