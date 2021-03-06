#include "header.h"
#include "Player.h"
#include "Field.h"

Field field;
Event event;
View view(FloatRect(0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y));


// удвляет не звучащие звуки
void sound_control() {
    // удаляем звуки, которые уже не звучат
    for (auto it = background_temp_sounds.begin(); it != background_temp_sounds.end(); ++it)
        if (it->getStatus() != it->Playing)
            background_temp_sounds.erase(it);
}


inline void load_texures_and_sounds() {
    PLATFORM_TEXTURE.loadFromFile("res/grass.jpg"); // загружаем основную текстуру платформы
    Collision::CreateTextureAndBitmask(BULLET_TEXTURE, "res/bullet.png"); // загружаем текстуру для пуль
    Collision::CreateTextureAndBitmask(WALL_TEXTURE, "res/wall_texture.png"); // загружаем текатуру стен
    Collision::CreateTextureAndBitmask(HERO_TEXTURE, "res/new_hero.png"); // загружаем текстуру героя
    Collision::CreateTextureAndBitmask(HERO_CIRCLE_TEXTURE, "res/hero_circle.png");
    Collision::CreateTextureAndBitmask(BOX_TEXTURE, "res/box.jpg");

    PLATFORM_TEXTURE.setRepeated(true); // чтобы делать спрайты длиннее текстур
    WALL_TEXTURE.setRepeated(true);

    // загружаем звуки
    SHOOT_SOUND.loadFromFile("res/shoot.wav");
    RELOAD_SOUND.loadFromFile("res/reload.wav");

    FD_FONT.loadFromFile("res/FD.ttf"); // загружаем шрифты
}


// экран, отображающий победителя
bool show_winner(Winner winner) {
    Text text(string(winner == Winner::PLAYER1 ? "Player1" : "Player2") + " won", FD_FONT, 50 * zoom);

    text.setPosition(WINDOW_SIZE_X / 2 - 200 * SIZE_X_SCALE, WINDOW_SIZE_Y / 2 - 100 * SIZE_Y_SCALE); // задаём цвет и позицию надписи
    text.setFillColor(winner == Winner::PLAYER1 ? Color::Yellow : Color::Red);

    // рисуем надпись
    window.clear(Color::Green);
    window.draw(text);
    window.display();

    while (true) {
        sleep(milliseconds(25));

        // обрабатываем нажатия клавишь
        while (window.pollEvent(event)) {
            switch (event.type) {
                case Event::Closed:
                    window.close();
                    return false;

                case Event::JoystickButtonPressed:
                    // клавиша на геймпаде для закрытия игры
                    if (event.joystickButton.button == 6) {
                        window.close();
                        return false;
                    }

                    if (event.joystickButton.button == 0) return true;
            }
        }
    }
}


bool menu() {
    Text one("One player", FD_FONT, 50 * zoom);
    Text two("Two players", FD_FONT, 50 * zoom);
    Sprite fon(PLATFORM_TEXTURE, IntRect(0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y));
    bool one_player = true;

    one.setPosition(WINDOW_SIZE_X / 2 - 200 * SIZE_X_SCALE, WINDOW_SIZE_Y / 3 - 100 * SIZE_Y_SCALE); // задаём цвет и позицию надписи
    two.setPosition(WINDOW_SIZE_X / 2 - 200 * SIZE_X_SCALE, WINDOW_SIZE_Y * 2 / 3 - 100 * SIZE_Y_SCALE);
    one.setFillColor(Color::Red);
    two.setFillColor(Color::Black);

    // рисуем надписьи
    window.clear(Color::Green);
    window.draw(fon);
    window.draw(one);
    window.draw(two);
    window.display();

    while (true) {
        sleep(milliseconds(25));

        // обрабатываем нажатия клавишь
        while (window.pollEvent(event)) {
            switch (event.type) {
                case Event::Closed:
                    window.close();
                    return false;

                case Event::JoystickButtonReleased:
                    // клавиша на геймпаде для закрытия игры
                    if (event.joystickButton.button == 6) {
                        window.close();
                        return false;
                    }

                    if (event.joystickButton.button == 0) {
                        PLAYER2_II = one_player;
                        return true;
                    }

                    break;

                case Event::JoystickMoved:
                    if (event.joystickMove.axis == Joystick::Axis::PovY and event.joystickMove.position != 0) {
                        one_player = not one_player;

                        one.setFillColor(one_player ? Color::Red : Color::Black);
                        two.setFillColor(one_player ? Color::Black : Color::Red);

                        // рисуем надписьи
                        window.clear(Color::Green);
                        window.draw(fon);
                        window.draw(one);
                        window.draw(two);
                        window.display();
                    }
            }
        }
    }
}


inline void draw_scene() {
    field.draw_scene(); // добавляем обьекты на окно
    window.display(); // отображаем
    window.clear(Color::Cyan); // чистим обьекты и задаём фон
}


int WinMain() {
    srand(time(NULL)); // инициализация рандома

    // настраеваем окно
    window.setTitle("Game");
    window.setMouseCursorVisible(false);
    window.setView(view);
    window.setVerticalSyncEnabled(true);

    load_texures_and_sounds(); // загружаем текстуры и звуки
    if (not menu()) return 0;
    field.init_walls(); // генерируем поле
    field.players_to_position(); // отправляем игроков на позиции

    list<Event> controller_events;
    Winner temp_winner;

    while (window.isOpen()) {
        // обрабатываем событийя
        while (window.pollEvent(event)) {
            switch (event.type) {
                case Event::Closed:
                    window.close();
                    return 0;

                case Event::JoystickButtonPressed:
                    if (event.joystickButton.button == 6) {
                        window.close();
                        return 0;
                    }

                    if (event.joystickButton.button == 7) {
                        if (not menu()) return 0;

                        // заново генерируем поле и выставляем персонажей
                        field.init_walls();
                        field.players_to_position();
                    }

                    controller_events.emplace_back(event);
                    break;

                case Event::KeyPressed:
                    if (event.key.code == Keyboard::Key::Escape) {
                        window.close();
                        return 0;
                    }
                    break;
            }
        }

        temp_winner = field.update(controller_events); // обновляем поле, двигаем обьекты
        controller_events.clear();

        if (temp_winner != Winner::NO_ONE) {
            // в течении секунды поле будет изменятся
            Clock tmp;
            while (tmp.getElapsedTime() < seconds(1)) {
                field.update(controller_events);
                draw_scene();
            }

            if (!show_winner(temp_winner)) {
                window.close();
                return 0;
            }

            // заново генерируем поле и выставляем персонажей
            field.init_walls();
            field.players_to_position();
        }

        draw_scene(); // выводим кадр на экран
    }

    return 0;
}
