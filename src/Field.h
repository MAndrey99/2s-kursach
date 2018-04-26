#pragma once

#include "header.h"
#include "Player.h"
#include "Bullet.h"

// Отвечает за расположение обьектов на поле и их взаимодействие.
class Field {
    list<Sprite> platform;
    Player player;
    list<Sprite> walls;
    vector<Bullet> bullets;
public:
    Field();

    void init_walls();
    void draw_scene(); // рисует все обьекты на поле
    void update(list<Event> &events); // обновляет позиции всех обьектов на карте(например пуль) кроме персоонажей
};
