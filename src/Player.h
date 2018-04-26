#pragma once


#include "header.h"
#include "Muvement.h"
#include "Bullet.h"

class Player {
    class Controller {
        nam joysticID = 0;
        Player* owner;
        Clock clock;

    public:
        Controller(Player*);

        void update(list<Event> &events, vector<Bullet> &bullets);
    } controller;

    float speed = 3.0f;
    float helth = 100;

    Texture texture;
    Sprite sprite;
    CircleShape circleShape; // круг в котором находится сам персонаж. для взаимодействий

    Vector2f direction = Vector2f(1, 0); // направление взгляда (нормализованный вектор)

public:
    Muvement muvement; // все передвижение сохраняется сдесь. Спрайт двигается после каждого апдейта

    Player(Vector2f position);

    bool is_alive() { return helth > 0; }
    Vector2f get_position() { return sprite.getPosition(); }
    void update(list<Sprite> &walls, vector<Bullet> &bullets, list<Event> &events);
    void auto_drow();
    bool intersects(const Rect<float> &rectangle); // пересикается ли персонаж с другим обьектом
    void shoot(Bullet &bullet); // сделать выстрел
};

