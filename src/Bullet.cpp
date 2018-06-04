#include "Bullet.h"


Bullet::Bullet(Vector2f position, Muvement muvement, int damage): damage(damage), muvement(muvement) {
    sprite.setTexture(BULLET_TEXTURE);
    sprite.setPosition(position);
    sprite.setScale(SIZE_X_SCALE, SIZE_Y_SCALE);

    sound_control();
    background_temp_sounds.emplace_back(Sound(SHOOT_SOUND));
    background_temp_sounds.back().play();
}


void Bullet::update() {
    sprite.move(muvement.multiplyed(clock.getElapsedTime().asMilliseconds() * BULLET_SPEED));
    clock.restart();
}
