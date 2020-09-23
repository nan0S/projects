#pragma once

#include <list>
#include "Fundament.hpp"
#include "Bullet.hpp"
#include "Const.hpp"
#include "common/useful.hpp"

class Enemy : public HitBox {
public:
    RectangleShape body, gun;
    Vector2f pos;
    float bodyAngle, gunAngle;
    float shootingSpeed, time;
    float epsilon;
    bool life;
    int number;

    Enemy(); 

    void settings(Vector2f pos_, float bodyAngle_ = 0, float shootingSpeed_ = 0, int number_ = -1);
    void update(std::list<Bullet*> &bullets);
    Vector2f getPosition();
    void drawTo(RenderWindow *window);
};
