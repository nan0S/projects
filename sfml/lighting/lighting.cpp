#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "common/useful.hpp"
using namespace sf;

const int W = 1.6 * 300, H = 300;
const float C = 100;
const float INF = 4 * 1e2;

class Object {
	public:
		Vector2f pos;
		Vector2f v;
		float R;

		Object() {
			pos = Vector2f(randInteger(0, W), randInteger(0, H));
			R = randFraction(20, 30);
			v = Vector2f(randFraction(-2, 2), randFraction(-2, 2));
		}

		void update() {
			if(pos.x + v.x > W or pos.x + v.x < 0) v.x *= -1;
			if(pos.y + v.y > H or pos.y + v.y < 0) v.y *= -1;
			pos += v;
		}
};

std::vector<Object> objects;

int main()
{
	std::srand(time(NULL));

	ContextSettings settings; settings.antialiasingLevel = 8;
	RenderWindow window(VideoMode(W, H), "Lightning", Style::Titlebar, settings);

	VertexArray vertex(Points, W * H);

	for(int x = 0; x < W; ++x) {
		for(int y = 0; y < H; ++y) {
			int index = x + y * W;
			vertex[index].position = Vector2f(x, y);
		}
	}

	int objectAmount = 5;
	for(int i = 0; i < objectAmount; ++i) {
		objects.push_back(Object());
	}

	while(window.isOpen()) {
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

		for(int x = 0; x < W; ++x) {
			for(int y = 0; y < H; ++y) {
				int index = x + y * W;
				float color = 0;

				for(std::size_t i = 0; i < objects.size(); ++i) {
					float d = dist(objects[i].pos, vertex[index].position);
					d = std::max(d, 0.001f);
					color += objects[i].R * C / d;
				}
				color = std::min(color, 255.f);
				vertex[index].color = Color(color, color, color);
			}
		}

		window.clear();
		window.draw(vertex);
		window.display();
	}

	return 0;
}
