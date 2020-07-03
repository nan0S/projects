#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include "Enemy.hpp"
#include "Planet.hpp"
#include "Collectable.hpp"
#include "Player.hpp"
#include "Camera.hpp"
#include "Interface.hpp"
using namespace sf;

class Board {
	public:
		std::list<Enemy*> enemies;
		std::list<Planet*> planets;
		std::list<Animation*> animations;
		std::list<Bullet*> bullets;
		std::list<Sound*> sounds;
		std::list<Collectable*> collectables;
		Player player;

		int enemyCount, killedEnemies;
		int minGunSpeed, maxGunSpeed;
		int planetCount;
		int fuelCount;

		int time;

		float top, bottom, left, right;
		VertexArray border;

		Camera camera;

		Board();

		void setup();
		void generateRandom();

		bool canAddNewPlanet(Planet *planet);
		bool canAddNewEnemy(Enemy *enemy);
		bool canAddNewCollectable(Collectable *collectable);

		void addPlanet(Planet *p);
		void addEnemy(Enemy *e);
		void addBullet(Bullet *b);
		void addSound(Vector2f pos);
		void addAnimation(Animation *a);
		void createAnimation(Vector2f pos, std::string name);
		void addCollectable(Collectable *c);

		bool offScreen(HitBox *a);
		bool isOffScreen(Vector2f pos, float inacuraccy);
		void checkOffScreen();
		void checkCollisions();
		void removeDead(bool clearAll);

		void isEnd();
		void gameOver();
		void reset();

		void update(Interface &interface, RenderWindow *window);

		void drawTo(RenderWindow *window);
};
