#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <math.h>
#include "common/useful.hpp"
using namespace sf;

const float W = 500, H = 300;
const float C = 100;

class Circle {
	public:
		Vector2f pos;
		Vector2f v;
		float R;

		Circle() {
			pos = Vector2f(randInteger(0, W), randInteger(0, H));
			v = Vector2f(randFraction(-3, 3), randFraction(-3, 3));
			R = randFraction(20.f, 25.f);
		}

		void update() {
			Vector2f newPos = pos + v;
			if(newPos.x > W or newPos.x < 0) v.x *= -1;
			if(newPos.y > H or newPos.y < 0) v.y *= -1;

			pos += v;
		}
};


std::vector<Circle> objects;

int main()
{
	std::srand(time(NULL));

	RenderWindow window(VideoMode(W, H), "SFML!", Style::Default);

	int objectAmount = 5;
	for(int i = 0; i < objectAmount; ++i) {
		objects.push_back(Circle());
	}

	Image image;
	image.create(W, H);

	while (window.isOpen())
	{
		Event e;
		while(window.pollEvent(e)) {
			switch(e.type) {
				case Event::Closed:
					window.close();
				default:
					break;
			}
		}

		for(std::size_t i = 0; i < objects.size(); ++i) {
			objects[i].update();
		}

		for(std::size_t x = 0; x < image.getSize().x; ++x) {
			for(std::size_t y = 0; y < image.getSize().y; ++y) {
				float color = 0.f;
				for(std::size_t i = 0; i < objects.size(); ++i) {
					float d = dist(objects[i].pos, Vector2f(x, y));
					d = std::max(d, 0.f);
					color += objects[i].R * C / d;
				}
				color = std::min(color, 255.f);
				Color kiczak = hsv(color, color, color);
				image.setPixel(x, y, kiczak);
			}
		}

		Texture texture; texture.loadFromImage(image);
		Sprite sprite(texture);
		sprite.setPosition(Vector2f(0, 0));

		window.clear();
		window.draw(sprite);
		window.display();
	}

	return 0;
}
