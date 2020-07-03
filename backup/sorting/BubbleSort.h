#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<bits/stdc++.h>
using namespace sf;

namespace bble
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

void BubbleSort(RenderWindow &window, std::vector<RectangleShape> V) {
    window.clear(Color::Black);
    for(int i = 0; i < V.size(); ++i) window.draw(V[i]);
    window.display();

    bool change = true;
    int ile = 0;
    while(change) {
        change = false;
        ++ile;
        for(int i = 0; i < V.size() - ile; ++i) {
            V[i].setFillColor(Color::Green);
            V[i + 1].setFillColor(Color::Red);

            int highI = V[i].getLocalBounds().height;
            int highJ = V[i + 1].getLocalBounds().height;
            if(highI > highJ) {
                Vector2f posI = V[i].getPosition(), posJ = V[i + 1].getPosition();
                V[i].setPosition(posJ); V[i + 1].setPosition(posI);
                V[i].setFillColor(Color::Red); V[i + 1].setFillColor(Color::Green);
                std::swap(V[i], V[i + 1]);
                change = true;
            }

            bble::processEvents(window);

            window.clear(Color::Black);
            for(int j = 0; j < V.size(); ++j) window.draw(V[j]);
            window.display();

            sleep(seconds(0.005));

            V[i].setFillColor(Color::White);
            if(i == V.size() - ile - 1) V[i + 1].setFillColor(Color::White);
        }
    }
}
