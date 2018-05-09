#include "Field.h"
#include "Collision.h"


Field::Field(): player1(Vector2f(WINDOW_SIZE_X / 12, WINDOW_SIZE_Y / 10), Color::Yellow, 0, Vector2f(1, 0)),
                player2(Vector2f(WINDOW_SIZE_X - WINDOW_SIZE_X / 12, WINDOW_SIZE_Y - WINDOW_SIZE_Y / 10), Color::Red, 1, Vector2f(-1, 0))
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
            if (Collision::BoundingBoxTest(bullets[i].sprite, it)
                    or bullets[i].sprite.getGlobalBounds().top < 0 or bullets[i].sprite.getGlobalBounds().top > WINDOW_SIZE_Y
                    or bullets[i].sprite.getGlobalBounds().left < 0 or bullets[i].sprite.getGlobalBounds().left > WINDOW_SIZE_X) {
                bullets.erase(bullets.begin() + i); // удаляем пулю тк она в стене или вне поля
                break;
            }
    }

    if (!player1.update(walls, bullets, events)) return Winner::PLAYER2;
    if (!player2.update(walls, bullets, events)) return Winner::PLAYER1;
    return Winner::NO_ONE;
}


void Field::init_walls() {
    walls.clear();
    platform.clear();
    bullets.clear();

    Sprite tmp;
    tmp.setTexture(WALL_TEXTURE);
    tmp.setScale(0.7, 0.7);

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


    tmp.setTexture(PLATFORM_TEXTURE);
    tmp.setScale(0.5, 0.5);
    tmp.setTextureRect(IntRect(0, 0, WINDOW_SIZE_X*2, WINDOW_SIZE_Y*2));
    tmp.setPosition(0, 0);
    platform.emplace_back(tmp); // добавляем фоновую картинку


    // добавляем ящики в рандомные места
    tmp = Sprite(BOX_TEXTURE);
    tmp.setPosition(rand() % (WINDOW_SIZE_X - 400) + 200, rand() % (WINDOW_SIZE_Y - 400) + 200);
    tmp.setScale(0.123, 0.123);

    while (!intersect_wall(tmp) and rand() % (BOXES_MIDDLE_COUNT + 1)) {
        walls.emplace_back(tmp);
        tmp.setRotation(rand());
        tmp.setPosition(rand() % (WINDOW_SIZE_X - 400) + 200, rand() % (WINDOW_SIZE_Y - 400) + 200);
    }
}


void Field::players_to_position() {
    player1.to_position(Vector2f(WINDOW_SIZE_X / 12, WINDOW_SIZE_Y / 10), Vector2f(1, 0));
    player2.to_position(Vector2f(WINDOW_SIZE_X - WINDOW_SIZE_X / 12, WINDOW_SIZE_Y - WINDOW_SIZE_Y / 10), Vector2f(-1, 0));
}


bool Field::intersect_wall(Sprite &sprite) {
    for (Sprite &s : walls)
        if (Collision::BoundingBoxTest(sprite, s))
            return true;
    return false;
}
