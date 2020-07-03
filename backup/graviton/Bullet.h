#include<SFML/Graphics.hpp>
#include<iostream>
using namespace sf;

class Bullet : public HitBox, public RectangleFundament {
public:
    Vector2f v;
    float angle;
    int number;
    std::string name;

    Bullet() {}

    void settings(Vector2f pos_, Vector2f v_, float angle_, int number_, Texture &tex, std::string name_) {
        shape.setTexture(tex);
        shape.setTextureRect(IntRect(0, 0, 32, 64));
        shape.setOrigin(Vector2f(shape.getGlobalBounds().width / 2, shape.getGlobalBounds().height / 2));
        shape.setScale(0.55, 0.55);

        v = v_;
        pos = pos_;
        angle = angle_;
        life = true;
        name = name_;
        number = number_;

        float scale = 0.55;

        addElementToHitBox(Circle(Vector2f(0, -6), pos, 4 * scale));
        addElementToHitBox(Circle(Vector2f(0, -14 * scale), pos, 6 * scale));
        addElementToHitBox(Circle(Vector2f(0, -22 * scale), pos, 4 * scale));
    }

    void update() {
        pos.x += v.x * timeSlowDown; pos.y += v.y * timeSlowDown;
        shape.setPosition(pos);
        shape.setRotation(angle + 90);

        for(int i = 0; i < things.size(); ++i) {
            things[i].setRotation(angle + 90);
            things[i].setPosition(pos);
        }
    }
};
