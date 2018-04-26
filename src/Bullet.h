#pragma once

#include "header.h"
#include "Muvement.h"


class Bullet {
    Muvement muvement;  // содержит направление и скорость
    Clock clock; // время с предыдущего движения

public:
    Sprite sprite;
    int damage;

    Bullet(Vector2f position, Muvement muvement, int damage);
    void update();
};
