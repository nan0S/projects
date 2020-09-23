#include "Planet.hpp"
#include "Const.hpp"

Planet::Planet() {}

Planet::Planet(Vector2f pos_, float R_, int number_) {
	R = R_;
	float outlineThick = std::min(R / 13, 4.f);
	pos = pos_;

	shape.setRadius(R - outlineThick);
	shape.setOrigin(R, R);
	shape.setPosition(pos);
	shape.setFillColor(Color::Black);
	shape.setOutlineThickness(outlineThick);
	shape.setOutlineColor(Color::Green);

	mass = R * ro;
	life = true;
	number = number_;

	addElementToHitBox(Circle(Vector2f(0, 0), pos, R));
}
