#include "SelectSort.hpp"

static void processEvents (RenderWindow &window)
{
	Event e;
	while (window.pollEvent(e))
		switch (e.type)
		{
			case Event::Closed:
				window.close();
				break;
			default:
				break;
		}
}

void SelectSort(RenderWindow &window, std::vector<RectangleShape> V)
{
	window.clear(Color::Black);
	for (auto& rect : V)
		window.draw(rect);
	window.display();

	for (size_t i = 0; i < V.size(); ++i)
	{
		auto& rect1 = V[i];
		rect1.setFillColor(Color::Green);

		float highI = rect1.getLocalBounds().height;
		for (size_t j = i + 1; j < V.size(); ++j)
		{
			auto& rect2 = V[j];
			rect2.setFillColor(Color::Red);

			float highJ = rect2.getLocalBounds().height;
			if (highJ < highI)
			{
				Vector2f posI = rect1.getPosition(), posJ = rect2.getPosition();
				rect2.setPosition(posI); rect1.setPosition(posJ);
				rect2.setFillColor(Color::Green); rect1.setFillColor(Color::Red);
				std::swap(rect1, rect2);
				highI = highJ;
			}

			processEvents(window);

			window.clear(Color::Black);
			for (size_t k = 0; k < V.size(); ++k)
				window.draw(V[k]);
			window.display();

			sleep(seconds(0.005));

			rect2.setFillColor(Color::White);
		}
		rect1.setFillColor(Color::White);
	}
}

