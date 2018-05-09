#pragma once

RenderWindow window(VideoMode::getDesktopMode(), "Game", Style::Fullscreen);

const unsigned int WINDOW_SIZE_X = VideoMode::getDesktopMode().width;
const unsigned int WINDOW_SIZE_Y = VideoMode::getDesktopMode().height;

Texture WALL_TEXTURE;
Texture PLATFORM_TEXTURE;
Texture BULLET_TEXTURE;
Texture HERO_TEXTURE;
Texture BOX_TEXTURE;
Texture HERO_CIRCLE_TEXTURE;

#if ENABLE_SOUNDS
SoundBuffer SHOOT_SOUND;
#endif

Font FD_FONT;

list<Sound> background_temp_sounds;


void load_texures_and_sounds() {
    PLATFORM_TEXTURE.loadFromFile("res/grass.jpg"); // загружаем основную текстуру платформы
    Collision::CreateTextureAndBitmask(BULLET_TEXTURE, "res/bullet.png"); // загружаем текстуру для пуль
    Collision::CreateTextureAndBitmask(WALL_TEXTURE, "res/wall_texture.png"); // загружаем текатуру стен
    Collision::CreateTextureAndBitmask(HERO_TEXTURE, "res/new_hero.png"); // загружаем текстуру героя
    Collision::CreateTextureAndBitmask(HERO_CIRCLE_TEXTURE, "res/hero_circle.png");
    Collision::CreateTextureAndBitmask(BOX_TEXTURE, "res/box.jpg");

    PLATFORM_TEXTURE.setRepeated(true); // чтобы делать спрайты длиннее текстур
    WALL_TEXTURE.setRepeated(true);

#if ENABLE_SOUNDS
    SHOOT_SOUND.loadFromFile("res/shoot.wav"); // загружаем музыку
#endif

    FD_FONT.loadFromFile("res/FD.ttf"); // загружаем шрифты
}
