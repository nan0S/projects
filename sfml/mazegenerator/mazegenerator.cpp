#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <algorithm>
using namespace sf;

const int W = 1200, H = 720;
const int cellWidth = 20, cellHeight = 20;
const int rowCount = H / cellHeight, colCount = W / cellWidth;

enum direction {
	right,
	left,
	up,
	down
};

const int neighbourCount = 4;
const int dx[] = {1, 0, -1, 0};
const int dy[] = {0, 1, 0, -1};
const direction dir[] = {right, down, left, up};
int iterator[] = {0, 1, 2, 3};

VertexArray currentCell;

void updateCurrentCell(Vector2i pos) {
	Vector2i posOnScreen(pos.x * cellWidth, pos.y * cellHeight);
	currentCell[0].position = Vector2f(posOnScreen.x, posOnScreen.y);
	currentCell[1].position = Vector2f(posOnScreen.x + cellWidth, posOnScreen.y);
	currentCell[2].position = Vector2f(posOnScreen.x + cellWidth, posOnScreen.y + cellHeight);
	currentCell[3].position = Vector2f(posOnScreen.x, posOnScreen.y + cellHeight);
}

class Wall {
	public:
		bool downExists, rightExists;
		VertexArray downWall, rightWall;

		Wall() {}
};

class Cell : public Wall {
	public:
		VertexArray shape;
		Vector2i positionInArray, positionOnScreen;
		bool visited;

		Cell(Vector2i positionInArray_) {
			positionInArray = positionInArray_;
			positionOnScreen = Vector2i(positionInArray.x * cellWidth, positionInArray.y * cellHeight);

			visited = false;

			setupWalls();
			setupShape();
		}

		void setupWalls() {
			rightExists = downExists = true;

			downWall.setPrimitiveType(Lines);
			downWall.resize(2);
			rightWall.setPrimitiveType(Lines);
			rightWall.resize(2);

			for(int i = 0; i < 2; ++i)
				downWall[i].color = rightWall[i].color = Color(0, 0, 0);

			downWall[0].position = Vector2f(positionOnScreen.x, positionOnScreen.y + cellHeight);
			downWall[1].position = Vector2f(positionOnScreen.x + cellWidth, positionOnScreen.y + cellHeight);
			rightWall[0].position = Vector2f(positionOnScreen.x + cellWidth, positionOnScreen.y);
			rightWall[1].position = Vector2f(positionOnScreen.x + cellWidth, positionOnScreen.y + cellHeight);
		}

		void setupShape() {
			shape.setPrimitiveType(Quads);
			shape.resize(4);

			for(int i = 0; i < 4; ++i)
				shape[i].color = Color(150, 200, 50);

			shape[0].position = Vector2f(positionOnScreen.x, positionOnScreen.y);
			shape[1].position = Vector2f(positionOnScreen.x + cellWidth, positionOnScreen.y);
			shape[2].position = Vector2f(positionOnScreen.x + cellWidth, positionOnScreen.y + cellHeight);
			shape[3].position = Vector2f(positionOnScreen.x, positionOnScreen.y + cellHeight);
		}

		void removeWall(direction move) {
			if(move == down)
				downExists = false;
			else
				rightExists = false;
		}

		void SetColor(Color color)
		{
			for (int i = 0; i < 4; ++i)
				shape[i].color = color;
		}

		void draw(RenderWindow &window) {
			window.draw(shape);
			if(downExists) window.draw(downWall);
			if(rightExists) window.draw(rightWall);
		}
};

class Maze {
	public:
		Cell* canvas[colCount + 1][rowCount + 1];

		VertexArray border;

		std::vector<Vector2i> callStack;

		Maze() {
			setupBorder();
			setupCells();
		}

		void setupBorder() {
			border.setPrimitiveType(LinesStrip);
			border.resize(5);

			for(int i = 0; i < 5; ++i)
				border[i].color = Color(100, 100, 100);

			int xOff = 0, yOff = 0;
			border[0].position = Vector2f(xOff, yOff);
			border[1].position = Vector2f(W - xOff, yOff);
			border[2].position = Vector2f(W - xOff, H - yOff);
			border[3].position = Vector2f(xOff, H - yOff);
			border[4].position = Vector2f(xOff, yOff);
		}

		void setupCells() {
			for(int x = 0; x < colCount; ++x)
				for(int y = 0; y < rowCount; ++y)
					canvas[x][y] = new Cell(Vector2i(x, y));

			callStack.push_back(Vector2i(0, 0));

			currentCell.setPrimitiveType(Quads);
			currentCell.resize(4);
			for(int i = 0; i < 4; ++i)
				currentCell[i].color = Color(200, 50, 50);
		}

		void go() {
			Vector2i pos = callStack.back();
			updateCurrentCell(pos);
			if(canvas[pos.x][pos.y] -> visited == false) canvas[pos.x][pos.y] -> visited = true;

			std::random_shuffle(iterator, iterator + 4);
			bool found = false;

			for(int i = 0; i < neighbourCount; ++i) {
				int it = iterator[i];
				Vector2i newPos(pos.x + dx[it], pos.y + dy[it]);
				direction move = dir[it];

				if(newPos.x < 0 or newPos.x >= colCount or newPos.y < 0 or newPos.y >= rowCount)
					continue;
				if(canvas[newPos.x][newPos.y] -> visited == true)
					continue;

				if(move == right or move == down)
					canvas[pos.x][pos.y] -> removeWall(move);
				else {
					if(move == up)
						canvas[newPos.x][newPos.y] -> removeWall(down);
					else
						canvas[newPos.x][newPos.y] -> removeWall(right);
				}

				found = true;
				callStack.push_back(newPos);
				break;
			}

			if(!found)
				callStack.pop_back();
		}

		void makeMove() {
			go();
		}

		bool isEnd() {
			return callStack.empty();
		}

		void draw(RenderWindow &window) {
			for(int x = 0; x < colCount; ++x)
				for(int y = 0; y < rowCount; ++y)
					canvas[x][y] -> draw(window);
			window.draw(currentCell);
			window.draw(border);
		}
};

class App {
	public:
		RenderWindow window;
		Maze maze;

		App() {
			setupWindow();
		}

		void setupWindow() {
			ContextSettings settings; settings.antialiasingLevel = 16;
			window.create(VideoMode(W, H), "Maze Generator", Style::Default, settings);
			window.setFramerateLimit(60);
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

		void run() {
			while(window.isOpen()) {
				processEvents();
				if(maze.isEnd() == false) maze.makeMove();
				draw();
			}
		}

		void draw() {
			window.clear(Color::White);

			maze.draw(window);

			window.display();
		}
};

int main()
{
	srand(time(NULL));

	App app;
	app.run();

	return 0;
}
