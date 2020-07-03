#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include <deque>
#include <math.h>
#include <sstream>
#include "../universalfunctions/useful.h"
using namespace sf;

const int W = 1200, H = 720;
const float radius = 5;

class App {
public:
    RenderWindow window;

    float flySpeed;
    float speedModificationSpeed;
    int starCount;

    VertexArray line;
    CircleShape shape;

    Font boldFont;
    Text starsOnScreen;

    Text framesPerSecond;
    Clock clock;
    int drawnFrameCount;
    int minFrames, maxFrames;

    class Star {
    public:
        float x, y, z;
        Vector2f pos;
        // CircleShape shape;

        Color color;

        Vector2f pastPosition;
        // VertexArray line;

        Star() {
            x = randFraction(-10 * W, 10 * W);
            y = randFraction(-10 * H, 10 * H);
            z = randFraction(0, 10 * W);

            // color = Color(randInteger(0, 255), randInteger(0, 255), randInteger(0, 255));
            // color = Color::White;
            // shape.setFillColor(color);

            // line.setPrimitiveType(Lines);
            // line.resize(2);
        }

        bool offScreen() {
            return (pos.x < -W or pos.x > W or pos.y < -H or pos.y > H);
        }

        void reset() {
            x = randFraction(-10 * W, 10 * W);
            y = randFraction(-10 * H, 10 * H);
            z = randFraction(8 * W, 10 * W);
        }

        void update(float &flySpeed, CircleShape &shape, VertexArray &line) {
            z -= flySpeed;

            if((flySpeed > 0 and offScreen()) or (z > 6 * W and flySpeed < 0)) {
                reset();
            }

            pos.x = constrain(x / z, -1, 1, -W, W);
            pos.y = constrain(y / z, -1, 1, -H, H);

            pastPosition.x = constrain(x / (z + 30 * flySpeed), -1, 1, -W, W);
            pastPosition.y = constrain(y / (z + 30 * flySpeed), -1, 1, -H, H);

            line[0].position = pastPosition;
            line[1].position = pos;
            // line[0].color = Color::Black;
            // line[1].color = color;

            float newRadius = radius / z * 650;
            shape.setRadius(newRadius);
            shape.setOrigin(newRadius, newRadius);
            shape.setPosition(pos);
        }

        void draw(RenderWindow &window, CircleShape &shape, VertexArray &line) {
            window.draw(line);
            window.draw(shape);
        }
    };

    std::list<Star*> stars;

    App() {
        std::cout << "Podaj liczbe gwiazd: ";
        // std::cin >> starCount;
        starCount = 500;
        ContextSettings settings; settings.antialiasingLevel = 16;
        window.create(VideoMode(W, H), "Starfield", Style::Default, settings);
        View view(Vector2f(0, 0), Vector2f(2 * W, 2 * H));
        window.setView(view);

        flySpeed = 1;
        speedModificationSpeed = 0.3;

        for(int i = 0; i < starCount; ++i) {
            Star *s = new Star();
            stars.push_back(s);
        }

        boldFont.loadFromFile("../fonts/myfont_bold.ttf");

        starsOnScreen.setFont(boldFont);
        starsOnScreen.setCharacterSize(24);
        starsOnScreen.setPosition(-W + 30, H - 50);

        line.setPrimitiveType(Lines);
        line.resize(2);
        line[0].color = Color::Black;
        line[1].color = Color::White;

        shape.setFillColor(Color::White);

        framesPerSecond.setFont(boldFont);
        framesPerSecond.setCharacterSize(48);
        framesPerSecond.setPosition(-W + 30, -H + 15);

        drawnFrameCount = 0;
        minFrames = 1e6; maxFrames = -1;
        clock.restart();
    }

    void processEvents() {
        Event event;
        while(window.pollEvent(event)) {
            switch(event.type) {
                case Event::Closed:
                    window.close();
                    std::cout << "minFPS: " << minFrames << "\n" << "maxFPS: " << maxFrames << "\n";
                    break;
                case Event::KeyPressed:
                    if(event.key.code == Keyboard::Key::Up) {
                        flySpeed += speedModificationSpeed;
                    }
                    else if(event.key.code == Keyboard::Key::Down) {
                        flySpeed -= speedModificationSpeed;
                    }
            }
        }
    }

    void update() {
        for(auto s : stars) {
            s -> update(flySpeed, shape, line);
        }
    }

    void countStarsOnScreen() {
        int numberOfStarsOnScreen = 0;
        for(auto s : stars) {
            if(!(s -> offScreen())) {
                ++numberOfStarsOnScreen;
            }
        }

        std::stringstream liczba; liczba << numberOfStarsOnScreen;
        starsOnScreen.setString(liczba.str());
    }

    void updateFrameCounter() {
        float frames = drawnFrameCount / clock.getElapsedTime().asSeconds();
        int intFrames = frames;

        std::stringstream liczba; liczba << intFrames;
        framesPerSecond.setString(liczba.str());

        minFrames = std::min(minFrames, intFrames);
        maxFrames = std::max(maxFrames, intFrames);

        drawnFrameCount = 0;
        clock.restart();
    }

    void drawAll() {
        window.clear(Color::Black);

        for(auto s : stars) {
            s -> update(flySpeed, shape, line);
            s -> draw(window, shape, line);
        }
        window.draw(starsOnScreen);
        window.draw(framesPerSecond);

        ++drawnFrameCount;
        if(clock.getElapsedTime().asSeconds() > 0.1) {
            updateFrameCounter();
        }

        window.display();
    }

    void run() {
        while(window.isOpen()) {
            processEvents();
            // update();
            countStarsOnScreen();
            drawAll();
        }
    }
};

int main()
{
    App app;
    app.run();

    return 0;
}
