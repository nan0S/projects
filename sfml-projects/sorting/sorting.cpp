#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <bits/stdc++.h>
#include "common/useful.hpp"
#include "SelectSort.hpp"
#include "BubbleSort.hpp"
using namespace sf;

const int N = 100;
const float W = 1200, H = 800;
const float A = W / N, B = H / N;

std::vector<RectangleShape> V;
Font bold, semibold;

void init()
{
	bold.loadFromFile("fonts/font_bold.ttf");
	semibold.loadFromFile("fonts/font_semibold.ttf");
}

void drawText(const std::string& napis, const Font& font,
		const int& charSize, 
		const float& x, const float& y, 
		RenderWindow &window)
{
	Text ret(napis, font, charSize);
	ret.setOrigin(ret.getLocalBounds().width / 2, ret.getLocalBounds().height / 2);
	ret.setPosition(x, y);
	window.draw(ret);
}

void displayMenu(RenderWindow &window)
{
	window.clear(Color::Black);

	drawText("Choose sort method:", bold, 30, W / 2, 200, window);

	drawText("1.", semibold, 25, 300, 400, window);
	drawText("SelectSort", bold, 25, 500, 400, window);

	drawText("2.", semibold, 25, 300, 500, window);
	drawText("BubbleSort", bold, 25, 500, 500, window);
	
	drawText("Press Q to quit", bold, 15, 110, H - 40, window);

	window.display();
}

void generateArray()
{
	for(int i = 0; i < N; ++i)
	{
		int high = randInteger(1, H);

		RectangleShape shape(Vector2f(A, static_cast<float>(high)));
		shape.setOrigin(0, high);
		shape.setPosition(i * A, H);
		shape.setFillColor(Color::White);

		V.push_back(shape);
	}
}

int main()
{
	RenderWindow window(VideoMode(W, H), "Types of Sorting", Style::Titlebar);
	window.clear(Color::Black);

	init();
	displayMenu(window);
	generateArray();

	while (window.isOpen())
	{
		Event e;
		while (window.pollEvent(e))
			switch (e.type)
			{
				case Event::Closed:
					window.close();
					break;
				case Event::TextEntered:
				{
					char pressed = static_cast<char>(e.text.unicode);
					if (pressed >= '1' and pressed <= '9')
					{
						if (pressed == '1') SelectSort(window, V);
						if (pressed == '2') BubbleSort(window, V);
						sleep(seconds(0.5));
						displayMenu(window);
					}
					break;
				}
				default:
					break;
			}
		if (pressed(sf::Keyboard::Q))
			break;
	}

	return 0;
}
