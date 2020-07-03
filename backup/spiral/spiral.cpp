#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

const int W = 1920, H = 1080;
const float DEGTORAD = (M_PI / 180.f);

int main ()
{
    RenderWindow window(VideoMode(W, H), "Title");
    window.clear(Color::Black);
    window.setFramerateLimit(240);

    int n = 0;
    float c = 10.f;

    vector < CircleShape > Circles;

    while (window.isOpen())
    {

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        float angle = n * 137.5f * DEGTORAD;
        float d = c * sqrt(n);
        ++n;

        CircleShape circle(4.f, 50);
        circle.setFillColor(Color::White);

        Vector2f pos(d * sin(angle) + W / 2, d * cos(angle) + H / 2);
        circle.setPosition(pos);

        Circles.push_back(circle);

        window.clear(Color::Black); 
        for (auto c : Circles)
            window.draw(c);
        window.display();
    }

    return 0;
}