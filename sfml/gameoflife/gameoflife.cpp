#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include "../universalfunctions/useful.h"
using namespace sf;

const int W = 1200, H = 720;

const int cellWidth = 20, cellHeight = 20;
const int rowCount = W / cellHeight, columnCount = W / cellWidth;

const int dx[] = {-1, 0, 1, -1, 1, -1, 0, 1};
const int dy[] = {-1, -1, -1, 0, 0, 1, 1, 1};

class Cell {
public:
    VertexArray shape;
    Vector2f positionOnScreen, positionInArray;

    bool life, newLife;

    Cell(Vector2f positionInArray_) {
        positionInArray = positionInArray_;
        positionOnScreen = Vector2f(positionInArray.y * cellWidth, positionInArray.x * cellHeight);

        life = randInteger(0, 1);
        shape.setPrimitiveType(Quads);
        shape.resize(4);
    }

    void update() {
        life = newLife;
        if(life) {
            for(int i = 0; i < 4; ++i)
                shape[i].color = Color::Black;
        }
        else {
            for(int i = 0; i < 4; ++i) {
                shape[i].color = Color::White;
            }
        }
    }

    void draw(RenderWindow &window, bool gameStopped) {
        if(gameStopped) {
            shape[0].position = Vector2f(positionOnScreen.x + 0.02 * cellWidth, positionOnScreen.y + 0.02 * cellHeight);
            shape[1].position = Vector2f(positionOnScreen.x + 0.98 * cellWidth, positionOnScreen.y + 0.02 * cellHeight);
            shape[2].position = Vector2f(positionOnScreen.x + 0.98 * cellWidth, positionOnScreen.y + 0.98 * cellHeight);
            shape[3].position = Vector2f(positionOnScreen.x + 0.02 * cellWidth, positionOnScreen.y + 0.98 * cellHeight);
        }
        else {
            shape[0].position = Vector2f(positionOnScreen.x, positionOnScreen.y);
            shape[1].position = Vector2f(positionOnScreen.x + cellWidth, positionOnScreen.y);
            shape[2].position = Vector2f(positionOnScreen.x + cellWidth, positionOnScreen.y + cellHeight);
            shape[3].position = Vector2f(positionOnScreen.x, positionOnScreen.y + cellHeight);
        }
        window.draw(shape);
    }
};

RenderWindow window;
Cell* cells[rowCount + 1][columnCount + 1];
VertexArray verticalLine[columnCount + 1], horizontalLine[rowCount + 1];

class App {
public:
    bool gameStopped;

    App() {
        initWindow();
        initCells();
        initVariables();
        initBoard();
    }

    void initWindow() {
        ContextSettings settings; settings.antialiasingLevel = 16;
        window.create(VideoMode(W, H), "Cellular Automata", Style::Default, settings);
        window.setFramerateLimit(30);
    }
    void initCells() {
        for(int i = 0; i < rowCount; ++i) {
            for(int j = 0; j < columnCount; ++j) {
                Cell *c = new Cell(Vector2f(i, j));
                cells[i][j] = c;
            }
        }
    }
    void initVariables() {
        gameStopped = false;
    }
    void initBoard() {
        for(int i = 0; i <= rowCount; ++i) {
            horizontalLine[i].setPrimitiveType(Lines);
            horizontalLine[i].resize(2);

            horizontalLine[i][0].color = horizontalLine[i][1].color = Color(100, 100, 100, 150);

            horizontalLine[i][0].position = Vector2f(0, cellHeight * i);
            horizontalLine[i][1].position = Vector2f(W, cellHeight * i);
        }

        for(int i = 0; i <= columnCount; ++i) {
            verticalLine[i].setPrimitiveType(Lines);
            verticalLine[i].resize(2);

            verticalLine[i][0].color = verticalLine[i][1].color = Color(100, 100, 100, 150);

            verticalLine[i][0].position = Vector2f(cellWidth * i, 0);
            verticalLine[i][1].position = Vector2f(cellWidth * i, H);
        }
    }

    void processEvents() {
        Event event;
        while(window.pollEvent(event)) {
            switch(event.type) {
                case Event::Closed:
                    window.close();
                    break;

                case Event::KeyPressed:
                    switch(event.key.code) {
                        case Keyboard::Key::P:
                            gameStopped = !gameStopped;
                            break;
                        case Keyboard::Key::C:
                            clearBoard();
                    }

                case Event::MouseButtonPressed:
                    if(event.mouseButton.button == Mouse::Button::Left) {
                        Vector2i mousePosition = Mouse::getPosition(window);
                        Vector2i cellPosition(mousePosition.y / cellHeight, mousePosition.x / cellWidth);

                        Cell *c = cells[cellPosition.x][cellPosition.y];
                        c -> newLife = !(c -> life);
                        c -> update();
                    }
            }
        }
    }
    void update() {
        for(int i = 0; i < rowCount; ++i) {
            for(int j = 0; j < columnCount; ++j) {
                Cell *c = cells[i][j];
                int liveNeightbourCount = 0;

                for(int i = 0; i < 8; ++i) {
                    int x = c -> positionInArray.x + dx[i], y = c -> positionInArray.y + dy[i];
                    if(x < 0 or x >= rowCount or y < 0 or y >= columnCount)
                        continue;
                    if(cells[x][y] -> life == true)
                        ++liveNeightbourCount;
                }

                if(c -> life == true)
                    if(liveNeightbourCount <= 1 or liveNeightbourCount >= 4)
                        c -> newLife = false;
                    else
                        c -> newLife = true;
                else
                    if(liveNeightbourCount == 3)
                        c -> newLife = true;
                    else
                        c -> newLife = false;
            }
        }

        for(int i = 0; i < rowCount; ++i) {
            for(int j = 0; j < columnCount; ++j) {
                cells[i][j] -> update();
            }
        }
    }

    void clearBoard() {
        for(int i = 0; i < rowCount; ++i) {
            for(int j = 0; j < columnCount; ++j) {
                cells[i][j] -> newLife = false;
                cells[i][j] -> update();
            }
        }
    }
    void drawBackground() {
        for(int i = 0; i <= rowCount; ++i) {
            window.draw(horizontalLine[i]);
        }

        for(int i = 0; i < columnCount; ++i) {
            window.draw(verticalLine[i]);
        }
    }
    void render() {
        window.clear(Color::White);

        if(gameStopped) drawBackground();

        for(int i = 0; i < rowCount; ++i) {
            for(int j = 0; j < columnCount; ++j) {
                cells[i][j] -> draw(window, gameStopped);
            }
        }

        window.display();
    }

    void run() {
        while(window.isOpen()) {
            processEvents();
            if(!gameStopped) update();
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
