#pragma once

#include "header.h"
#include "Player.h"

// Отвечает за расположение обьектов на поле и их взаимодействие.
class Field {
    Texture WALL_TEXTURE;

public:
    Player player;
    list<Sprite> walls;

    Field();

    void drow_scene(); // рисует все обьекты на поле
};
