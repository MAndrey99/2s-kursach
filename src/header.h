#pragma once

#include <iostream>
#include <list>
#include <cmath>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Audio.hpp>

#include "Collision.h"

#define ENABLE_SOUNDS true
#define GAME_SCALE 0.8f // отвечает за то, во сколько раз будут больше текстуры
#define SIZE_X_SCALE ((float(WINDOW_SIZE_X) / 1920) * GAME_SCALE)
#define SIZE_Y_SCALE ((float(WINDOW_SIZE_Y) / 1080) * GAME_SCALE)

using namespace std;
using namespace sf;

typedef unsigned char nam;

inline RenderWindow window(VideoMode::getDesktopMode(), "Game", Style::Fullscreen);

inline constexpr float zoom = 0.5; // отвечает за то, во сколько раз будет считаться разрешение экрана больше
inline const unsigned int WINDOW_SIZE_X = VideoMode::getDesktopMode().width * zoom;
inline const unsigned int WINDOW_SIZE_Y = VideoMode::getDesktopMode().height * zoom;
inline const float BULLET_SPEED = (WINDOW_SIZE_X / 540) * (WINDOW_SIZE_Y / 460) / (2 * SIZE_X_SCALE * SIZE_Y_SCALE);

inline Texture WALL_TEXTURE;
inline Texture PLATFORM_TEXTURE;
inline Texture BULLET_TEXTURE;
inline Texture HERO_TEXTURE;
inline Texture BOX_TEXTURE;
inline Texture HERO_CIRCLE_TEXTURE;

#if ENABLE_SOUNDS
inline SoundBuffer SHOOT_SOUND;

void sound_control();
#endif

inline Font FD_FONT;

inline list<Sound> background_temp_sounds;

#define squ(a) a*a
