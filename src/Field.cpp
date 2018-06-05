#include "Field.h"
#include "Collision.h"


Field::Field() : player1(0, Color::Blue), player2(1, Color::Red) {}


void Field::draw_scene() {
    for (Sprite& i : platform) window.draw(i); // рисуем платформу
    for (Sprite& i : walls) window.draw(i); // рисуем стены
    for (Bullet& i : bullets) window.draw(i.sprite); // рисуем пули
    player1.auto_drow(); // рисуем игроков
    player2.auto_drow();
}


Winner Field::update(list<Event> &events) {
    bool need_inc = true;
    for (auto i = bullets.begin(); i != bullets.end();) { // цикл обновления каждой пули
        i->update(); // двигаем пулю

        if (i->sprite.getGlobalBounds().top < 0 or i->sprite.getGlobalBounds().top > WINDOW_SIZE_Y
                or i->sprite.getGlobalBounds().left < 0 or i->sprite.getGlobalBounds().left > WINDOW_SIZE_X) {
             i = bullets.erase(i); // удаляем пулю тк она вне поля
             need_inc = false;
        } else for (Sprite &it : walls)
            if (Collision::BoundingBoxTest(i->sprite, it)) {
                i = bullets.erase(i); // удаляем пулю тк она вне поля
                need_inc = false;
                break;
            }

        if (need_inc) i++;
        need_inc = true;
    }

    if (!player1.update(walls, bullets, events, player2)) return Winner::PLAYER2;
    if (!player2.update(walls, bullets, events, player1)) return Winner::PLAYER1;
    return Winner::NO_ONE;
}


void Field::init_walls() {
    walls.clear(); // чистим тк этот метод может вызываться когда стены и платформа уже сгенерированы
    platform.clear();
    bullets.clear();

    Sprite tmp;
    tmp.setTexture(WALL_TEXTURE);
    tmp.setScale(0.7f * SIZE_X_SCALE, 0.7f * SIZE_Y_SCALE);

    float size_x = tmp.getGlobalBounds().width, size_y = tmp.getGlobalBounds().height;

    for (int i = 0; i * size_y < WINDOW_SIZE_Y; i++) {
        tmp.setPosition(0, i * size_y);
        walls.emplace_back(tmp); // добавляем слева

        tmp.setPosition(WINDOW_SIZE_X - size_x, i * size_y);
        walls.emplace_back(tmp); // добавляем справа
    }

    for (int i = 0; i * size_x < WINDOW_SIZE_X; i++) {
        tmp.setPosition(i * size_x, 0);
        walls.emplace_back(tmp); // добавляем сверху

        tmp.setPosition(i * size_x, WINDOW_SIZE_Y - size_y);
        walls.emplace_back(tmp); // добавляем снизу
    }


    tmp.setTexture(PLATFORM_TEXTURE);
    tmp.setScale(0.5f * SIZE_X_SCALE / zoom, 0.5f * SIZE_Y_SCALE / zoom);
    tmp.setTextureRect(IntRect(0, 0, WINDOW_SIZE_X*3/GAME_SCALE, WINDOW_SIZE_Y*3/GAME_SCALE));
    tmp.setPosition(0, 0);
    platform.emplace_back(tmp); // добавляем фоновую картинку


    // добавляем ящики в рандомные места
    tmp = Sprite(BOX_TEXTURE);
    tmp.setPosition(rand() % int(WINDOW_SIZE_X - 410 * SIZE_X_SCALE) + 210 * SIZE_X_SCALE,
                    rand() % int(WINDOW_SIZE_Y - 410 * SIZE_Y_SCALE) + 210 * SIZE_Y_SCALE);
    tmp.setScale(0.123 * SIZE_X_SCALE, 0.123 * SIZE_Y_SCALE);

    while (!intersect_wall(tmp)) {
        walls.emplace_back(tmp);
        tmp.setRotation(rand());
        tmp.setPosition(rand() % int(WINDOW_SIZE_X - 410 * SIZE_X_SCALE) + 210 * SIZE_X_SCALE,
                        rand() % int(WINDOW_SIZE_Y - 410 * SIZE_Y_SCALE) + 210 * SIZE_Y_SCALE);
    }
}


void Field::players_to_position() {
    player1.to_position(Vector2f(WINDOW_SIZE_X / 12 * SIZE_X_SCALE / zoom, WINDOW_SIZE_Y / 10 * SIZE_Y_SCALE / zoom),
                        Vector2f(1, 0), PLAYER1_II);
    player2.to_position(Vector2f(WINDOW_SIZE_X - WINDOW_SIZE_X * SIZE_X_SCALE / 12 / zoom,
                                 WINDOW_SIZE_Y - WINDOW_SIZE_Y * SIZE_Y_SCALE / 10 / zoom),
                        Vector2f(-1, 0), PLAYER2_II);
}


bool Field::intersect_wall(Sprite &sprite) {
    for (Sprite &s : walls)
        if (Collision::BoundingBoxTest(sprite, s))
            return true;
    return false;
}
