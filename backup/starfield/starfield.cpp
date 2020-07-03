#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include <deque>
#include <math.h>
#include <sstream>
#include "../universalfunctions/useful.h"
using namespace sf;

const float W = 1200, H = 720;
const float radius = 5;

class Star 
{
public:

    float x, y, z;
    Vector2f pos, pastPosition;
    Color color;
    VertexArray line;

    Star () 
    {
        x = randFraction(-30 * W, 30 * W);
        y = randFraction(-30 * H, 30 * H);
        z = randFraction(0, 30 * W);

        color = Color::White;

        line.setPrimitiveType(Lines);
        line.resize(2);
    }

    bool OffScreen () 
    {
        return (pastPosition.x < -W or pastPosition.x > W or pastPosition.y < -H or pastPosition.y > H);
    }

    void Reset () 
    {
        x = randFraction(-30 * W, 30 * W);
        y = randFraction(-30 * H, 30 * H);
        z = randFraction(W, 30 * W);
    }

    void Update (float &flySpeed) 
    {
        z -= flySpeed;

        pos.x = constrain(x / z, -1, 1, -W, W);
        pos.y = constrain(y / z, -H / W, H / W, -H, H);

        pastPosition.x = constrain(x / (z + 30 * flySpeed), -1, 1, -W, W);
        pastPosition.y = constrain(y / (z + 30 * flySpeed), -H / W, H / W, -H, H);

        if((flySpeed > 0 and (z <= 0.f or OffScreen())) or (z > 30 * W and flySpeed < 0)) 
        {
            Reset();
            return;
        }

        line[0].position = pastPosition;
        line[1].position = pos;
        line[0].color = Color::Black;
        line[1].color = color;
    }

    void Draw (RenderWindow &window) 
    {
        window.draw(line);
    }
};

class App 
{
public:

    RenderWindow window;

    std::list<Star*> stars;

    Font boldFont;
    Text framesPerSecond;

    float flySpeed;
    float speedModificationSpeed;
    int starCount;

    Clock clock;
    int drawnFrameCount;

    App () 
    {
        starCount = 5000;

        ContextSettings settings; settings.antialiasingLevel = 16;
        window.create(VideoMode(W, H), "Starfield", Style::Default, settings);
        View view(Vector2f(0, 0), Vector2f(2 * W, 2 * H));
        window.setView(view);

        flySpeed = 5;
        speedModificationSpeed = 0.3;

        for(int i = 0; i < starCount; ++i) 
        {
            Star *s = new Star();
            stars.push_back(s);
        }

        boldFont.loadFromFile("../fonts/myfont_bold.ttf");

        framesPerSecond.setFont(boldFont);
        framesPerSecond.setCharacterSize(48);
        framesPerSecond.setPosition(-W + 30, -H + 15);

        drawnFrameCount = 0;
        clock.restart();
    }

    void ProcessEvents () 
    {
        Event event;
        while(window.pollEvent(event)) 
        {
            switch(event.type) 
            {
                case Event::Closed:
                    window.close();
                    break;
                case Event::KeyPressed:
                    if(event.key.code == Keyboard::Key::Up) 
                        flySpeed += speedModificationSpeed;
                    else if(event.key.code == Keyboard::Key::Down)
                        flySpeed -= speedModificationSpeed;
            }
        }
    }

    void Update () 
    {
        for(auto s : stars) 
            s -> Update(flySpeed);
    }

    void CountStarsOnScreen () 
    {
        int numberOfStarsOnScreen = 0;
        for(auto s : stars)
            if(!(s -> OffScreen()))
                ++numberOfStarsOnScreen;

        std::stringstream liczba; liczba << numberOfStarsOnScreen;
    }

    void UpdateFrameCounter() 
    {
        float frames = drawnFrameCount / clock.getElapsedTime().asSeconds();
        int intFrames = frames;

        std::stringstream liczba; liczba << intFrames;
        framesPerSecond.setString(liczba.str());

        drawnFrameCount = 0;
        clock.restart();
    }

    void DrawAll() 
    {
        window.clear(Color::Black);

        for(auto s : stars)
            s -> Draw(window);

        window.draw(framesPerSecond);

        ++drawnFrameCount;
        if(clock.getElapsedTime().asSeconds() > 0.1f)
            UpdateFrameCounter();

        window.display();
    }

    void Run () 
    {
        while(window.isOpen()) 
        {
            ProcessEvents();
            Update();
            CountStarsOnScreen();
            DrawAll();
        }
    }
};

int main()
{
    App app;
    app.Run();

    return 0;
}
