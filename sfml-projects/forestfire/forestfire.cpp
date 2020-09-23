#include <iostream>
#include <SFML/Graphics.hpp>
#include "common/useful.hpp"
using namespace sf;

enum cellState {
	burning,
	growing
};

const int W = 1200, H = 720;
const int cellWidth = 4, cellHeight = 4;
const int rowCount = H /cellHeight, columnCount = W / cellWidth;

const int neighbourCount = 8;
const int dx[] = {-1, 0, 1, -1, 1, -1, 0, 1};
const int dy[] = {-1, -1, -1, 0, 0, 1, 1, 1};

const long long INF = 1e18;

class Cell {
	public:
		Vector2i positionOnScreen, positionInArray;
		VertexArray shape;
		cellState state;

		float greenColor, redColor;
		float newColor;

		float growingSpeed, burningSpeed;

		int igniteLimit;
		int igniteStatus;

		float weakTime; 
		int burnStrength;

		Cell(Vector2i positionInArray_) {
			positionInArray = positionInArray_;
			positionOnScreen = Vector2i(positionInArray.x * cellWidth, positionInArray.y * cellHeight);

			state = growing;
			greenColor = randFraction(0, 255);
			growingSpeed = randFraction(0.06, 0.1);
			burningSpeed = -growingSpeed * 15;

			igniteStatus = 0;
			igniteLimit = constrain(0, 0, 25, 20, 10);

			weakTime = randFraction(0, 200);

			shape.setPrimitiveType(Quads);
			shape.resize(4);
			shape[0].position = Vector2f(positionOnScreen.x, positionOnScreen.y);
			shape[1].position = Vector2f(positionOnScreen.x + cellWidth, positionOnScreen.y);
			shape[2].position = Vector2f(positionOnScreen.x + cellWidth, positionOnScreen.y + cellHeight);
			shape[3].position = Vector2f(positionOnScreen.x, positionOnScreen.y + cellHeight);
		}

		void applyChanges() {
			if(state == growing)
				greenColor = newColor;
			else
				redColor = newColor;

			for(int i = 0; i < 4; ++i) {
				if(state == growing)
					shape[i].color = Color(0, greenColor, 0);
				else
					shape[i].color = Color(redColor, 0, 0);
			}
		}

		void start(cellState command, int newBurnStrength = 5) {
			state = command;
			if(state == growing) {
				newColor = 0;
				weakTime = randFraction(30, 150);
				igniteStatus = 0;
			}
			else
			{
				newColor = 255;
				burnStrength = newBurnStrength;
			}
		}

		void grow() {
			newColor = greenColor;
			newColor += growingSpeed;
			newColor = std::min(newColor, 255.f);

			igniteLimit = newColor / 10;
			igniteLimit = constrain(igniteLimit, 0, 25, 20, 10);
		}

		void burn() {
			newColor = redColor;
			newColor += burningSpeed;
			if(newColor <= 0.f) start(growing);
		}

		void ignite(int sourceOfIgnitionCount) {
			igniteStatus += sourceOfIgnitionCount;
		}

		void draw(RenderWindow &window) {
			window.draw(shape);
		}
};

class App {
	public:
		RenderWindow window;
		Cell* cell[columnCount + 1][rowCount + 1];

		App() {
			printInfo();
			initWindow();
			initCells();
		}

		void printInfo()
		{
			std::cout << "Forest file simulation!" << std::endl;
			std::cout << "Fire starts randomly and then spreads!" << std::endl;
		}

		void initWindow() {
			ContextSettings settings; settings.antialiasingLevel = 8;
			window.create(VideoMode(W, H), "Forest Fire", Style::Titlebar, settings);
		}

		void initCells() {
			for(int x = 0; x < columnCount; ++x) {
				for(int y = 0; y < rowCount; ++y) {
					cell[x][y] = new Cell(Vector2i(x, y));
				}
			}
		}

		void processEvents() {
			Event event;
			while(window.pollEvent(event)) {
				switch(event.type) {
					case Event::Closed:
						window.close();
					default:
						break;
				}
			}
		}

		void update() {
			for(int i = 0; i < columnCount; ++i) {
				for(int j = 0; j < rowCount; ++j) {
					Cell *c = cell[i][j];

					if(c -> state == growing) {
						if(c -> greenColor >= c -> weakTime) {
							int burningNeighbourCount = 0;
							int maxBurnStrength = 0;

							for(int k = 0; k < neighbourCount; ++k) {
								int x = i + dx[k], y = j + dy[k];
								Cell *s = cell[x][y];

								if (rand() % 3 == 2)
									continue;

								if(x < 0 or x >= columnCount or y < 0 or y >= rowCount)
									continue;

								if(s -> state == burning and s -> redColor >= 30.f and s -> burnStrength > 0) {
									++burningNeighbourCount;
									maxBurnStrength = std::max(maxBurnStrength, s -> burnStrength);
								}
							}

							c -> ignite(burningNeighbourCount);

							if(c -> igniteStatus >= c -> igniteLimit)
								c -> start(burning, maxBurnStrength);
							else if (randInteger(0, 5000000) == 2555)
								c -> start(burning);
							else
								c -> grow();
						}
						else
							c -> grow();
					}
					else
						c -> burn();
				}
			}

			for(int x = 0; x < columnCount; ++x) {
				for(int y = 0; y < rowCount; ++y) {
					cell[x][y] -> applyChanges();
				}
			}
		}

		void render() {
			window.clear(Color::White);

			for(int x = 0; x < columnCount; ++x) {
				for(int y = 0; y < rowCount; ++y) {
					cell[x][y] -> draw(window);
				}
			}

			window.display();
		}

		void run() {
			while(window.isOpen()) {
				processEvents();
				update();
				render();
			}
		}
};

int main()
{
	App app;
	app.run();

	return 0;
}
