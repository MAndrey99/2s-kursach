#include "Bullet.h"


Bullet::Bullet(Vector2f position, Muvement muvement, int damage): damage(damage), muvement(muvement) {
    sprite.setTexture(BULLET_TEXTURE);
    sprite.setPosition(position);

    Sound sound(SHOOT_SOUND);
    background_temp_sounds.emplace_back(sound);
    background_temp_sounds.back().play();
}


void Bullet::update() {
    sprite.move(muvement.multiplyed(clock.getElapsedTime().asMilliseconds() * BULLET_SPEED));
    clock.restart();
}
