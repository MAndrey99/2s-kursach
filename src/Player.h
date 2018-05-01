#pragma once

#include "header.h"
#include "Muvement.h"
#include "Bullet.h"

#define HEALTH_LINE_X_LENGTH 100
#define HEALTH_LINE_Y_LENGTH 5

class Player {
    class Controller {
        nam joysticID = 0;
        Player* owner;
        Clock clock;

    public:
        Controller(Player*, nam joysticID);

        void update(list<Event> &events, vector<Bullet> &bullets);
    } controller;

    float speed = 3.0f;
    float helth = 100;

    Sprite sprite;
    CircleShape circleShape; // круг в котором находится сам персонаж. для взаимодействий
    RectangleShape health_scale_line; // линия здоровья

    Vector2f direction = Vector2f(1, 0); // направление взгляда (нормализованный вектор)

public:
    Muvement muvement; // все передвижение сохраняется сдесь. Спрайт двигается после каждого апдейта

    Player(Vector2f position, Color color, nam joysticID);

    bool is_alive() { return helth > 0; }
    Vector2f get_position() { return sprite.getPosition(); }
    bool update(list<Sprite> &walls, vector<Bullet> &bullets, list<Event> &events); // false -> персонаж мёртв
    void auto_drow();
    bool intersects(const Rect<float> &rectangle); // пересикается ли персонаж с другим обьектом
    void shoot(Bullet &bullet); // сделать выстрел
    void to_position(Vector2f position);
};

