#include "header.h"
#include "init.h"
#include "Player.h"
#include "Field.h"

Field field;
Clock sync_clock;
Event event;


void load_texures_and_sounds() {
    BULLET_TEXTURE.loadFromFile("res/bullet.png"); // загружаем текстуру для пуль
    PLATFORM_TEXTURE.loadFromFile("res/grass.jpg"); // загружаем основную текстуру платформы
    WALL_TEXTURE.loadFromFile("res/wall_texture.png"); // загружаем текатуру стен
    HERO_TEXTURE.loadFromFile("res/new_hero.png"); // загружаем текстуру героя

    PLATFORM_TEXTURE.setRepeated(true); // чтобы делать спрайты длиннее текстур
    WALL_TEXTURE.setRepeated(true);

#if ENABLE_SOUNDS
    SHOOT_SOUND.loadFromFile("res/shoot.wav"); // загружаем музыку
#endif

    FD_FONT.loadFromFile("res/FD.ttf"); // загружаем шрифты
}


#if ENABLE_SOUNDS
void sound_control() {
    // удаляем звуки, которые уже не звучат
    for (auto it = background_temp_sounds.begin(); it != background_temp_sounds.end(); ++it)
        if (it->getStatus() != it->Playing)
            background_temp_sounds.erase(it);
}
#endif


void draw_fps() {
    static Clock clock;
    static int pictures_count;
    static Text fps_text("FPS: 00", FD_FONT, 20);

    if (clock.getElapsedTime() >= seconds(1)) {
        fps_text.setString(string("FPS: ") + to_string(pictures_count));
        pictures_count = 0;
        clock.restart();
    } else ++pictures_count;

    window.draw(fps_text);
}


bool show_winner(Winner winner) {
    Text text(string(winner == Winner::PLAYER1 ? "Player1" : "Player2") + " won", FD_FONT, 50);

    text.setPosition(WINDOW_SIZE_X / 3, WINDOW_SIZE_Y / 3);
    text.setFillColor(winner == Winner::PLAYER1 ? Color::Yellow : Color::Red);

    window.clear(Color::Green);
    window.draw(text);
    window.display();

    while (true) {
        sleep(milliseconds(25));

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return false;
            }

            if (event.type == event.JoystickButtonPressed) {
                if (event.joystickButton.button == 0) return true;
            }
        }
    }
}


inline void draw_scene() {
    field.draw_scene();
    if constexpr (DRAW_FPS) draw_fps();
    window.display();
    window.clear(Color::Cyan);
}

inline void sync() {
    sleep(milliseconds(1000 / FRAME_LIMIT) - sync_clock.getElapsedTime());
    sync_clock.restart();
}


int main() {
    load_texures_and_sounds();
    field.init_walls();

    list<Event> controller_events;
    Winner temp_winner;

    while (window.isOpen()) {
        sync();

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();

            if (event.type == Event::JoystickButtonPressed)
                controller_events.emplace_back(event);
        }

        temp_winner = field.update(controller_events);

#if ENABLE_SOUNDS
        sound_control();
#endif

        controller_events.clear();

        if (temp_winner != Winner::NO_ONE) {
            // в течении секунды поле будет изменятся
            Clock tmp;
            while (tmp.getElapsedTime() < seconds(1)) {
                sync();
                field.update(controller_events);
                draw_scene();
            }

            if (!show_winner(temp_winner)) {
                window.close();
                return 0;
            }

            field.init_walls();
            field.players_to_position();
        }

        draw_scene();
    }

    return 0;
}
