#include "Fundament.hpp"

Circle::Circle(Vector2f relativePos_, Vector2f originPos_, float R_) {
	relativePos = relativePos_;
	originPos = originPos_;
	baseAngle = atan2(relativePos.y, relativePos.x) * 180 / PI;
	R = R_;
	d = dist(originPos, originPos + relativePos);
}

void Circle::setRotation(float angle) {
	relativePos.x = d * cos((angle + baseAngle) * DEGTORAD);
	relativePos.y = d * sin((angle + baseAngle) * DEGTORAD);
}
void Circle::setPosition(Vector2f p) {
	originPos = p;
}

Vector2f Circle::getPos() {
	return originPos + relativePos;
}

void Circle::drawTo(RenderWindow *window) {
	CircleShape shape(R);
	shape.setOrigin(Vector2f(R - 0.5, R - 0.5));
	shape.setFillColor(Color::Transparent);
	shape.setOutlineThickness(0.5);
	shape.setOutlineColor(Color::Yellow);
	shape.setPosition(getPos());
	window->draw(shape);
}

Hitbox::HitBox() {}

void Hitbox::addElementToHitBox(Circle c) {
	things.push_back(c);
}

bool Hitbox::isColliding(HitBox *h, float inacuraccy) {
	for(auto c1 : things) {
		for(auto c2 : h->things) {
			if(dist(c1.getPos(), c2.getPos()) < c1.R + c2.R + inacuraccy) {
				return true;
			}
		}
	}
	return false;
}

void Hitbox::drawHitBox(RenderWindow *window) {
	for(auto t : things) {
		t.drawTo(window);
	}
}

RectangleFundament::RectangleFundament() {life = true;}

Vector2f RectangleFundament::getPosition() {
	return pos;
}

void RectangleFundament::drawTo(RenderWindow *window) {
	window->draw(shape);
}

CircleFundament::CircleFundament() {life = true;}

Vector2f CircleFundament::getPosition() {
	return pos;
}

void CircleFundament::drawTo(RenderWindow *window) {
	window->draw(shape);
}
