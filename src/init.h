#pragma once

RenderWindow window(VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), "Game", Style::Close);

Texture WALL_TEXTURE;
Texture PLATFORM_TEXTURE;
Texture BULLET_TEXTURE;
Texture HERO_TEXTURE;

SoundBuffer SHOOT_SOUND;

Font FD_FONT;

list<Sound> background_temp_sounds;
