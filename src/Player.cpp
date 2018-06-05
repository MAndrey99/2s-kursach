#include "Player.h"
#include "II.h"

#define rand_sign() (rand() % 2 == 0 ? 1.f : -1.f)


Player::Player(Vector2f position, Color color, int joysticID, Vector2f direction): sprite(HERO_TEXTURE),
                                                                                   circle(HERO_CIRCLE_TEXTURE),
                                                                                   controller(this, joysticID),
                                                                                   muvement() {
    reload_sound.setBuffer(RELOAD_SOUND);
    bullets_count.setFillColor(Color(100, 50, 40));
    bullets_count.setFont(FD_FONT);
    bullets_count.setCharacterSize(17.f * zoom * GAME_SCALE);

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


bool Player::update(list<Sprite> &walls, list<Bullet> &bullets, list<Event> &events, Player &other) {
    for (auto i = bullets.begin(); i != bullets.end();) { // проверяем пересечения с пулями
        if (sprite.getGlobalBounds().intersects(i->sprite.getGlobalBounds())
                and Collision::PixelPerfectTest(sprite, i->sprite)) {
            if (helth > 0) {
                helth -= i->damage;
                if (helth < 0) helth = 0;
                (controller.joysticID == 0 ? player1_II_data : player2_II_data).last_damage.restart();
            }
            i = bullets.erase(i); // удаляем пулю
        } else i++;
    }

    if (helth == 0) return false; // не обрабатываем движения мёртвого игрока

    if (controller.is_II) {
        for (auto &it : events) {
            if (it.type == it.JoystickButtonPressed
                    and it.joystickButton.joystickId == controller.joysticID
                    and it.joystickButton.button == 1) {
                muvement.length = 0;
                controller.is_II = !controller.is_II;
            }
        }

        controller.II_update(bullets, walls, other);
    } else {
        controller.update(events, bullets, other);

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
    }

    controller.clock.restart();
    return true;
}


void Player::auto_drow() {
    window.draw(sprite);
//    window.draw(circle);

    health_scale_line.setPosition(sprite.getGlobalBounds().left + 10 * SIZE_X_SCALE,
                                  sprite.getGlobalBounds().top + 110 * SIZE_Y_SCALE);
    health_scale_line.setSize(Vector2f(HEALTH_LINE_X_LENGTH * SIZE_X_SCALE * helth / 100,
                                       HEALTH_LINE_Y_LENGTH * SIZE_Y_SCALE));

    bullets_count.setPosition(health_scale_line.getPosition().x,
                              health_scale_line.getPosition().y + health_scale_line.getGlobalBounds().height * 2);

    bullets_count.setString(reload_sound.getStatus() == Sound::Playing ? "Reloading" : to_string(bullets));

    window.draw(health_scale_line);
    window.draw(bullets_count);
}


void Player::shoot(list<Bullet> &bullets, float additional_accuracy = 0) {
    if (shoot_cd.getElapsedTime().asSeconds() < SHOOTING_CD) return;
    else shoot_cd.restart();

    if (reload_sound.getStatus() == Sound::Playing or this->bullets == 0) return;

    Vector2f t(-direction.y / direction.x, 1);
    if (direction.x * t.y - direction.y * t.x < 0)
        t = Vector2f(-t.x, -t.y);
    t = Muvement(t).direction; // t и owner->direction - ортонормированный базис. t поможет сдвинуть пулю к дулу
    float f = rand_sign() * (1 - ACCURACY - additional_accuracy) * (rand() % 100 + 1) / 100;

    bullets.emplace_back(Bullet({get_position().x + t.x*10*SIZE_X_SCALE, get_position().y + t.y*10*SIZE_Y_SCALE},
                                Muvement({direction.x + f * t.x, direction.y + f * t.y}), 25));
    bullets.back().sprite.setRotation(sprite.getRotation());
    bullets.back().sprite.move(direction.x * 85 * SIZE_X_SCALE, direction.y * 85 * SIZE_Y_SCALE);

    this->bullets--;
}


void Player::to_position(Vector2f position, Vector2f direction, bool is_II = false) {
    helth = 100.0f;
    bullets = MAX_BULLETS_COUNT;
    sprite.setPosition(position);
    circle.setPosition(sprite.getPosition());
    set_direction(direction);
    controller.clock.restart();
    controller.is_II = is_II;
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


void Player::look_at(Vector2f vec) {
    float dx = abs(get_position().x - vec.x), dy = abs(get_position().y - vec.y);
    float new_r; // новый угл

    if (vec.x > get_position().x) {
        if (vec.y > get_position().y)
            new_r = atan(dy/dx);
        else
            new_r = float(2*M_PI) - atan(dy/dx);
    } else {
        if (vec.y > get_position().y)
            new_r = float(M_PI) - atan(dy/dx);
        else
            new_r = float(M_PI) + atan(dy/dx);
    }

    sprite.setRotation((new_r / M_PI) * 180);
    circle.setRotation(sprite.getRotation());
    this->direction = Vector2f(cos(new_r), sin(new_r));
}


inline void Player::reload() {
    bullets = MAX_BULLETS_COUNT;
    reload_sound.play();
}


Player::Controller::Controller(Player *owner, int joysticID): owner(owner), joysticID(abs(joysticID)), is_II(joysticID < 0) {}


void Player::Controller::II_update(list<Bullet> &bullets, list<Sprite> &walls, Player &other) {
    // получаем вектор движения
    II_data &data = joysticID == 0 ? player1_II_data : player2_II_data;
    Vector2f &movement_vector = owner->muvement.direction;
    auto reset_muvement = [&movement_vector, &data] () {
        float x = float(rand() % 101) / 100;
        movement_vector = {rand_sign() * x, (1 - x) * rand_sign()};
        data.moving_time = seconds(float(rand() % 100) / 30);
        data.moving_cd.restart();
    };

    if (data.moving_cd.getElapsedTime() < data.moving_time) {
        if (not owner->try_move(movement_vector.x * HERO_SPEED * clock.getElapsedTime().asMilliseconds() / 10,
                                movement_vector.y * HERO_SPEED * clock.getElapsedTime().asMilliseconds() / 10,
                                walls))
            reset_muvement();
    } else {
        reset_muvement();
    }

    // получаем вектор направления
    owner->look_at(other.get_position());

    // делаем выстрел если противник на линии огня
    if (is_inline(owner->get_position(), other.get_position(), walls))
        owner->shoot(bullets);

    // регеним жизни
    if (owner->helth < 100 and data.last_damage.getElapsedTime() > seconds(3)) {
        owner->helth += clock.getElapsedTime().asSeconds() * 5;
    }

    // перезарядка если надо
    if (owner->bullets == 0) owner->reload();
}


void Player::Controller::update(list<Event> &events, list<Bullet> &bullets, Player &other) {
    // получаем вектор движения
    Vector2f movement_vector(Joystick::getAxisPosition(joysticID, Joystick::Axis::X), Joystick::getAxisPosition(joysticID, Joystick::Axis::Y));
    if (movement_vector.x*movement_vector.x + movement_vector.y*movement_vector.y
            < MIN_STIC_MOVEMENT_TO_MOVE * MIN_STIC_MOVEMENT_TO_MOVE)
        movement_vector = Vector2f(0, 0);

    // двигаем персоонажа
    owner->muvement.add({movement_vector.x * clock.getElapsedTime().asSeconds(), movement_vector.y * clock.getElapsedTime().asSeconds()});

    // получаем вектор направления
    if (squ(Joystick::getAxisPosition(joysticID, Joystick::Axis::U))
        + squ(Joystick::getAxisPosition(joysticID, Joystick::Axis::V)) > squ(MIN_STIC_MOVEMENT_TO_MOVE)) {
        owner->direction.x = Joystick::getAxisPosition(joysticID, Joystick::Axis::U);
        owner->direction.y = Joystick::getAxisPosition(joysticID, Joystick::Axis::V);

        // нормализуем вектор направления
        float len = sqrt(owner->direction.x * owner->direction.x + owner->direction.y * owner->direction.y);
        owner->direction.x /= len;
        owner->direction.y /= len;
    }

    if (Joystick::isButtonPressed(joysticID, 2) and owner->helth < 100) {
        // восстановление hp
        owner->helth += clock.getElapsedTime().asSeconds() * 7;
        owner->muvement = Muvement(owner->muvement.multiplyed(0.4));
    } else {
        if (Joystick::isButtonPressed(joysticID, 4))
            owner->look_at({other.get_position().x + 10 * GAME_SCALE, other.get_position().y + 10 * GAME_SCALE});

        // далее обработка клавишь
        Event::JoystickButtonEvent event;
        for (Event &it : events) {
            if (it.type != Event::JoystickButtonPressed) continue;

            event = it.joystickButton;
            if (event.joystickId != joysticID) continue;

            switch (event.button) {
                case 5:
                    if (owner->reload_sound.getStatus() != Sound::Playing) owner->shoot(bullets, 0.03f);
                    break;

                case 3:
                    if (owner->reload_sound.getStatus() == Sound::Stopped) owner->reload();
                    break;

                case 1:
                    is_II = !is_II;
                    break;
            }
        }

        if constexpr (ENABLE_AUTO_SHOOTING) { // автоматическая стрельба!
            if (Joystick::getAxisPosition(joysticID, Joystick::Axis::Z) > 20 and owner->reload_sound.getStatus() != Sound::Playing)
                owner->shoot(bullets);
        }
    }
}
