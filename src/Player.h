#pragma once


#include "header.h"
#include "Muvement.h"

class Player {
    class Controller {
        nam joysticID = 0;
        Player* owner;
        Clock clock;

    public:
        Controller(Player*);

        void update(list<Event>&);
    } controller;

    float speed = 2.0f;
    int helth = 100;

    Texture texture;
    Sprite sprite;
    CircleShape circleShape; // круг в котором находится сам персонаж. для взаимодействий.

    Vector2f direction = Vector2f(1, 0); // направление взгляда (нормализованный вектор)

public:
    Muvement muvement; // все передвижение сохраняется сдесь. Спрайт двигается после каждого апдейта.

    Player(Vector2f position);

    void update(list<Event>&, list<Sprite> &walls);
    void auto_drow();
};

