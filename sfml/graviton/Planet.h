#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

class Planet : public HitBox, public CircleFundament {
public:
    float R;
    float mass;
    int number;

    Planet() {}

    Planet(Vector2f pos_, float R_, int number_ = 0) {
        R = R_;
        float outlineThick = std::min(R / 13, 4.f);
        pos = pos_;

        shape.setRadius(R - outlineThick);
        shape.setOrigin(R, R);
        shape.setPosition(pos);
        shape.setFillColor(Color::Black);
        shape.setOutlineThickness(outlineThick);
        shape.setOutlineColor(Color::Green);

        mass = R * ro;
        life = true;
        number = number_;

        addElementToHitBox(Circle(Vector2f(0, 0), pos, R));
    }
};
