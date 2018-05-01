#pragma once

#include "header.h"
#include "Player.h"
#include "Bullet.h"
#include "enums.h"

// Отвечает за расположение обьектов на поле и их взаимодействие.
class Field {
    list<Sprite> platform;
    Player player1;
    Player player2;
    list<Sprite> walls;
    vector<Bullet> bullets;

public:
    Field();

    void init_walls();
    void draw_scene(); // рисует все обьекты на поле
    Winner update(list<Event> &events); // обновляет позиции всех обьектов на карте. false -> конец игры
};
