#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

class Circle {
public:
    Vector2f originPos; //pozycja obiektu
    Vector2f relativePos; //wzgledem obiektu z ktorym jest zwiazana
    float baseAngle; //wzgledem obiektu z ktorym jest zwiazany
    float d;
    float R;

    Circle(Vector2f relativePos_, Vector2f originPos_, float R_);

    void setRotation(float angle);
    void setPosition(Vector2f p);

    Vector2f getPos();

    void drawTo(RenderWindow *window);
};

class HitBox {
public:
    std::vector<Circle> things;

    HitBox();

    void addElementToHitBox(Circle c);
    bool isColliding(HitBox *h, float inacuraccy);
    void drawHitBox(RenderWindow *window);
};

class RectangleFundament {
public:
    Vector2f pos;
    bool life;
    Sprite shape;

    RectangleFundament();
    Vector2f getPosition();
    void drawTo(RenderWindow *window);
};

class CircleFundament {
public:
    Vector2f pos;
    bool life;
    CircleShape shape;

    CircleFundament();
    Vector2f getPosition();
    void drawTo(RenderWindow *window);
};
