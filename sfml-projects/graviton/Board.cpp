#include "Board.hpp"

Board::Board() {
	enemyCount = killedEnemies = 0;
	minGunSpeed = 300; maxGunSpeed = 1000;
	planetCount = 4;
	fuelCount = 4;
	time = 0;

	border.setPrimitiveType(LinesStrip); border.resize(5);
	for(size_t i = 0; i < border.getVertexCount(); ++i) {
		border[i].color = Color::Red;
	}
	top = 0; bottom = H; left = 0; right = W;
	player.settings(Vector2f(W / 2, H / 2), Vector2f(0, 0), 180, 100, shipOff);

	camera.settings(Vector2f(player.pos.x, player.pos.y), Vector2f(W, H));
}

void Board:: Board::setup() {
	generateRandom();
}

void Board::generateRandom() {
	border[0].position = Vector2f(left, top);
	border[1].position = Vector2f(right, top);
	border[2].position = Vector2f(right, bottom);
	border[3].position = Vector2f(left, bottom);
	border[4].position = Vector2f(left, top);

	int timesTried = 0;

	for(int i = 0; i < planetCount; ++i) {
		float R = randFraction(30, 120);
		Vector2f pos(randFraction(left + 2 * R, right - 2 * R), randFraction(top + 2 * R, bottom - 2 * R));

		Planet *candidatePlanet = new Planet(pos, R, i + 1);

		if(canAddNewPlanet(candidatePlanet)) {
			timesTried = 0;

			addPlanet(candidatePlanet);

			int enemyCount = randInteger(0, 2);
			for(int j = 0; j < enemyCount; ++j) {
				float x = randFraction(-R, R);
				float y = sqrt(R * R - x * x);
				if(rand() % 2) y = -y;
				float angle = atan2(y, x) * 180 / PI; angle += 90;

				Enemy *enemy = new Enemy;
				enemy -> settings(Vector2f(pos.x + x, pos.y + y), angle, randInteger(minGunSpeed, maxGunSpeed), i + 1);

				if(canAddNewEnemy(enemy)) {
					addEnemy(enemy);
				}
				else {
					--j;
					delete enemy;
				}
			}
		}
		else {
			delete candidatePlanet;
			if(timesTried < 100) {
				i--;
				++timesTried;
			}
			else
				timesTried = 0;
		}
	}

	for(int i = 0; i < fuelCount; ++i) {
		Vector2f pos(randFraction(left, right), randFraction(top, bottom));
		Collectable *fuel;
		fuel = new Collectable(pos, fuelTexture, randInteger(200, 500), FUEL);

		if(canAddNewCollectable(fuel))
			addCollectable(fuel);
		else
			i--;
	}
}

bool Board::canAddNewPlanet(Planet *planet) {
	if(isOffScreen(planet -> pos, planet -> R)) return false;
	if(dist(planet -> pos, player.pos) < (planet -> R) * 3) return false;

	for(auto p : planets)
		if(p -> isColliding(planet, 50))
			return false;

	return true;
}
bool Board::canAddNewEnemy(Enemy *enemy) {
	for(auto e : enemies)
		if(e -> isColliding(enemy, 10))
			return false;

	return true;
}
bool Board::canAddNewCollectable(Collectable *collectable) {
	for(auto p : planets) {
		if(p -> isColliding(collectable, (p -> R)))
			return false;
	}

	for(auto c : collectables) {
		if(dist(c -> pos, collectable -> pos) < 200)
			return false;
	}

	return true;
}

void Board::addPlanet(Planet *p) {
	planets.push_back(p);
}
void Board::addEnemy(Enemy *e) {
	enemies.push_back(e);
	++enemyCount;
}
void Board::addBullet(Bullet *b) {
	bullets.push_back(b);
}
void Board::addSound(Vector2f pos) {
	float distance = dist(pos, player.getPosition());
	Sound *sound = new Sound;
	sound -> setBuffer(buffer);
	sound -> setVolume(7500 / distance);
	sound -> play();
	sounds.push_back(sound);
}
void Board::addAnimation(Animation *a) {
	animations.push_back(a);
}
void Board::createAnimation(Vector2f pos, std::string name) {
	// addSound(pos);

	if(name == "missileExplosion") {
		missileExplosion.setPosition(pos);
		Animation *a = new Animation(missileExplosion);
		addAnimation(a);
	}
	else if(name == "machineExplosion") {
		machineExplosion.setPosition(pos);
		Animation *a = new Animation(machineExplosion);
		addAnimation(a);
	}
}
void Board::addCollectable(Collectable *c) {
	collectables.push_back(c);
}

bool Board::offScreen(HitBox *a) {
	for(auto t : a->things) {
		Vector2f pos = t.getPos();
		if(isOffScreen(pos, 0))
			return true;
	}
	return false;
}
bool Board::isOffScreen(Vector2f pos, float inacuraccy) {
	return (pos.x - inacuraccy < left or pos.x + inacuraccy > right or pos.y - inacuraccy < top or pos.y + inacuraccy > bottom);
}
void Board::checkOffScreen() {
	if(offScreen(&player)) {
		gameOver();
		return;
	}

	for(auto b : bullets) {
		if(offScreen(b)) {
			b->life = false;
			createAnimation(b->getPosition(), "missileExplosion");
		}
	}
}
void Board::checkCollisions() {
	for(auto p : planets) { //gracz z planetami
		if(player.isColliding(p, 0)) {
			gameOver();
			return;
		}
	}

	for(auto e : enemies) { //gracz z przeciwnikami
		if(player.isColliding(e, 0)) {
			gameOver();
			return;
		}
	}

	for(auto b : bullets) { //gracz z pociskami
		if(b->life == false) continue;
		if(b->name != "friendly" and player.isColliding(b, 0)) {
			gameOver();
			return;
		}
	}

	for(auto c : collectables) { //gracz z rzeczami do zbierania
		if(player.isColliding(c, 0)) {
			c -> applyAttributeTo(player);
			c -> life = false;
		}
	}

	for(auto b : bullets) { //pociski z przeciwnikami
		if(b -> life == false) continue;
		for(auto e : enemies) {
			if(b->name == "nonfriendly" or b -> life == false) continue;
			if(b->isColliding(e, 0)) {
				b->life = false;
				e->life = false;
				++killedEnemies;
				time = 0;

				createAnimation(e -> getPosition(), "machineExplosion");
			}
		}
	}

	for(auto a : bullets) { //pocisk z pociskiem
		if(a -> life == false) continue;
		for(auto b : bullets) {
			if(a->name == "friendly" and b->name == "nonfriendly" and b -> life != false) {
				if(a->isColliding(b, 0)) {
					a->life = false;
					b->life = false;

					Vector2f pos = a -> getPosition();
					pos += Vector2f((a -> v.x) * 5, (a -> v.y) * 5);
					createAnimation(pos, "missileExplosion");
				}
			}
		}
	}

	for(auto p : planets) { // pociski z planetami
		for(auto b : bullets) {
			if(p->number == b->number or b->life == false) continue;
			if(p->isColliding(b, 0)) {
				b->life = false;

				Vector2f pos = b -> getPosition();
				pos += Vector2f((b -> v.x) * 5, (b -> v.y) * 5);
				createAnimation(pos, "missileExplosion");
			}
		}
	}
}
void Board::removeDead(bool clearAll) {
	std::list<Bullet*>::iterator it;
	for(it = bullets.begin(); it != bullets.end();) {
		Bullet *b = *it;
		if(b -> life == false or clearAll) {it = bullets.erase(it); delete b;}
		else it++;
	}

	std::list<Enemy*>::iterator it1;
	for(it1 = enemies.begin(); it1 != enemies.end();) {
		Enemy *e = *it1;
		if(e->life == false or clearAll) {it1 = enemies.erase(it1); delete e;}
		else it1++;
	}

	std::list<Animation*>::iterator it2;
	for(it2 = animations.begin(); it2 != animations.end();) {
		Animation *a = *it2;
		if(a->isEnd() or clearAll) {it2 = animations.erase(it2); delete a;}
		else it2++;
	}

	std::list<Sound*>::iterator it3;
	for(it3 = sounds.begin(); it3 != sounds.end();) {
		Sound *s = *it3;
		if(s -> getStatus() != Sound::Status::Playing or clearAll) {it3 = sounds.erase(it3); delete s;}
		else it3++;
	}

	std::list<Collectable*>::iterator it4;
	for(it4 = collectables.begin(); it4 != collectables.end();) {
		Collectable *c = *it4;
		if(c -> life == false or clearAll) {it4 = collectables.erase(it4); delete c;}
		else it4++;
	}

	if(clearAll) {
		std::list<Planet*>::iterator it5;
		for(it5 = planets.begin(); it5 != planets.end();) {
			Planet *p = *it5;
			it5 = planets.erase(it5); delete p;
		}
	}
}

void Board::isEnd() {
	if(killedEnemies == enemyCount and time > 200) {
		reset();
		generateRandom();
	}
}
void Board::gameOver() {
	createAnimation(player.getPosition(), "machineExplosion");
	player.death();
	bullets.clear();
}
void Board::reset() {
	enemyCount = killedEnemies = 0;
	minGunSpeed *= 0.9; maxGunSpeed *= 0.9;
	minGunSpeed = std::max(minGunSpeed, 20); maxGunSpeed = std::max(maxGunSpeed, 30);
	planetCount += 2;

	left = std::max(-400.f, left - float(0.04 * W)); right = std::min(W + 400.f, right + float(0.04 * W));
	top = std::max(-400.f, top - float(0.04 * H)); bottom = std::min(H + 400.f, bottom + float(0.04  * H));

	camera.range.x = std::min(W * 1.15, camera.range.x * 1.02);
	camera.range.y = std::min(H * 1.15, camera.range.y * 1.02);

	removeDead(true);

	player.reset();
}

void Board::update(RenderWindow *window) {
	isEnd();
	checkCollisions();
	checkOffScreen();
	removeDead(false);

	for(auto e : enemies)
		e->update(bullets);

	for(auto b : bullets)
		b->update();

	for(auto a : animations)
		a->update();

	player.update(planets);

	camera.update(player, window);
}

void Board::drawTo(RenderWindow *window) {
	window -> draw(border);

	player.drawTo(window);

	for(auto p : planets)
		p -> drawTo(window);

	for(auto e : enemies)
		e -> drawTo(window);

	for(auto a : animations)
		a->drawTo(window);

	for(auto c : collectables) {
		c -> drawTo(window);
	}

	for(auto b : bullets)
		b->drawTo(window);


	time += 1;
}
