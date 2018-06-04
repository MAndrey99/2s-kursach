#pragma once

struct II_data {
    Clock moving_cd;
    Clock last_damage;
    Time moving_time = seconds(0);
} player1_II_data, player2_II_data;


// провиряет можно ли провести линию между точками так, чтобы не задеть текстуры стен
bool is_inline(Vector2f a, Vector2f b, list<Sprite> &walls) {
    if (a.x > b.x) swap(a, b);
    b.x -= a.x;
    b.y -= a.y;
    // теперь а слева от b и в b хранится позиция относитьельно а

    Vector2f t(-b.y / b.x, 1);
    if (b.x * t.y - b.y * t.x < 0)
        t = Vector2f(-t.x, -t.y);
    t = Muvement(t).direction; // t и b - ортонормированный базис. t поможет сдвинуть пулю к дулу
    float length = sqrt(squ(b.x) + squ(b.y));

    Sprite rect;
    rect.setColor(Color::Red);
    rect.setTexture(WALL_TEXTURE);
    rect.setTextureRect(IntRect(0, 0, 20*GAME_SCALE, length - 100 * GAME_SCALE));
    rect.setPosition(a.x + t.x*30*GAME_SCALE + b.x * (90*GAME_SCALE/length), a.y + t.y*30*GAME_SCALE + b.y * (90*GAME_SCALE/length));
    rect.setRotation(( (b.y > 0 ? atan(b.y/b.x) : 2*M_PI - atan(-b.y/b.x)) / M_PI) * 180 - 90);
//    window.draw(rect);

    for (Sprite &it : walls)
        if (Collision::BoundingBoxTest(rect, it))
            return false;

    return true;
}
