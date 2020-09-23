#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.hpp"
using namespace sf;

class Collectable : public HitBox, public RectangleFundament {
public:
    float attributeAmount;
    collectableType type;

    Collectable();
    Collectable(Vector2f pos_, Texture &texture, float attributeAmount_, collectableType type_);

    void applyAttributeTo(Player &player);
};
