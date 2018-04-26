#pragma once

#include "header.h"

class Muvement {
    Vector2f direction; // нормолизованый вектор движения. те направление.

public:
    float length; // длинна вектора движения

    explicit Muvement(Vector2f);
    Muvement();

    Vector2f v_sum(Vector2f other); // складывается с другим вектором
    void set_direction(Vector2f);
    void add(Vector2f);
    Vector2f get_direction() { return direction; }
    Vector2f multiplyed(float scale); // получение вектора движения, умноженного на число

    operator Vector2f();
};
