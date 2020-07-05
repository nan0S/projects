#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <list>
#include "common/useful.hpp"
using namespace sf;

const int W = 1200, H = 720;

class TrailElement
{
public:
    CircleShape shape;
    Vector2f pos;
    
    Clock clock;   
    float timeBetweenFades;

    TrailElement () {}

    TrailElement (Vector2f pos, float R) : pos(pos)
    {
        shape.setRadius(R);
        shape.setOrigin(Vector2f(R / 2, R / 2));
        shape.setPosition(pos);

        timeBetweenFades = 0.0025f;
    }

    void Update ()
    {
        Color color = shape.getFillColor();
        if (clock.getElapsedTime().asSeconds() >= timeBetweenFades)
        {
            color.a -= 1;
            shape.setFillColor(Color(color.r, color.g, color.b, color.a));
            clock.restart();
        }
    }

    bool IsDead ()
    {
        return shape.getFillColor().a <= 0;
    }

    void Render (RenderWindow &window)
    {   
        window.draw(shape);
    }
};

class Player
{
public:
    float R;
    float maxSpeed;
    float acceleration;

    Vector2f pos, v;
    CircleShape shape;

    Clock clock;

    std::list <TrailElement*> Trail;
    float distanceBetweenTrails;

    float collisionFriction;

    Player ()
    {
        R = 5.f;
        acceleration = 0.01f;
        maxSpeed = 2.f;
        pos = Vector2f(W / 2, H / 2);

        shape.setRadius(R);
        shape.setOrigin(Vector2f(R / 2, R / 2));
        shape.setPosition(pos);

        v = Vector2f(0.f, 0.f);
        distanceBetweenTrails = 1.f;
        collisionFriction = 0.6f;
    }

    void Update ()
    {
        CheckDeaths();
        GetInput();
        Move();

        if (Trail.empty() || dist(pos, Trail.back() -> pos) >= distanceBetweenTrails)
        {
            AddTrailElement();
            clock.restart();
        }

        for (auto t : Trail)
            t -> Update();
    }

    void GetInput ()
    {
        if (Keyboard::isKeyPressed(Keyboard::Key::A))
            v.x -= acceleration;
        if (Keyboard::isKeyPressed(Keyboard::Key::D))
            v.x += acceleration;
        if (Keyboard::isKeyPressed(Keyboard::Key::W))
            v.y -= acceleration;
        if (Keyboard::isKeyPressed(Keyboard::Key::S))
            v.y += acceleration;
    }

    void Move ()
    {
        float magnitude = sqrt(v.x * v.x + v.y * v.y);
        if (magnitude > maxSpeed)
            v *= maxSpeed / magnitude;
        v *= 0.9999f;

        pos += v;
        if (pos.x < 0.f)
        {
            pos.x = 0.f;
            v.x *= -collisionFriction;
        }
        else if (pos.x > W)
        {
            pos.x = W;
            v.x *= -collisionFriction;
        }
        if (pos.y < 0.f)
        {
            pos.y = 0.f;
            v.y *= -collisionFriction;
        }
        else if (pos.y > H)
        {
            pos.y = H;
            v.y *= -collisionFriction;
        }

        shape.setPosition(pos);
    }

    void AddTrailElement ()
    {
        Trail.push_back(new TrailElement(pos, R / 2));
    }

    void CheckDeaths ()
    {
        for (auto it = Trail.begin(); it != Trail.end(); )
        {
            TrailElement *t = *it;
            if (t -> IsDead())
            {
                it = Trail.erase(it);
                delete t;
            }
            else
                it++;
        }
    }

    void Render (RenderWindow &window)
    {
        for (auto t : Trail)
            t -> Render(window);
        window.draw(shape);
    }
};

class App 
{
public:
    RenderWindow window;
    Player player;

    App ()
    {
        ContextSettings settings;
        settings.antialiasingLevel = 8;
        window.create(VideoMode(W, H), "Trail", Style::Titlebar, settings);
	   PrintHelp();
    }

    void PrintHelp()
    {
	    std::cout << "Move around with WASD keys and leave trail behind you!" << std::endl;
    }

    void Run ()
    {
        while (window.isOpen())
        {
            ProcessEvents();
            Update();
            Render();
        }
    }

    void ProcessEvents ()
    {
        Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case Event::Closed:
                    window.close();
                    break;
			default:
				break;
            }
        }
    }

    void Update ()
    {
        player.Update();
    }

    void Render ()
    {
        window.clear(Color::Black);
        player.Render(window);
        window.display();
    }
};

int main()
{
    App app;
    app.Run();

    return 0;
}
