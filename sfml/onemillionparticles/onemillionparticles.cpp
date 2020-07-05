#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "common/useful.hpp"
using namespace sf;

const float W = 1920, H = 1080;
const float R = 0.5f, C = 0.0001f;
const Vector2f dir[] = {Vector2f(-1.92 * 50, 0), Vector2f(1.92 * 50, 0), Vector2f(0, -1.08 * 50), Vector2f(0, 1.08 * 50)};

Vector2f windowView;

class Particle {
	public:
		Vector2f pos, v;
		float mass, mass_1;
		VertexArray point;

		Particle() {
			pos = Vector2f(randInteger(0, W), randInteger(0, H));
			v = Vector2f(randFraction(0.f, 2.f), randFraction(0.f, 2.f));
			mass = randFraction(10.f, 50.f);
			mass_1 = 1 / mass;
			initPoint();
		}

		void initPoint() {
			point.setPrimitiveType(Points);
			point.resize(1);
			point[0] = Vertex(pos, Color::White);
		}

		void update() {
			calculateColor();

			v *= 0.995f;
			pos += v;
			point[0].position = pos;
		}

		void force(Vector2f force) {
			v += force * mass_1;
		}

		void calculateColor() {
			float d = v.x * v.x + v.y * v.y;
			float color = constrain(d, 0.f, 2500.f, 100.f, 10.f);
			point[0].color = hsv(color, color, color);
		}

		void draw(RenderWindow &window) {
			window.draw(point);
		}
};

class App {
	public:
		RenderWindow window;
		std::vector<Particle*> particles;

		App() {
			ContextSettings settings;
			settings.antialiasingLevel = 8;
			window.create(VideoMode(W, H), "One Million Particles", Style::Titlebar, settings);
			window.setFramerateLimit(60);

			windowView = Vector2f(0, 0);

			for(int i = 0; i < 30000; ++i) {
				particles.push_back(new Particle());
			}

			printHelp();
		}

		void printHelp() {
			std::cout << "One Million Particles!" << std::endl;
			std::cout << "Move cursor to make particles follow you and speed up!" << std::endl;
		}

		void run() {
			while(window.isOpen()) {
				processEvents();
				update();
				draw();
			}
		}

		void processEvents() {
			Event event;
			while(window.pollEvent(event)) {
				switch(event.type) {
					case Event::Closed:
						window.close();
						break;
					case Event::KeyPressed: {
						Keyboard::Key key = event.key.code;
						if(key == Keyboard::Key::W or key == Keyboard::Key::S)
							zoom(key);
						if(key == Keyboard::Key::Up or key == Keyboard::Key::Down or
								key == Keyboard::Key::Left or key == Keyboard::Key::Right)
							move(key);
					}
					default:
						break;
				}
			}
		}

		void update() {
			applyForces();
			for(auto p : particles)
				p -> update();
		}

		void applyForces() {
			Vector2i temp = Mouse::getPosition(window);
			Vector2f mousePos = Vector2f(temp.x + windowView.x, temp.y + windowView.y);

			for(auto p : particles) {
				Vector2f F = mousePos - p -> pos;
				float d = dist(mousePos, p -> pos);
				F *= d * C;
				p -> force(F);
			}
		}

		void zoom(Keyboard::Key key) {
			float zoomRatio = (key == Keyboard::Key::W) ? 0.9f : 1.1f;
			View view = window.getView();
			view.zoom(zoomRatio);
			window.setView(view);
		}

		void move(Keyboard::Key key) {
			View view = window.getView();
			view.move(dir[key - 71]);
			window.setView(view);
		}

		void draw() {
			window.clear(Color::Black);
			for(auto p : particles)
				p -> draw(window);	
			window.display();
		}
};

int main()
{
	App app;
	app.run();
	return 0;
}
