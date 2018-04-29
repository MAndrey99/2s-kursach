#include "header.h"
#include "Player.h"
#include "Field.h"

RenderWindow window(VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), "Game", Style::Close);
Field field;
Clock sync_clock;

Texture WALL_TEXTURE;
Texture PLATFORM_TEXTURE;
Texture BULLET_TEXTURE;
Texture HERO_TEXTURE;

SoundBuffer SHOOT_SOUND;

list<Sound> background_temp_sounds;

void load_texures_and_sounds() {
    BULLET_TEXTURE.loadFromFile("res/bullet.png"); // загружаем текстуру для пуль
    PLATFORM_TEXTURE.loadFromFile("res/grass.jpg"); // загружаем основную текстуру платформы
    WALL_TEXTURE.loadFromFile("res/wall_texture.png"); // загружаем текатуру стен
    HERO_TEXTURE.loadFromFile("res/new_hero.png"); // загружаем текстуру героя

    PLATFORM_TEXTURE.setRepeated(true); // чтобы делать спрайты длиннее текстур
    WALL_TEXTURE.setRepeated(true);

    SHOOT_SOUND.loadFromFile("res/shoot.wav"); // загружаем музыку
}


void sound_control() {
    // удаляем звуки, которые уже не звучат
    for (auto it = background_temp_sounds.begin(); it != background_temp_sounds.end(); ++it)
        if (it->getStatus() != it->Playing)
            background_temp_sounds.erase(it);
}


int main()
{
    load_texures_and_sounds();
    field.init_walls();

    Event event{};
    list<Event> controller_events;

    while (window.isOpen()) {
        sleep(milliseconds(1000 / FRAME_LIMIT) - sync_clock.getElapsedTime());
        sync_clock.restart();

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();

            if (event.type == Event::JoystickButtonPressed)
                controller_events.emplace_back(event);
        }

        field.update(controller_events);
        sound_control();
        controller_events.clear();

        field.draw_scene();
        window.display();
        window.clear(Color::Cyan);
    }

    return 0;
}
