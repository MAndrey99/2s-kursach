#include "header.h"
#include "Player.h"

#define MIN_STIC_MOVEMENT_TO_MOVE 15.0f

Player::Player(Vector2f position, Color color, int joysticID, Vector2f direction): sprite(HERO_TEXTURE),
                                                                                   circle(HERO_CIRCLE_TEXTURE),
                                                                                   controller(this, joysticID),
                                                                                   muvement() {
    sprite.setTextureRect(IntRect(17, 50, 200, 150));
    sprite.setPosition(position);
    sprite.setScale(0.5 * SIZE_X_SCALE, 0.5 * SIZE_Y_SCALE);
    sprite.setColor(color);
    sprite.setOrigin(57, 74);

    health_scale_line.setPosition(sprite.getGlobalBounds().left + 10 * SIZE_X_SCALE,
                                    sprite.getGlobalBounds().top + 110 * SIZE_Y_SCALE);
    health_scale_line.setSize(Vector2f(HEALTH_LINE_X_LENGTH * SIZE_X_SCALE, HEALTH_LINE_Y_LENGTH * SIZE_Y_SCALE));
    health_scale_line.setFillColor(Color::Red);

    set_direction(direction);

    // кружок используется для обработки столкновений со стенами
    circle.setTextureRect(sprite.getTextureRect());
    circle.setRotation(sprite.getRotation());
    circle.setPosition(position);
    circle.setScale(sprite.getScale());
    circle.setOrigin(sprite.getOrigin());
}


bool Player::update(list<Sprite> &walls, list<Bullet> &bullets, list<Event> &events) {
    for (auto i = bullets.begin(); i != bullets.end(); i++) { // проверяем пересечения с пулями
        if (sprite.getGlobalBounds().intersects(i->sprite.getGlobalBounds())
                and Collision::PixelPerfectTest(sprite, i->sprite)) {
            if (helth > 0) {
                helth -= i->damage;
                if (helth < 0) helth = 0;
            }
            bullets.erase(i); // удаляем пулю
            i--;
        }
    }

    if (helth == 0) return false; // не обрабатываем движения мёртвого игрока
    controller.update(events, bullets);

    Vector2f move = muvement.multiplyed(HERO_SPEED);
    muvement.length = 0;

    // двигаем персонажа
    if (try_move(move.x, 0, walls)) {
        try_move(0, move.y, walls);
    } else {
        try_move(0, move.y, walls);
        try_move(move.x, 0, walls);
    }

    set_direction(direction);

    return true;
}


void Player::auto_drow() {
    window.draw(sprite);
//    window.draw(circle);

    health_scale_line.setPosition(sprite.getGlobalBounds().left + 10 * SIZE_X_SCALE,
                                  sprite.getGlobalBounds().top + 110 * SIZE_Y_SCALE);
    health_scale_line.setSize(Vector2f(HEALTH_LINE_X_LENGTH * SIZE_X_SCALE * helth / 100, HEALTH_LINE_Y_LENGTH * SIZE_Y_SCALE));
    window.draw(health_scale_line);
}


void Player::shoot(list<Bullet> &bullets) {
    Vector2f t(-direction.y / direction.x, 1);
    if (direction.x * t.y - direction.y * t.x < 0)
        t = Vector2f(-t.x, -t.y);
    t = Muvement(t).get_direction(); // t и owner->direction - ортонормированный базис. t поможет сдвинуть пулю к дулу

    bullets.emplace_back(Bullet(Vector2f(get_position().x + t.x*15*SIZE_X_SCALE, get_position().y + t.y*15*SIZE_Y_SCALE), Muvement(direction), 30));
    bullets.back().sprite.setRotation(sprite.getRotation());
    bullets.back().sprite.move(direction.x * 85 * SIZE_X_SCALE, direction.y * 85 * SIZE_Y_SCALE);
}


void Player::to_position(Vector2f position, Vector2f direction) {
    helth = 100.0f;
    sprite.setPosition(position);
    circle.setPosition(sprite.getPosition());
    set_direction(direction);
    controller.clock.restart();
}


void Player::set_direction(Vector2f direction) {
    float new_r = asin(direction.y); // угол в радианах, соответствующий вектору направления
    if (direction.x < 0) new_r = M_PI - new_r;
    sprite.setRotation(new_r / M_PI * 180);
    circle.setRotation(sprite.getRotation());
    this->direction = direction;
}


bool Player::try_move(float x, float y, list<Sprite> &walls) {
    auto p = sprite.getPosition(); // сохраняем то что есть сейчас, чтобы откатиться если движение невозможно
    circle.move(Vector2f(x, y));

    for (Sprite& i : walls) { // проверяем нет ли пересичений со стенами
        if (circle.getGlobalBounds().intersects(i.getGlobalBounds()) and Collision::PixelPerfectTest(circle, i)) {
            circle.setPosition(p);
            return false;
        }
    }

    sprite.setPosition(circle.getPosition());
    return true;
}


Player::Controller::Controller(Player *owner, int joysticID): owner(owner), joysticID(joysticID) {}


static void II_update(Player &player, list<Bullet> &bullets) {


}


void Player::Controller::update(list<Event> &events, list<Bullet> &bullets) {
    if (joysticID == -1) {
        II_update(*owner, bullets);
        return;
    }

    // получаем вектор движения
    Vector2f movement_vector(Joystick::getAxisPosition(joysticID, Joystick::Axis::X), Joystick::getAxisPosition(joysticID, Joystick::Axis::Y));
    if (movement_vector.x*movement_vector.x + movement_vector.y*movement_vector.y
            < MIN_STIC_MOVEMENT_TO_MOVE * MIN_STIC_MOVEMENT_TO_MOVE)
        movement_vector = Vector2f(0, 0);

    // двигаем персоонажа
    owner->muvement.add(Vector2f(movement_vector.x * clock.getElapsedTime().asSeconds(), movement_vector.y * clock.getElapsedTime().asSeconds()));

    // получаем вектор направления
    if (squ(Joystick::getAxisPosition(joysticID, Joystick::Axis::U))
        + squ(Joystick::getAxisPosition(joysticID, Joystick::Axis::R)) > squ(MIN_STIC_MOVEMENT_TO_MOVE)) {
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
