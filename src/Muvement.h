#pragma once


#include <SFML/System.hpp>

class Muvement {
    sf::Vector2f direction; // нормолизованый вектор движения. те направление.

public:
    float length; // длинна вектора движения

    explicit Muvement(sf::Vector2f);
    Muvement();

    sf::Vector2f v_sum(sf::Vector2f other); // складывается с другим вектором
    void set_direction(sf::Vector2f);
    void add(sf::Vector2f);
    sf::Vector2f get_direction() { return direction; }

    operator sf::Vector2f();
};
