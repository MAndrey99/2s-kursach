#pragma once

#include "header.h"

struct Muvement {
    Vector2f direction; // нормолизованый вектор движения. те направление.
    float length; // длинна вектора движения

    explicit Muvement(Vector2f);
    Muvement();

    Vector2f v_sum(Vector2f other); // складывается с другим вектором
    void add(Vector2f);
    Vector2f multiplyed(float scale); // получение вектора движения, умноженного на число

    operator Vector2f();
};
