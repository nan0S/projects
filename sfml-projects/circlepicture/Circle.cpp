#include "Circle.hpp"

Circle::Circle() {growingSpeed = 0;}

Circle::Circle(Vector2f pos_, float growingSpeed_, float maxRadius_, Color color) {
	shape.setFillColor(color);
	pos = pos_;
	growingSpeed = growingSpeed_;
	R = 0;
	maxRadius = maxRadius_;
}

void Circle::isDead() {
	if(R >= maxRadius)
		growingSpeed = 0;
}

void Circle::update() {
	R += growingSpeed;
	isDead();

	shape.setRadius(R);
	shape.setOrigin(Vector2f(R, R));
	shape.setPosition(pos);
}

void Circle::drawTo(RenderWindow &window) {
	window.draw(shape);
}
