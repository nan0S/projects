#include "Enemy.hpp"

Enemy::Enemy() {life = true;}

void Enemy::settings(Vector2f pos_, float bodyAngle_, float shootingSpeed_, int number_) {
	pos = pos_;
	bodyAngle = bodyAngle_;
	gunAngle = bodyAngle;
	shootingSpeed = shootingSpeed_;
	life = true;
	time = 0;
	number = number_;

	body.setSize(dim);
	body.setOrigin(Vector2f(dim.x / 2, dim.y / 2));
	body.setPosition(pos);
	body.setRotation(bodyAngle);
	body.setFillColor(Color::Black);
	body.setOutlineThickness(1.3);
	body.setOutlineColor(Color::Red);

	gun.setSize(Vector2f(5, 18));
	gun.setOrigin(2.5, 18);
	gun.setPosition(pos);
	gun.setRotation(bodyAngle);
	gun.setFillColor(Color::Black);
	gun.setOutlineThickness(1.3);
	gun.setOutlineColor(Color::Red);

	epsilon = -randFraction(0.1, 0.5);

	for(int i = -1; i <= 1; ++i) {
		addElementToHitBox(Circle(Vector2f(i * 10, 0), pos, 5));
		things.back().setPosition(pos);
		things.back().setRotation(bodyAngle);
	}
	addElementToHitBox(Circle(Vector2f(0, -2.5), pos, 2.5));
	addElementToHitBox(Circle(Vector2f(0, -9), pos, 2.5));
	addElementToHitBox(Circle(Vector2f(0, -18 + 2.5), pos, 2.5));
}

void Enemy::update(std::list<Bullet*> &bullets) {
	gunAngle += epsilon;
	gun.setRotation(gunAngle);
	if(gunAngle - bodyAngle >= 60 or gunAngle - bodyAngle <= -60) epsilon = -epsilon;

	for(std::size_t i = things.size() - 3; i < things.size(); ++i) {
		things[i].setRotation(gunAngle);
	}

	++time;
	if(time >= shootingSpeed) {
		time = 0;
		Bullet *b = new Bullet;
		b->settings(pos,
				Vector2f(nonFriendlyMissileSpeed * cos((270 + gunAngle) * DEGTORAD), nonFriendlyMissileSpeed * sin((270 + gunAngle) * DEGTORAD)),
				270 + gunAngle,
				number,
				enemyMissile, "nonfriendly");
		bullets.push_back(b);
	}
}

Vector2f Enemy::getPosition() {
	return pos;
}

void Enemy::drawTo(RenderWindow *window) {
	window->draw(body);
	window->draw(gun);
}
