#pragma once

#include<SFML/Graphics.hpp>
#include<iostream>
#include "Fundament.hpp"
using namespace sf;

class Bullet : public HitBox, public RectangleFundament {
public:
    Vector2f v;
    float angle;
    int number;
    std::string name;

    Bullet();
    void settings(Vector2f pos_, Vector2f v_, float angle_, int number_, Texture &tex, std::string name_);
    void update();
};
