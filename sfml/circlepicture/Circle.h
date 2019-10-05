#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
using namespace sf;

class Circle {
public:
    CircleShape shape;
    Vector2f pos;
    float growingSpeed;
    float R, maxRadius;

    Circle() {growingSpeed = 0;}

    Circle(Vector2f pos_, float growingSpeed_, float maxRadius_, Color color) {
        shape.setFillColor(color);
        pos = pos_;
        growingSpeed = growingSpeed_;
        R = 0;
        maxRadius = maxRadius_;
    }

    void isDead() {
        if(R >= maxRadius)
            growingSpeed = 0;
    }

    void update() {
        R += growingSpeed;
        isDead();

        shape.setRadius(R);
        shape.setOrigin(Vector2f(R, R));
        shape.setPosition(pos);
    }

    void drawTo(RenderWindow &window) {
        window.draw(shape);
    }
};
