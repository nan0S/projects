#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.hpp"
using namespace sf;

class Camera {
public:
    Vector2f pos;
    Vector2f range;
    Vector2f v;

    Camera();

    void settings(Vector2f pos_, Vector2f range_);

    void move(Player &player);
    void force(float Fx, float Fy);
    void applyCameraTo(RenderWindow *window);
    void update(Player &player, RenderWindow *window);

    Vector2f getPosition();
    Vector2f getRange();
};
