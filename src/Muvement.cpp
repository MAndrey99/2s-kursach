#include <cmath>
#include "Muvement.h"

Muvement::Muvement(sf::Vector2f arg): length((float)sqrt(arg.x*arg.x + arg.y*arg.y)),
                                      direction(arg.x / length, arg.y / length) {}

Muvement::Muvement(): length(0), direction() {}


void Muvement::set_direction(sf::Vector2f new_direction) {
    float n_length = (float)sqrt(new_direction.x*new_direction.x + new_direction.y*new_direction.y); // длинна вектора new_direction
    direction.x = new_direction.x / n_length;
    direction.y = new_direction.y / n_length;
}

sf::Vector2f Muvement::v_sum(sf::Vector2f other) {
    other.x += direction.x * length;
    other.y += direction.y * length;
    return other;
}

Muvement::operator sf::Vector2f() {
    return sf::Vector2f(direction.x * length, direction.y * length);
}

void Muvement::add(sf::Vector2f arg) {
    arg = v_sum(arg);
    length = (float)sqrt(arg.x*arg.x + arg.y*arg.y); // длинна нового вектора
    direction.x = arg.x / length;
    direction.y = arg.y / length;
}
