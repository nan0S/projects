#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <bits/stdc++.h>
using namespace sf;

namespace slct
{
    void processEvents (RenderWindow &window)
    {
        Event e;
        while(window.pollEvent(e)) {
                switch(e.type) {
                case Event::Closed:
                    window.close();
                    break;
            }
        }
    }
}

void SelectSort(RenderWindow &window, std::vector<RectangleShape> V) {
    window.clear(Color::Black);
    for(int i = 0; i < V.size(); ++i) window.draw(V[i]);
    window.display();

    for(int i = 0; i < V.size(); ++i) {
        V[i].setFillColor(Color::Green);
        float highI = V[i].getLocalBounds().height;
        for(int j = i + 1; j < V.size(); ++j) {
            V[j].setFillColor(Color::Red);
            float highJ = V[j].getLocalBounds().height;
            if(highJ < highI) {
                Vector2f posI = V[i].getPosition(), posJ = V[j].getPosition();
                V[j].setPosition(posI); V[i].setPosition(posJ);
                V[j].setFillColor(Color::Green); V[i].setFillColor(Color::Red);
                std::swap(V[i], V[j]);
                highI = highJ;
            }

            slct::processEvents(window);

            window.clear(Color::Black);
            for(int k = 0; k < V.size(); ++k) window.draw(V[k]);
            window.display();

            sleep(seconds(0.005));

            V[j].setFillColor(Color::White);
        }
        V[i].setFillColor(Color::White);
    }
}

