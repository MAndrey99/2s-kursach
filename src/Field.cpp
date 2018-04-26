#include "Field.h"


Field::Field(): player(Vector2f(WINDOW_SIZE_X / 2, WINDOW_SIZE_Y /2)) {}


void Field::draw_scene() {
    for (Sprite& i : platform) window.draw(i); // рисуем платформу
    for (Bullet& i : bullets) window.draw(i.sprite); // рисуем пули
    for (Sprite& i : walls) window.draw(i); // рисуем стены
    player.auto_drow(); // рисуем игрока
}


void Field::update(list<Event> &events) {
    for (int i = 0; i < bullets.size(); i++) {
        bullets[i].update(); // двигаем пулю

        for (Sprite &it : walls)
            if (it.getGlobalBounds().intersects(bullets[i].sprite.getGlobalBounds()))
                bullets.erase(bullets.begin() + i); // удаляем пулю тк она в стене
    }

    player.update(walls, bullets, events);
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
