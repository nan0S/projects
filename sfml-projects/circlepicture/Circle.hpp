#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
using namespace sf;

class Circle {
	public:
		CircleShape shape;
		Vector2f pos;
		float growingSpeed;
		float R, maxRadius;

		Circle();
		Circle(Vector2f pos_, float growingSpeed_, float maxRadius_, Color color);

		void isDead();
		void update();
		void drawTo(RenderWindow &window);
};
