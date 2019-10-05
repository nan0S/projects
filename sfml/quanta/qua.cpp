#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
#include "../universalfunctions/useful.h"
using namespace sf;

const int W = 1920, H = 700, d = 30;
const float maxSpeed = 0.8f, maxDist = 300.f, minDx = 5.5f, maxDx = 15.f;
const float spawnDist = 0.f;    
const int minSpawnCount = 1, maxSpawnCount = 5;

class Object
{
public:
    Vector2f v, mid;
    VertexArray shape;

    Object() 
    {
        Setup(Vector2f(randInteger(d, W - d), randInteger(d, H - d)));
    }

    Object(Vector2f pos)
    {
        Setup(pos);
    }

    void Setup(Vector2f pos)
    {
        GetNewVelocity();
        
        shape = VertexArray(Triangles, 3);
        shape[0].position = Vector2f(pos.x, pos.y + randFraction(minDx, maxDx));
        shape[1].position = Vector2f(pos.x + randFraction(minDx, maxDx), pos.y);
        shape[2].position = Vector2f(pos.x + randFraction(minDx, maxDx), pos.y + randFraction(minDx, maxDx));

        for (int i = 0; i < shape.getVertexCount(); ++i)
        {
            shape[i].color = Color(175, 175, 175);
            mid += shape[i].position;
        }

        mid.x /= 3.f;
        mid.y /= 3.f;
    }

    void GetNewVelocity()
    {
        v = Vector2f(randFraction(-maxSpeed, maxSpeed), randFraction(-maxSpeed, maxSpeed));
    }

    void Update()
    {
        for (int i = 0; i < shape.getVertexCount(); ++i)    
            shape[i].position += v;
        mid += v;

        if (mid.x < 0.f || mid.x > W || mid.y < 0.f || mid.y > H)
        {
            Vector2f newMid;

            if (rand() % 2)
            {
                if (rand() % 2)     
                    newMid = Vector2f(0.f, randFraction(0.f, H));
                else
                    newMid = Vector2f(W, randFraction(0.f, H));
            }
            else
            {
                if (rand() % 2)
                    newMid = Vector2f(randFraction(0.f, W), 0.f);
                else
                    newMid = Vector2f(randFraction(0.f, W), H);
            }

            Vector2f dir = newMid - mid;
            for (int i = 0; i < (int)shape.getVertexCount(); ++i)
                shape[i].position += dir;
            mid = newMid;
            GetNewVelocity();   
        }     
    }

    void Draw(RenderWindow &window)
    {
        window.draw(shape);
    }
};

class App
{
public:
    RenderWindow window;
    
    int objectCount;
    std::list <Object*> Objects;

    App()
    {
        ContextSettings settings;
        settings.antialiasingLevel = 16;
        window.create(VideoMode(W, H), "Quanta", Style::Default, settings);
        window.setFramerateLimit(60);

        objectCount = 30;

        for (int i = 0; i < objectCount; ++i)
            Objects.push_back(new Object());
    }

    void Run()
    {
        while (window.isOpen())
        {
            ProcessEvents();
            Update();
            Draw();
        }
    }

    void ProcessEvents()
    {
        Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case Event::Closed:
                    window.close();
                    break;
                case Event::MouseButtonPressed:
                    if (event.mouseButton.button == Mouse::Left)
                        Spawn();
            }
        }
    }

    void Update()
    {
        for (auto o : Objects)
            o -> Update();
    }

    void Spawn()
    {
        Vector2i temp = Mouse::getPosition(window);
        Vector2f pos = Vector2f(temp.x, temp.y);

        int count = randInteger(minSpawnCount, maxSpawnCount);
        objectCount += count;

        for (int i = 0; i < count; ++i)
            Objects.push_back(new Object(Vector2f(pos.x + randFraction(0.f, spawnDist), pos.y + randFraction(0.f, spawnDist))));
    }

    void Draw()
    {
        window.clear(Color::Black);

        VertexArray line = VertexArray(Lines, 2);
        line[0].color = line[1].color = Color::White;

        for (auto o1 : Objects)
            for (auto o2 : Objects)
            {
                float Dist = dist(o1 -> mid, o2 -> mid);

                if (Dist < maxDist)
                {
                    line[0].position = o1 -> mid;
                    line[1].position = o2 -> mid;
                        
                    float color = constrain(std::min(Dist, maxDist), 0.f, maxDist, 100.f, 0.f);
                    line[0].color = line[1].color = Color(color, color, color);

                    window.draw(line);
                }
            }    

        for (auto o : Objects)
            o -> Draw(window);

        window.display();
    }
};

int main()
{
    App app;
    app.Run();

    return 0;
}