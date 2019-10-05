#include <SFML/Graphics.hpp>
#include <iostream>

class Camera {
public:
    Vector2f pos;
    Vector2f range;
    Vector2f v;

    Camera() {pos.x = W / 2; pos.y = H / 2; v.x = 0; v.y = 0;}

    void settings(Vector2f pos_, Vector2f range_) {
        pos = pos_;
        range = range_;
    }

    void move(Player &player) {
        Vector2f des = player.getPosition();
        float d = dist(des, pos);
        float F = d * d / 830000;
        if(d == 0) return;
        float Sin = (des.y - pos.y) / d;
        float Cos = (des.x - pos.x) / d;
        force(F * Cos, F * Sin);

        v.x *= 0.965;
        v.y *= 0.965;

        pos.x += v.x;
        pos.y += v.y;
    }
    void force(float Fx, float Fy) {
        v.x += Fx;
        v.y += Fy;
    }

    void applyCameraTo(RenderWindow *window) {
        View view(pos, range);
        window->setView(view);
    }

    void update(Player &player, RenderWindow *window) {
        move(player);
        applyCameraTo(window);
    }

    Vector2f getPosition() {
        return pos;
    }

    Vector2f getRange() {
        return range;
    }
};
