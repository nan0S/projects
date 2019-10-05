#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include "../universalfunctions/useful.h"
using namespace sf;

const float W = 1200, H = 720;
const float radius = 5;

class Star {
public:
    float x, y, z;
    Vector2f pos, pastPos;

    CircleShape shape;
    VertexArray line;

    Star() {
        reset();

        line.setPrimitiveType(Lines);
        line.resize(2);
        line[0].color = Color::Black; line[1].color = Color::White;

        shape.setFillColor(Color::White);
    }

    void reset() {
        x = randFraction(-W, W);
        y = randFraction(-H, H);
        z = randFraction(0, W);
    }


    bool offScreen() {
        return (pastPos.x < -W or pastPos.x > W or pastPos.y < -H or pastPos.y > H);
    }

    void update() {
        --z;

        pos.x = constrain(x / z, -1, 1, -W, W);
        pos.y = constrain(y / z, -H / W, H / W, -H, H);
        pastPos.x = constrain(x / (z + 15), -1, 1, -W, W);
        pastPos.y = constrain(y / (z + 15), -H / W, H / W, -H, H);

        if(offScreen()) {
            reset();
            return;
        }

        float R = radius / z * 150.f;
        shape.setRadius(R);
        shape.setOrigin(R, R);
        shape.setPosition(pos);

        line[0].position = pastPos;
        line[1].position = pos;
    }

    void draw(RenderWindow &window) {
        window.draw(line);
        // window.draw(shape);
    }
};

int main()
{
    RenderWindow window(VideoMode(W, H), "Starfield", Style::Default);
    View view(Vector2f(0, 0), Vector2f(2 * W, 2 * H));
    window.setView(view);

    std::list<Star*> stars;
    for(int i = 0; i < 500; ++i) {
        Star *s = new Star();
        stars.push_back(s);
    }

    while(window.isOpen()) {
        Event event;
        while(window.pollEvent(event)) {
            switch(event.type) {
                case Event::Closed:
                    window.close();
                    break;
            }
        }

        window.clear(Color::Black);
        for(auto s : stars) {
            s -> update();
            s -> draw(window);
        }
        window.display();
    }

    return 0;
}
