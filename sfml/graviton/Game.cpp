#include "Game.hpp"
#include "common/useful.hpp"

Game::Game() {
	ContextSettings settings; settings.antialiasingLevel = 16;
	window = new RenderWindow(VideoMode(W, H), "Graviton", Style::Default, settings);
	window->setFramerateLimit(120);

	shipOn.loadFromFile("images/ship_on.png");
	shipOff.loadFromFile("images/ship_off.png");
	machineExplosionTexture.loadFromFile("images/explosions/type_C.png");
	missile.loadFromFile("images/fire.png");
	enemyMissile.loadFromFile("images/fire_red.png");
	missileExplosionTexture.loadFromFile("images/explosions/type_B.png");
	background.loadFromFile("images/cosmos2.png");
	sun.loadFromFile("images/sun.png");
	fuelTexture.loadFromFile("images/fuel.png");

	machineExplosion.setup(machineExplosionTexture, 256, 256, 48, 2);
	missileExplosion.setup(missileExplosionTexture, 192, 192, 48, 1);
	missileExplosion.sprite.setScale(0.3, 0.3);

	sBackground.setTexture(background);

	fuelFont.loadFromFile("../fonts/myfont_bold.ttf");

	gravityToggle = false;
	spaceReleased = true;

	buffer.loadFromFile("sounds/bomb.wav");

	board.setup();

	drawnFramesCount = 0;
	framesPerSecond.setFont(fuelFont);
	framesPerSecond.setCharacterSize(32);
	framesPerSecond.setColor(Color::Yellow);
	clock.restart();
}

void Game::run() {
	while(window->isOpen()) {
		processEvents();
		update();
		render();
	}
}

void Game::processEvents() {
	Event event;
	while(window->pollEvent(event)) {
		switch(event.type) {
			case Event::Closed:
				window->close();
				break;
			case Event::KeyPressed:
				if(pressed(Keyboard::Key::Space) and spaceReleased) {
					Bullet *b = new Bullet();
					b->settings(Vector2f(board.player.pos.x, board.player.pos.y),
							Vector2f(friendlyMissileSpeed * cos(board.player.angle * DEGTORAD), friendlyMissileSpeed * sin(board.player.angle * DEGTORAD)),
							board.player.angle, -1,
							missile, "friendly");
					board.addBullet(b);
					spaceReleased = false;
				}
				else if(pressed(Keyboard::Key::P)) gravityToggle = !gravityToggle;
				else if(pressed(Keyboard::Key::B)) board.killedEnemies = board.enemyCount;
				else if(pressed(Keyboard::Key::K)) timeSlowDown = std::max(timeSlowDown - 0.1f, 0.01f);
				else if(pressed(Keyboard::Key::L)) timeSlowDown = std::min(timeSlowDown + 0.1f, 1.f);
				break;
			case Event::KeyReleased:
				if(event.key.code == Keyboard::Space) spaceReleased = true;
			default:
				break;
		}
	}
}

void Game::update() {
	board.update(window);
	// camera.update(board.player, window);
	interface.update(board.player.fuel / 10, board.camera.getPosition(), board.camera.getRange());
	updateFrameCounterPosition();
}

void Game::drawHitBoxes(RenderWindow *window) {
	board.player.drawHitBox(window);

	for(auto b : board.bullets) {
		b->drawHitBox(window);
	}

	for(auto e : board.enemies) {
		e->drawHitBox(window);
	}
}
void Game::drawBackground(RenderWindow *window) {
	float w = sBackground.getGlobalBounds().width, h = sBackground.getGlobalBounds().height;
	for(int i = -3; (i - 3) * w < W; ++i) {
		for(int j = -4; (j - 4) * h < H; ++j) {
			sBackground.setPosition(i * w, j * h);
			window->draw(sBackground);
		}
	}
}

void Game::updateFrameCounter() {
	float frames = drawnFramesCount / clock.getElapsedTime().asSeconds();
	int intFrames = frames;

	std::stringstream liczba; liczba << intFrames;
	framesPerSecond.setString(liczba.str());

	drawnFramesCount = 0;
	clock.restart();
}
void Game::updateFrameCounterPosition() {
	Camera *c = &board.camera;
	framesPerSecond.setPosition(c -> pos.x - c -> range.x / 2 + 30, c -> pos.y - c -> range.y / 2 + 15);
}

void Game::render() {
	window->clear(Color::Black);

	drawBackground(window);
	board.drawTo(window);
	interface.drawTo(window);

	++drawnFramesCount;
	if(clock.getElapsedTime().asSeconds() > 0.1f) {
		updateFrameCounter();
	}
	window -> draw(framesPerSecond);

	// drawHitBoxes(window);

	window->display();
}
