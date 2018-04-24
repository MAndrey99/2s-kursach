#include "Field.h"


Field::Field(): player(Vector2f(WINDOW_SIZE_X / 2, WINDOW_SIZE_Y /2)) {
    WALL_TEXTURE.loadFromFile("res/wall_texture.png");
    Sprite tmp;
    tmp.setTexture(WALL_TEXTURE);
    tmp.setScale(0.5, 0.5);

    float size_x = tmp.getGlobalBounds().width, size_y = tmp.getGlobalBounds().height;

    for (int i = 0; i * size_y < WINDOW_SIZE_Y; i++) {
        tmp.setPosition(0, i * size_y);
        walls.emplace_back(tmp); // добавляем слева

        tmp.setPosition(WINDOW_SIZE_X - size_x, i * size_y);
        walls.emplace_back(tmp); // добавляем справа
    }

    for (int i = 0; i * size_x < WINDOW_SIZE_X; i++) {
        tmp.setPosition(i *size_x, 0);
        walls.emplace_back(tmp); // добавляем слева

        tmp.setPosition(i * size_x, WINDOW_SIZE_Y - size_y);
        walls.emplace_back(tmp); // добавляем справа
    }
}

void Field::drow_scene() {
    player.auto_drow();

    for (Sprite& i : walls) window.draw(i); // ресуем стены
}
