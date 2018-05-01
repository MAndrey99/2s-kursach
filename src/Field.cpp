#include "Field.h"


Field::Field(): player1(Vector2f(WINDOW_SIZE_X / 10, WINDOW_SIZE_Y / 10), Color::Yellow, 0),
                player2(Vector2f(WINDOW_SIZE_X - WINDOW_SIZE_X / 10, WINDOW_SIZE_Y - WINDOW_SIZE_Y / 10), Color::Red, 1)
                {}


void Field::draw_scene() {
    for (Sprite& i : platform) window.draw(i); // рисуем платформу
    for (Bullet& i : bullets) window.draw(i.sprite); // рисуем пули
    for (Sprite& i : walls) window.draw(i); // рисуем стены
    player1.auto_drow(); // рисуем игроков
    player2.auto_drow();
}


Winner Field::update(list<Event> &events) {
    for (int i = 0; i < bullets.size(); i++) {
        bullets[i].update(); // двигаем пулю

        for (Sprite &it : walls)
            if (it.getGlobalBounds().intersects(bullets[i].sprite.getGlobalBounds()))
                bullets.erase(bullets.begin() + i); // удаляем пулю тк она в стене
    }

    if (!player1.update(walls, bullets, events)) return Winner::PLAYER1;
    if (!player2.update(walls, bullets, events)) return Winner::PLAYER2;
    return Winner::NO_ONE;
}

void Field::init_walls() {
    Sprite tmp;
    tmp.setTexture(WALL_TEXTURE);
    tmp.setScale(0.7, 0.7);

    tmp.setPosition(0, 0);
    tmp.setTextureRect(IntRect(0, 0, WALL_TEXTURE.getSize().x * (1 / 0.7), WINDOW_SIZE_Y * (1 / 0.7)));
    walls.emplace_back(tmp); // добавляем левую

    tmp.setPosition(WINDOW_SIZE_X - tmp.getGlobalBounds().width, 0);
    walls.emplace_back(tmp); // добавляем правую

    tmp.setPosition(tmp.getGlobalBounds().width, 0);
    tmp.setTextureRect(IntRect(0, 0, (WINDOW_SIZE_X - tmp.getGlobalBounds().width * 2) * (1 / 0.7), WALL_TEXTURE.getSize().y));
    walls.emplace_back(tmp); // добавляем верхную

    tmp.setPosition(WALL_TEXTURE.getSize().x, WINDOW_SIZE_Y - tmp.getGlobalBounds().height);
    walls.emplace_back(tmp); // добавляем нижнию

    tmp.setTexture(PLATFORM_TEXTURE);
    tmp.setScale(0.5, 0.5);
    tmp.setTextureRect(IntRect(0, 0, WINDOW_SIZE_X*2, WINDOW_SIZE_Y*2));
    tmp.setPosition(0, 0);
    platform.emplace_back(tmp);
}
