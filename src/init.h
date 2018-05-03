#pragma once

RenderWindow window(VideoMode::getDesktopMode(), "Game", Style::Fullscreen);

const unsigned int WINDOW_SIZE_X = VideoMode::getDesktopMode().width;
const unsigned int WINDOW_SIZE_Y = VideoMode::getDesktopMode().height;

Texture WALL_TEXTURE;
Texture PLATFORM_TEXTURE;
Texture BULLET_TEXTURE;
Texture HERO_TEXTURE;
Texture BOX_TEXTURE;

#if ENABLE_SOUNDS
SoundBuffer SHOOT_SOUND;
#endif

Font FD_FONT;

list<Sound> background_temp_sounds;


void load_texures_and_sounds() {
    BULLET_TEXTURE.loadFromFile("res/bullet.png"); // загружаем текстуру для пуль
    PLATFORM_TEXTURE.loadFromFile("res/grass.jpg"); // загружаем основную текстуру платформы
    WALL_TEXTURE.loadFromFile("res/wall_texture.png"); // загружаем текатуру стен
    HERO_TEXTURE.loadFromFile("res/new_hero.png"); // загружаем текстуру героя
    BOX_TEXTURE.loadFromFile("res/box.jpg");

    PLATFORM_TEXTURE.setRepeated(true); // чтобы делать спрайты длиннее текстур
    WALL_TEXTURE.setRepeated(true);

#if ENABLE_SOUNDS
    SHOOT_SOUND.loadFromFile("res/shoot.wav"); // загружаем музыку
#endif

    FD_FONT.loadFromFile("res/FD.ttf"); // загружаем шрифты
}
