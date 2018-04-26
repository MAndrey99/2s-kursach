#include "Bullet.h"


Bullet::Bullet(Vector2f position, Muvement muvement, int damage): damage(damage), muvement(muvement) {
    sprite.setTexture(BULLET_TEXTURE);
    sprite.setPosition(position);
}


void Bullet::update() {
    sprite.move(muvement.multiplyed(clock.getElapsedTime().asMilliseconds() * BULLET_SPEED));
    clock.restart();
}
