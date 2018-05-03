#include "header.h"
#include "init.h"
#include "Player.h"
#include "Field.h"

Field field;
Event event;

#if FRAMETIME_LIMIT
Clock frame_clock; // считают время кадра
#endif


#if ENABLE_SOUNDS
// удвляет не звучащие звуки
void sound_control() {
    static Clock clock; // чтобы раз в секунду
    if (clock.getElapsedTime() < seconds(1)) return;

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

    text.setPosition(WINDOW_SIZE_X / 2 - 200, WINDOW_SIZE_Y / 2 - 100);
    text.setFillColor(winner == Winner::PLAYER1 ? Color::Yellow : Color::Red);

    window.clear(Color::Green);
    window.draw(text);
    window.display();

    while (true) {
        sleep(milliseconds(25));

        while (window.pollEvent(event)) {
            switch (event.type) {
                case Event::Closed:
                    window.close();
                    return false;

                case Event::JoystickButtonPressed:
                    if (event.joystickButton.button == 6) {
                        window.close();
                        return false;
                    }

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


int main() {
    window.setTitle("Game");
    window.setFramerateLimit(128);
    window.setMouseCursorVisible(false);

    load_texures_and_sounds();
    field.init_walls();

    list<Event> controller_events;
    Winner temp_winner;

    sleep(seconds(1));

#if FRAMETIME_LIMIT
    frame_clock.restart();
#endif

    while (window.isOpen()) {
#if FRAMETIME_LIMIT
        if (frame_clock.getElapsedTime() > milliseconds(25)) {
            cout << "FRAMETIME overhead: " << frame_clock.getElapsedTime().asMilliseconds() << "ms";
            return -1;
        }
        frame_clock.restart();
#endif

        // обрабатываем события
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

                    controller_events.emplace_back(event);
                    break;
            }
        }

        temp_winner = field.update(controller_events);
        controller_events.clear();

#if ENABLE_SOUNDS
        sound_control();
#endif

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

            field.init_walls();
            field.players_to_position();

#if FRAMETIME_LIMIT
            frame_clock.restart();
#endif
        }

        draw_scene();
    }

    return 0;
}
