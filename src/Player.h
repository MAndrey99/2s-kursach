#pragma once

#include "header.h"
#include "Muvement.h"
#include "Bullet.h"

#define HEALTH_LINE_X_LENGTH 100
#define HEALTH_LINE_Y_LENGTH 5
#define HERO_SPEED 3
#define MAX_BULLETS_COUNT 20
#define SHOOTING_CD 0.085f
#define MIN_STIC_MOVEMENT_TO_MOVE 15.0f
#define ENABLE_AUTO_SHOOTING true
#define ACCURACY 0.95f


class Player {
    // этот обьект обрабатывает все события. через него идёт управление персонажем
    struct Controller {
        bool is_II;
        int joysticID; // если меньше нуля, то управляет компьютер. до этого было положительное значение
        Player* owner;
        Clock clock;

        Controller(Player*, int joysticID);
        void update(list<Event> &events, list<Bullet> &bullets, Player &other);
        void II_update(list<Bullet> &bullets, list<Sprite> &walls, Player &other);
    } controller;

    float helth = 100;
    int bullets = MAX_BULLETS_COUNT;

    Sprite sprite;
    Sprite circle; // круг в котором находится сам персонаж. для взаимодействий с текстурами
    RectangleShape health_scale_line; // линия здоровья
    Clock shoot_cd; // расщитывает время с предыдущего выстрела
    Sound reload_sound;
    Text bullets_count;

    Vector2f direction; // направление взгляда (нормализованный вектор)
    bool try_move(float x, float y, list<Sprite> &walls); // двигает персонажа если это возможно

public:
    Muvement muvement; // все передвижение сохраняется сдесь. Спрайт двигается после каждого апдейта

    Player(Vector2f position, Color color, int joysticID, Vector2f direction);
    Player(int joysticID, Color color) : Player({0, 0}, color, joysticID, {0, 1}) {};

    Vector2f get_position() { return sprite.getPosition(); }
    bool update(list<Sprite> &walls, list<Bullet> &bullets, list<Event> &events, Player &other); // false -> персонаж мёртв
    inline void reload(); // перезарядка
    void auto_drow(); // добавляет персонажа и его атрибуты в новый кадр
    void shoot(list<Bullet> &bullets); // сделать выстрел
    void to_position(Vector2f position, Vector2f direction, bool is_II);
    void set_direction(Vector2f direction); // задаёт направление взгляда. !!!direction должен быть нормализован
    void look_at(Vector2f vec); // ставит направление взгляда
};
