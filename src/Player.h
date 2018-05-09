#pragma once

#include "header.h"
#include "Muvement.h"
#include "Bullet.h"

#define HEALTH_LINE_X_LENGTH 100
#define HEALTH_LINE_Y_LENGTH 5
#define HERO_SPEED 3

class Player {
    struct Controller {
        nam joysticID = 0;
        Player* owner;
        Clock clock;

        Controller(Player*, nam joysticID);
        void update(list<Event> &events, vector<Bullet> &bullets);
    } controller;

    float helth = 100;

    Sprite sprite;
    Sprite circle; // круг в котором находится сам персонаж. для взаимодействий с текстурами
    RectangleShape health_scale_line; // линия здоровья

    Vector2f direction; // направление взгляда (нормализованный вектор)
    bool try_move(float x, float y, list<Sprite> &walls);

public:
    Muvement muvement; // все передвижение сохраняется сдесь. Спрайт двигается после каждого апдейта

    Player(Vector2f position, Color color, nam joysticID, Vector2f direction);

    bool is_alive() { return helth > 0; }
    Vector2f get_position() { return sprite.getPosition(); }
    bool update(list<Sprite> &walls, vector<Bullet> &bullets, list<Event> &events); // false -> персонаж мёртв
    void auto_drow();
    void shoot(vector<Bullet> &bullets); // сделать выстрел
    void to_position(Vector2f position, Vector2f direction);
    void set_direction(Vector2f direction);
};

