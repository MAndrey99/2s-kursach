#pragma once

#include <iostream>
#include <list>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>

#include "Collision.h"

#define BULLET_SPEED 3
#define DRAW_FPS true
#define ENABLE_SOUNDS true
#define FRAMETIME_LIMIT false
#define BOXES_MIDDLE_COUNT (WINDOW_SIZE_X * WINDOW_SIZE_Y / 10)

using namespace std;
using namespace sf;

typedef unsigned char nam;

extern RenderWindow window;

extern const unsigned int WINDOW_SIZE_X;
extern const unsigned int WINDOW_SIZE_Y;

extern Texture WALL_TEXTURE;
extern Texture PLATFORM_TEXTURE;
extern Texture BULLET_TEXTURE;
extern Texture HERO_TEXTURE;
extern Texture BOX_TEXTURE;
extern Texture HERO_CIRCLE_TEXTURE;

#if ENABLE_SOUNDS
extern SoundBuffer SHOOT_SOUND;
#endif

extern Font FD_FONT;

extern list<Sound> background_temp_sounds;

#define squ(a) a*a
