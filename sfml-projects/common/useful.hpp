#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
using namespace sf;

int randInteger (int a, int b);

long long randLongLong (long long a, long long b);

float randFraction (float a, float b);

bool pressed (Keyboard::Key key);

float dist (Vector2f pos1, Vector2f pos2);

float constrain (float value, float valueA, float valueB, float targetA, float targetB);

void line (Vector2f point1, Vector2f point2, RenderWindow *window);

void Clamp (float &value, float mini, float maxi);

sf::Color hsv(int hue, float sat, float val);
