#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<bits/stdc++.h>
using namespace sf;

const float W = 1920, H = 1080;
const int P = 7355011;
const int A = 20;
const float G = 1;
const float maxSpeed = 50;

struct object;

std::list<object> planet;

int randomInteger(int a, int b) {
	if(b < a) return 0;
	int mod = b - a + 1;
	int randomNumber = (((rand() * P) % mod + mod) % mod) + a;
	return randomNumber;
}

float randomFraction(float a, float b) {
	if(b < a) return 0;
	int mod = int(b - 1) * 100 + 1;
	float randomNumber = (((rand() * P) % mod + mod) % mod) * 0.01 + a;
	return randomNumber;
}

struct object {
	CircleShape shape;
	float mass, R;
	float vx, vy;

	object() {
		vx = randomFraction(-3, 3); vy = randomFraction(-3, 3);
		mass = randomFraction(1000, 5000);
		R = mass * 0.005;

		float x = randomFraction(0, 1200);
		float y = randomFraction(0, 800);
		shape.setRadius(R);
		shape.setOrigin(R / 2, R / 2);
		shape.setPosition(x, y);
		Color color(randomInteger(0, 255), randomInteger(0, 255), randomInteger(0, 255), randomInteger(155, 255));
		shape.setFillColor(color);
	}

	object(float mass_, float vx_, float vy_) {
		mass = mass_; vx = vx_; vy = vy_;
		R = mass * 0.005;

		float x = randomFraction(0, 1200);
		float y = randomFraction(0, 800);
		shape.setRadius(R);
		shape.setOrigin(R / 2, R / 2);
		shape.setPosition(x, y);
		Color color(randomInteger(0, 255), randomInteger(0, 255), randomInteger(0, 255), randomInteger(155, 255));
		shape.setFillColor(color);
	}

	Vector2f GetPos() {
		return shape.getPosition();
	}

	void SetPos(float x, float y) {
		shape.setPosition(Vector2f(x, y));
	}

	void force(float Fx, float Fy) {
		float ax = Fx / mass, ay = Fy / mass;
		vx += ax; vy += ay;
	}

	void Move(float dx, float dy) {
		shape.move(dx, dy);
	}

	void update() {
		float speed = sqrt(vx * vx + vy * vy);
		if(speed > maxSpeed) {
			vx *= maxSpeed / speed;
			vy *= maxSpeed / speed;
		}
		Move(vx, vy);
	}

	void Draw(RenderWindow &window) {
		window.draw(shape);
	}

	void setMassAndRadius(float mass_) {
		mass = mass_;
		R = mass * 0.005;
		shape.setRadius(R);
		shape.setOrigin(R / 2, R / 2);
	}
};

int main()
{
	srand(time(NULL));

	ContextSettings setting;
	setting.antialiasingLevel = 16;
	RenderWindow window(VideoMode(W, H), "Gravity Simulation", Style::Default, setting);
	window.setFramerateLimit(60);

	float X = W, Y = H;
	float massGrowth = 100;

	std::list<object>::iterator it;
	for(int i = 0; i < 1; ++i) {
		planet.push_back(object(10000, 0, sqrt((G * 10000) / (2 * 500))));
		planet.back().SetPos(300, (i + 5) * 100);
	}
	for(int i = 0; i < 1; ++i) {
		planet.push_back(object(10000, 0, -sqrt((G * 10000) / (2 * 500))));
		planet.back().SetPos(800, (i + 5) * 100);
	}

	float Mass = 0;
	bool zmienna = false;
	object whileClick;

	while(window.isOpen()) {
		Event e;

		/*std::list<object> L;
		  for(it = planet.begin(); it != planet.end(); ++it) {
		  Vector2f pos = it->GetPos();
		  if(pos.x > 0 and pos.x < W and pos.y > 0 and pos.y < H) {
		  L.push_back(*it);
		  }
		  else L.push_back(object());
		  }
		  std::swap(L, planet);*/

		if(Mouse::isButtonPressed(Mouse::Button::Left)) {
			if(zmienna == false) {
				whileClick = object();
				zmienna = true;
			}
		}
		if(zmienna) {
			Mass += massGrowth;
			whileClick.setMassAndRadius(Mass);
			Vector2i mousePos = Mouse::getPosition();
			whileClick.SetPos(float(mousePos.x), float(mousePos.y));
		}

		while(window.pollEvent(e)) {
			switch(e.type) {
				case Event::Closed: {
					window.close();
					break;
				}
				case Event::KeyPressed: {
					if(Keyboard::isKeyPressed(Keyboard::Key::V)) {
						X *= 1.1; Y *= 1.1;
						massGrowth *= 1.1;
						View view(Vector2f(W / 2, H / 2), Vector2f(X, Y));
						window.setView(view);
					}
					else if(Keyboard::isKeyPressed(Keyboard::Key::B)) {
						X /= 1.1; Y /= 1.1;
						massGrowth /= 1.1;
						View view(Vector2f(W / 2, H / 2), Vector2f(X, Y));
						window.setView(view);
					}
					break;
				}
				case Event::MouseButtonReleased: {
					Mass = 0; zmienna = false;
					Vector2i mousePos = Mouse::getPosition();
					whileClick.SetPos(float(mousePos.x), float(mousePos.y));
					planet.push_back(whileClick);
					break;
				}
				default:
					break;
			}
		}

		for(it = planet.begin(); it != planet.end(); ++it) {
			Vector2f pos1 = it->GetPos();
			float Fx = 0, Fy = 0;
			std::list<object>::iterator it2;
			for(it2 = planet.begin(); it2 != planet.end(); ++it2) {
				Vector2f pos2 = it2->GetPos();

				float C = it->mass * it2->mass;
				float d = sqrt((pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y));
				if(d == 0) continue;

				float Sin = (pos2.y - pos1.y) / d;
				float Cos = (pos2.x - pos1.x) / d;
				float F = (G * C) / (d * d);

				Fx += F * Cos;
				Fy += F * Sin;
			}
			it->force(Fx, Fy);
			it->update();
		}

		window.clear(Color::Black);
		for(it = planet.begin(); it != planet.end(); ++it) {
			it->Draw(window);
		}
		if(zmienna) whileClick.Draw(window);
		window.display();
	}
	return 0;
}
