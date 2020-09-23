#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include <deque>

const int WIDTH = 800, HEIGHT = 600;
const int A = 20;
const int KOL = WIDTH / A, WIER = HEIGHT / A;

sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Snake", sf::Style::Titlebar);

sf::Font font_bold;
sf::Font font_light;

sf::Text snake("SNAKE", font_bold, 100);
sf::Text thegame("The Game", font_bold, 35);
sf::Text start("Press any button to START", font_light, 20);
sf::Text score("Score:", font_light, 25);
sf::Text best_score("Best Score:", font_light, 25);

int naj_wynik;

std::pair<int, int> food;

void stworz_menu() {
	font_bold.loadFromFile("fonts/font_bold.ttf");
	font_light.loadFromFile("fonts/font_light.ttf");

	snake.setOrigin(snake.getLocalBounds().width / 2, snake.getLocalBounds().height / 2);
	snake.setPosition(sf::Vector2f(WIDTH / 2, 100));

	thegame.setOrigin(thegame.getLocalBounds().width / 2, thegame.getLocalBounds().height / 2);
	thegame.setPosition(sf::Vector2f(WIDTH / 2, 185));

	start.setOrigin(start.getLocalBounds().width / 2, start.getLocalBounds().height / 2);
	start.setPosition(sf::Vector2f(WIDTH / 2, 380));
}

void print_help() {
	std::cout << "Snake The Game!" << std::endl;
	std::cout << "Navigate with WASD or arrow keys!" << std::endl;
}

void generate_food();

void init() {

	generate_food();

	snake.setCharacterSize(35);
	snake.setOrigin(sf::Vector2f(snake.getLocalBounds().width / 2, snake.getLocalBounds().height / 2));
	snake.setPosition(sf::Vector2f(WIDTH / 2, 17));

	thegame.setCharacterSize(13);
	thegame.setOrigin(sf::Vector2f(thegame.getLocalBounds().width / 2, thegame.getLocalBounds().height / 2));
	thegame.setPosition(sf::Vector2f(WIDTH / 2, 44));

	score.setOrigin(sf::Vector2f(score.getLocalBounds().width / 2, score.getLocalBounds().height / 2));
	score.setPosition(sf::Vector2f(65, 35));

	best_score.setOrigin(sf::Vector2f(best_score.getLocalBounds().width / 2, best_score.getLocalBounds().height / 2));
	best_score.setPosition(sf::Vector2f(672, 35));
}

bool is(sf::Keyboard::Key key) {
	return sf::Keyboard::isKeyPressed(key);
}

struct SNAKE {
	int x, y;
	int xspeed, yspeed;
	int score;
	std::deque<std::pair<int, int> > tail;

	SNAKE() {
		x = WIDTH / 2; y = HEIGHT / 2;
		xspeed = 1; yspeed = 0;
		score = 0;
	}

	void Draw() {
		sf::RectangleShape block(sf::Vector2f(A - 2, A - 2));
		block.setPosition(sf::Vector2f(x, y));
		block.setOutlineThickness(2);
		block.setOutlineColor(sf::Color::Black);
		window.draw(block);
		std::deque<std::pair<int, int> >::iterator it;
		for(it = tail.begin(); it != tail.end(); ++it) {
			block.setPosition(sf::Vector2f(it->first, it->second));
			window.draw(block);
		}
	}

	void direction() {
		if((xspeed != 1 or yspeed != 0) and (is(sf::Keyboard::Key::Left) or is(sf::Keyboard::Key::A))) {
			xspeed = -1; yspeed = 0;
		}
		else if((xspeed != -1 or yspeed != 0) and (is(sf::Keyboard::Key::Right) or is(sf::Keyboard::Key::D))) {
			xspeed = 1; yspeed = 0;
		}
		else if((xspeed != 0 or yspeed != 1) and (is(sf::Keyboard::Key::Up) or is(sf::Keyboard::Key::W))) {
			xspeed = 0; yspeed = -1;
		}
		else if((xspeed != 0 or yspeed != -1) and (is(sf::Keyboard::Key::Down) or is(sf::Keyboard::Key::S))) {
			xspeed = 0; yspeed = 1;
		}
	}

	bool eat() {
		if(x == food.first and y == food.second) {
			++score;
			return true;
		}
		else tail.pop_front();
		return false;
	}

	void update() {
		tail.push_back(std::make_pair(x, y));
		x += xspeed * A; y += yspeed * A;
	}

	bool death() {
		std::deque<std::pair<int, int> >::iterator it;

		if(x > (KOL - 2) * A or x < A or y > (WIER - 2) * A or y < 4 * A) return true;

		for(it = tail.begin(); it != tail.end(); ++it) {
			if(x == it->first and y == it->second) {
				return true;
			}
		}
		return false;
	}

	void restart() {
		x = WIDTH / 2 + 20; y = HEIGHT / 2 + 20;
		xspeed = 1; yspeed = 0;
		score = 0;
		tail.clear();
	}
};

SNAKE player;

void generate_food() {
	int xx = (rand() % (KOL - 2) + 1) * A, yy = (rand() % (WIER - 5) + 4) * A;
	bool czy = true;
	while(czy) {
		czy = false;
		if(xx == player.x and yy == player.y) czy = true;
		else {
			std::deque<std::pair<int, int> >::iterator it;
			for(it = player.tail.begin(); it != player.tail.end(); ++it) {
				if(xx == it->first and yy == it->second) {
					czy = true;
					break;
				}
			}
		}
		if(czy) {
			xx = (rand() % (KOL - 2) + 1) * A;
			yy = (rand() % (WIER - 5) + 4) * A;
		}
	}
	food = std::make_pair(xx, yy);
}

void wyswietl_jedzenie() {
	sf::RectangleShape jedz(sf::Vector2f(A, A));
	jedz.setPosition(sf::Vector2f(food.first, food.second));
	jedz.setFillColor(sf::Color::Red);
	window.draw(jedz);
}

void wyswietl_plansze() {
	window.draw(snake);
	window.draw(thegame);
	for(int i = 0; i < KOL; ++i) {
		sf::RectangleShape rect(sf::Vector2f(A, A));

		rect.setPosition(sf::Vector2f(i * A, 3 * A));
		window.draw(rect);

		rect.setPosition(sf::Vector2f(i * A, (WIER - 1) * A));
		window.draw(rect);
	}
	for(int i = 0; i < WIER; ++i) {
		sf::RectangleShape rect(sf::Vector2f(A, A));

		rect.setPosition(sf::Vector2f(0, i * A));
		window.draw(rect);

		rect.setPosition(sf::Vector2f((KOL - 1) * A, i * A));
		window.draw(rect);
	}
	std::stringstream liczba;
	liczba << player.score;
	sf::Text wynik(liczba.str(), font_bold, 25);
	wynik.setOrigin(sf::Vector2f(wynik.getLocalBounds().width / 2, wynik.getLocalBounds().height / 2));
	wynik.setPosition(sf::Vector2f(118, 36));
	window.draw(wynik);

	std::stringstream liczba2;
	liczba2 << naj_wynik;
	sf::Text wynik2(liczba2.str(), font_bold, 25);
	wynik2.setOrigin(sf::Vector2f(wynik2.getLocalBounds().width / 2, wynik2.getLocalBounds().height / 2));
	wynik2.setPosition(sf::Vector2f(755, 36));
	window.draw(wynik2);

	window.draw(score);
	window.draw(best_score);
}

void game_over() {
	naj_wynik = std::max(player.score, naj_wynik);

	sf::RectangleShape war1(sf::Vector2f(400, 200)), war2(sf::Vector2f(360, 180));

	war1.setOrigin(sf::Vector2f(war1.getLocalBounds().width / 2, war1.getLocalBounds().height / 2));
	war1.setFillColor(sf::Color::White);
	war1.setPosition(sf::Vector2f(WIDTH / 2, HEIGHT / 2));

	war2.setOrigin(sf::Vector2f(war2.getLocalBounds().width / 2, war2.getLocalBounds().height / 2));
	war2.setFillColor(sf::Color::Black);
	war2.setPosition(sf::Vector2f(WIDTH / 2, HEIGHT / 2));

	sf::Text over("GAME OVER !", font_bold, 50);
	//over.setFillColor(sf::Color::White);
	over.setOrigin(sf::Vector2f(over.getLocalBounds().width / 2, over.getLocalBounds().height / 2));
	over.setPosition(sf::Vector2f(WIDTH / 2, HEIGHT / 2 - 50));

	sf::Text res("Press any button to try again", font_light, 20);
	res.setOrigin(sf::Vector2f(res.getLocalBounds().width / 2, res.getLocalBounds().height / 2));

	res.setPosition(sf::Vector2f(WIDTH / 2, HEIGHT / 2 + 30));

	window.display();
	window.draw(war1);
	window.draw(war2);
	window.draw(over);
	window.draw(res);
	window.display();

	bool pressed = false;

	sf::Clock clock;

	while(window.isOpen()) {
		sf::Event event;
		while(window.pollEvent(event)) {
			switch(event.type) {
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::KeyPressed:
					if(clock.getElapsedTime().asSeconds() >= 0.2)
						pressed = true;
					break;
				case sf::Event::MouseButtonPressed:
					if(clock.getElapsedTime().asSeconds() >= 0.2)
						pressed = true;
					break;
				default:
					break;
			}
		}
		if(pressed) break;
	}

}

void run_game() {
	init();
	sf::Clock clock;
	while(window.isOpen()) {
		sf::Event event;
		while(window.pollEvent(event)) {
			switch(event.type) {
				case sf::Event::Closed:
					window.close();
					break;
				default:
					break;
			}
		}
		player.direction();
		player.update();

		if(player.eat()) generate_food();
		if(player.death()) {
			game_over();
			player.restart();
		}
		window.clear();
		player.Draw();
		wyswietl_jedzenie();
		wyswietl_plansze();
		window.display();

		clock.restart();
		sf::Time time = sf::seconds(1.f / (10.f + player.score / 2));
		sleep(time);
	}
}

int main()
{
	srand(time(NULL));
	stworz_menu();
	print_help();
	sf::Clock clock;

	window.clear(sf::Color::Black);
	window.draw(snake);
	window.draw(thegame);
	window.display();
	window.draw(snake);
	window.draw(thegame);
	window.draw(start);

	while(window.isOpen()) {
		sf::Event event;
		while(window.pollEvent(event)) {
			switch(event.type) {
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::KeyPressed:
					run_game();
					break;
				case sf::Event::MouseButtonPressed:
					run_game();
					break;
				default:
					break;
			}
		}

		sf::Time wait = sf::seconds(0.45);
		sleep(wait);
		window.display();
	}
}
