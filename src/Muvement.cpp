#include <cmath>
#include "Muvement.h"

Muvement::Muvement(sf::Vector2f arg): length(sqrt(squ(arg.x) * SIZE_X_SCALE + squ(arg.y) * SIZE_Y_SCALE)) {
    direction = length > 0 ? Vector2f(arg.x / length, arg.y / length) : Vector2f(0, 1);
}

Muvement::Muvement(): length(0), direction() {}


void Muvement::set_direction(sf::Vector2f new_direction) {
    float n_length = sqrt(squ(new_direction.x) + squ(new_direction.y)); // длинна вектора new_direction
    direction.x = new_direction.x / n_length;
    direction.y = new_direction.y / n_length;
}

sf::Vector2f Muvement::v_sum(sf::Vector2f other) {
    other.x += direction.x * length;
    other.y += direction.y * length;
    return other;
}

Muvement::operator Vector2f() {
    return sf::Vector2f(direction.x * length, direction.y * length);
}

void Muvement::add(sf::Vector2f arg) {
    arg = v_sum(arg);
    length = sqrt(arg.x*arg.x + arg.y*arg.y); // длинна нового вектора

    if (length != 0) {
        direction.x = arg.x / length;
        direction.y = arg.y / length;
    }
}


Vector2f Muvement::multiplyed(float scale) {
    return sf::Vector2f(direction.x * length * scale * SIZE_X_SCALE, direction.y * length * scale * SIZE_Y_SCALE);
}
