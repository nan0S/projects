#include<SFML/Graphics.hpp>
#include<iostream>
using namespace sf;

class Player : public HitBox, public RectangleFundament {
public:
    Vector2f v;
    float angle;
    float thrust;
    float mass;
    float fuel;
    int lifeNumber;
    playerState movementState;

    Player() {
        thrust = pThrust;
        lifeNumber = 5;
        fuel = 3000;
        movementState = normal;
    }

    void settings(Vector2f pos_, Vector2f v_, float angle_, float mass_, Texture &tex) {
        shape.setTexture(tex);
        shape.setTextureRect(IntRect(0, 0, 39, 36));
        shape.setOrigin(shape.getGlobalBounds().width / 2, shape.getGlobalBounds().height / 2);
        shape.setScale(0.55, 0.55);

        v = v_;
        pos = pos_;
        angle = angle_;
        mass = mass_;
        life = true;

        addElementToHitBox(Circle(Vector2f(-0.7, 1), pos, 3.7));
        addElementToHitBox(Circle(Vector2f(-0.7, -5.5), pos, 2.5));
        addElementToHitBox(Circle(Vector2f(-0.7, -9.3), pos, 0.6));
        addElementToHitBox(Circle(Vector2f(-5.3, 2), pos, 3.5));
        addElementToHitBox(Circle(Vector2f(-10, 4.5), pos, 0.7));
        addElementToHitBox(Circle(Vector2f(3.9, 2), pos, 3.5));
        addElementToHitBox(Circle(Vector2f(8.6, 4.5), pos, 0.7));
        addElementToHitBox(Circle(Vector2f(-0.7, 7.5), pos, 0.5));
    }

    void move() {
        float speed = sqrt(v.x * v.x + v.y * v.y);
        if(speed > maxSpeed) {
            v.x *= maxSpeed / speed;
            v.y *= maxSpeed / speed;
        }

        pos.x += v.x * timeSlowDown; pos.y += v.y * timeSlowDown;
        shape.setPosition(pos);
        shape.setRotation(angle + 90);

        for(int i = 0; i < things.size(); ++i) {
            things[i].setRotation(angle + 90);
            things[i].setPosition(pos);
        }
    }
    void direction() {
        if(pressed(Keyboard::Key::A)) {
            angle -= angleSpeed;
        }
        else if(pressed(Keyboard::Key::D)) {
            angle += angleSpeed;
        }

        if(pressed(Keyboard::Key::W) and fuel > 0) {
            v.x += thrust * cos(angle * DEGTORAD);
            v.y += thrust * sin(angle * DEGTORAD);
            fuel -= fuelUseSpeed;

            if(movementState == normal) {
                shape.setTexture(shipOn);
                shape.setTextureRect(IntRect(0, 0, 39, 43));
                movementState = accelerating;
            }
        }
        else {
            if(movementState == accelerating) {
                shape.setTexture(shipOff);
                shape.setTextureRect(IntRect(0, 0, 39, 36));
                movementState = normal;
            }
        }
    }

    void gravityInfluence(std::list<Planet*> &planets) {
        float Fx = 0, Fy = 0;
        for(auto p : planets) {
            float d = sqrt((p->pos.x - pos.x) * (p->pos.x - pos.x) + (p->pos.y - pos.y) * (p->pos.y - pos.y));
            if(d == 0) continue;
            float Sin = (p->pos.y - pos.y) / d;
            float Cos = (p->pos.x - pos.x) / d;
            float F = (mass * p->mass * GRAVITY) / (d * d);
            Fx += F * Cos; Fy += F * Sin;
        }
        force(Fx, Fy);
    }
    void force(float Fx, float Fy) {
        v.x += Fx; v.y += Fy;
    }

    void update(std::list<Planet*> &planets) {
        direction();
        gravityInfluence(planets);
        move();
    }

    void death() {
        reset();
        --lifeNumber;
    }

    void reset() {
        pos.x = W / 2;
        pos.y = H / 2;
        angle = 180;
        v.x = 0;
        v.y = 0;
    }
};
