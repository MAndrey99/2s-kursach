#pragma once

#include "header.h"
#include "Player.h"
#include "Bullet.h"
#include "enums.h"

#define PLAYER1_II true
#define PLAYER2_II true

// Отвечает за расположение обьектов на поле и их взаимодействие.
class Field {
    Player player1;
    Player player2;
    list<Sprite> platform;
    list<Sprite> walls;
    list<Bullet> bullets;

public:
    Field();

    void init_walls(); // генерирует рандомное поле
    bool intersect_wall(Sprite &sprite); // праверяет пересечение со стеной для наклонённых прямоугольников
    void players_to_position(); // возвращяет игроков на позиции
    void draw_scene(); // рисует все обьекты на поле
    Winner update(list<Event> &events); // обновляет позиции всех обьектов на карте. false -> конец игры
};
