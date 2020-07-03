#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include "Const.hpp"
using namespace sf;

class Interface {
public:
    RectangleShape fuelShape;
    Text fuelText, fuelAmountText;

    Interface();

    void applyFuel(float &fuel);
    void applyCameraPosition(Vector2f &pos, Vector2f &range);
    void update(float fuel, Vector2f pos, Vector2f range);
    void drawTo(RenderWindow *window);
};
