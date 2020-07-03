#pragma once

#include<SFML/Graphics.hpp>
#include<iostream>
#include <list>
#include "Fundament.hpp"
#include "Const.hpp"
#include "Planet.hpp"
using namespace sf;

class Player : public HitBox, public RectangleFundament {
public:
    Vector2f v;
    float angle;
    float thrust;
    float mass;
    float fuel;
    int lifeNumber;
    playerState movementState;

    Player();

    void settings(Vector2f pos_, Vector2f v_, float angle_, float mass_, Texture &tex);
    void move();
    void direction();
    void gravityInfluence(std::list<Planet*> &planets);
    void force(float Fx, float Fy);
    void update(std::list<Planet*> &planets);
    void death();
    void reset();
};
