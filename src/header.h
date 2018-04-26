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
#include <cmath>

#define WINDOW_SIZE_X 1500
#define WINDOW_SIZE_Y 1000
#define FRAME_LIMIT 60
#define BULLET_SPEED 2

using namespace std;
using namespace sf;

typedef unsigned char nam;

extern RenderWindow window;

extern Texture WALL_TEXTURE;
extern Texture PLATFORM_TEXTURE;
extern Texture BULLET_TEXTURE;
extern Texture HERO_TEXTURE;
