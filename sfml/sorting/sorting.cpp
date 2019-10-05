#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <bits/stdc++.h>
#include "../universalfunctions/useful.h"
#include "SelectSort.h"
#include "BubbleSort.h"
using namespace sf;

const int N = 100;
const float W = 1200, H = 800;
const float A = W / N, B = H / N;

std::vector<RectangleShape> V;

void drawText(std::string napis, Font font, int charSize, float x, float y, RenderWindow &window) {
    Text ret(napis, font, charSize);
    ret.setOrigin(ret.getLocalBounds().width / 2, ret.getLocalBounds().height / 2);
    ret.setPosition(x, y);
    window.draw(ret);
}

void displayMenu(RenderWindow &window) {
    window.clear(Color::Black);

    Font bold, light, semibold;
    bold.loadFromFile("myfont_bold.ttf");
    light.loadFromFile("myfont_light.ttf");
    semibold.loadFromFile("myfont_semibold.ttf");

    drawText("Wybierz metode sortowania:", bold, 30, W / 2, 200, window);

    drawText("1.", semibold, 25, 300, 400, window);
    drawText("SelectSort", bold, 25, 500, 400, window);

    drawText("2.", semibold, 25, 300, 500, window);
    drawText("BubbleSort", bold, 25, 500, 500, window);

    window.display();
}

void generateArray() {
    for(int i = 0; i < N; ++i) {
        int high = randInteger(1, H);
        std::cout << high << std::endl;

        RectangleShape shape(Vector2f(A, static_cast<float>(high)));
        shape.setOrigin(0, high);
        shape.setPosition(i * A, H);
        shape.setFillColor(Color::White);

        V.push_back(shape);
    }
}

int main()
{
    RenderWindow window(VideoMode(W, H), "Types of Sorting");
    window.clear(Color::Black);
    displayMenu(window);
    generateArray();

    std::cout << "Press to vizualize:\n1: SelectSort\n2: BubbleSort";

    while(window.isOpen()) {
        Event e;

        while(window.pollEvent(e)) {
            switch(e.type) {
            case Event::Closed:
                window.close();
                break;
            case Event::TextEntered:
                char pressed = static_cast<char>(e.text.unicode);
                if(pressed >= '1' and pressed <= '9') {
                    if(pressed == '1') SelectSort(window, V);
                    if(pressed == '2') BubbleSort(window, V);
                    sleep(seconds(2));
                    displayMenu(window);
                }
            }
        }


    }

    return 0;
}
