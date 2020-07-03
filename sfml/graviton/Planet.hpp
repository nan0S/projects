#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Fundament.hpp"
using namespace sf;

class Planet : public HitBox, public CircleFundament {
public:
    float R;
    float mass;
    int number;

    Planet();
    Planet(Vector2f pos_, float R_, int number_ = 0);
};
