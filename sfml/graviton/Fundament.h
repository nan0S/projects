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

    Circle(Vector2f relativePos_, Vector2f originPos_, float R_) {
        relativePos = relativePos_;
        originPos = originPos_;
        baseAngle = atan2(relativePos.y, relativePos.x) * 180 / PI;
        R = R_;
        d = dist(originPos, originPos + relativePos);
    }

    void setRotation(float angle) {
        relativePos.x = d * cos((angle + baseAngle) * DEGTORAD);
        relativePos.y = d * sin((angle + baseAngle) * DEGTORAD);
    }
    void setPosition(Vector2f p) {
        originPos = p;
    }

    Vector2f getPos() {
        return originPos + relativePos;
    }

    void drawTo(RenderWindow *window) {
        CircleShape shape(R);
        shape.setOrigin(Vector2f(R - 0.5, R - 0.5));
        shape.setFillColor(Color::Transparent);
        shape.setOutlineThickness(0.5);
        shape.setOutlineColor(Color::Yellow);
        shape.setPosition(getPos());
        window->draw(shape);
    }
};

class HitBox {
public:
    std::vector<Circle> things;

    HitBox() {}

    void addElementToHitBox(Circle c) {
        things.push_back(c);
    }

    bool isColliding(HitBox *h, float inacuraccy) {
        for(auto c1 : things) {
            for(auto c2 : h->things) {
                if(dist(c1.getPos(), c2.getPos()) < c1.R + c2.R + inacuraccy) {
                    return true;
                }
            }
        }
        return false;
    }

    void drawHitBox(RenderWindow *window) {
        for(auto t : things) {
            t.drawTo(window);
        }
    }
};

class RectangleFundament {
public:
    Vector2f pos;
    bool life;
    Sprite shape;

    RectangleFundament() {life = true;}

    Vector2f getPosition() {
        return pos;
    }

    void drawTo(RenderWindow *window) {
        window->draw(shape);
    }
};

class CircleFundament {
public:
    Vector2f pos;
    bool life;
    CircleShape shape;

    CircleFundament() {life = true;}

    Vector2f getPosition() {
        return pos;
    }

    void drawTo(RenderWindow *window) {
        window->draw(shape);
    }
};
