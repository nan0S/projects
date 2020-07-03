#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include "Circle.h"
#include "../universalfunctions/useful.h"
using namespace sf;

const int W = 1920, H = 1080;
const int A = 60;
const int rows = H / A, cols = W / A;

Vector2f moveVector;

Image image;
Color mask[H + 1][W + 1];

std::list<Circle*> statics[rows + 1][cols + 1],
                    dynamics[rows + 1][cols + 1];

std::vector<Circle*> toDrawNow;

void init() {
    image.loadFromFile("images/shrek.jpg");
    moveVector = Vector2f((W - image.getSize().x) / 2, (H - image.getSize().y) / 2);

    for(int x = 0; x < image.getSize().x; ++x) {
        for(int y = 0; y < image.getSize().y; ++y) {
            mask[x][y] = image.getPixel(x, y);
        }
    }
}

bool isGoodToStart(Vector2f pos) {
    pos -= moveVector;

    if(pos.x < 0 or pos.x > image.getSize().x or pos.y < 0 or pos.y > image.getSize().y) return false;

    pos += moveVector;

    for(int y = 0; y <= rows; ++y) {
        for(int x = 0; x <= cols; ++x) {

            for(auto c : statics[y][x])
                if(dist(c -> pos, pos) < c -> R - 10)
                    return false;

            for(auto c : dynamics[y][x])
                if(dist(c -> pos, pos) < c -> R - 10)
                    return false;
        }
    }

    return true;
}

void addElement(Vector2f pos) {
    int row = pos.y / A;
    int col = pos.x / A;

    Vector2f posRelativeToImage = pos - moveVector;

    Color color = mask[int(posRelativeToImage.x)][int(posRelativeToImage.y)];
    float growingSpeed = randFraction(3, 5);
    float maxRadius = randFraction(2, 4);

    Circle* circle;
    circle = new Circle(pos, growingSpeed, maxRadius, color);
    dynamics[row][col].push_back(circle);
}

bool isColliding(Circle *c1, Circle *c2) {
    return dist(c1 -> pos, c2 -> pos) < c1 -> R + c2 -> R;
}

void checkCollisions() {

    for(int y1 = 0; y1 <= rows; ++y1) {
        for(int x1 = 0; x1 <= cols; ++x1) {
            for(auto c1 : dynamics[y1][x1]) {

                if(c1 -> growingSpeed == 0) continue;

                bool collision = false;

                for(int y2 = std::max(0, y1 - 1); y2 <= std::min(rows, y1 + 1); ++y2) {
                    for(int x2 = std::max(0, x1 - 1); x2 <= std::min(cols, x1 + 1); ++x2) {
                        for(auto c2 : dynamics[y2][x2]) {

                            if(c2 -> growingSpeed == 0 or c1 -> pos == c2 -> pos) continue;

                            if(isColliding(c1, c2)) {
                                // std::cout << "(" <<  c1 -> pos.x << ", " << c1 -> pos.y << ") (" << c2 -> pos.x << ", " << c2 -> pos.y << ") " << c1 -> R << " " << c2 -> R << "\n";
                                c2 -> growingSpeed = 0;
                                collision = true;
                                break;
                            }
                        }
                        if(collision) break;
                    }
                    if(collision) break;
                }
                if(collision) c1 -> growingSpeed = 0;
            }
        }
    }

    for(int y1 = 0; y1 <= rows; ++y1) {
        for(int x1 = 0; x1 <= cols; ++x1) {
            for(auto c1 : dynamics[y1][x1]) {

                if(c1 -> growingSpeed == 0) continue;

                bool collision = false;

                for(int y2 = std::max(0, y1 - 1); y2 <= std::min(rows, y1 + 1); ++y2) {
                    for(int x2 = std::max(0, x1 - 1); x2 <= std::min(cols, x1 + 1); ++x2) {
                        for(auto c2 : statics[y2][x2]) {

                            if(isColliding(c1, c2)) {
                                collision = true;
                                break;
                            }
                        }
                        if(collision) break;
                    }
                    if(collision) break;
                }

                if(collision) c1 -> growingSpeed = 0;
            }
        }
    }
}

void checkGrowing() {
    for(int y = 0; y <= rows; ++y) {
        for(int x = 0; x <= cols; ++x) {
            std::list<Circle*>::iterator it;
            for(it = dynamics[y][x].begin(); it != dynamics[y][x].end();) {
                Circle *c = *it;
                if(c -> growingSpeed == 0) {
                    toDrawNow.push_back(c);
                    statics[y][x].push_back(c);
                    it = dynamics[y][x].erase(it);
                }
                else it++;
            }
        }
    }
}

void lotteryPoint() {
    for(int i = 0; i < 10; ++i) {
        Vector2f pos(randInteger(0, W), randInteger(0, H));
        if(isGoodToStart(pos)) addElement(pos);
    }
}

void editMode(RenderWindow &window) {
    Vector2i m = Mouse::getPosition(window);
    Vector2f mousePos = Vector2f(m.x, m.y);

    for(int i = -14; i <= 14; i += 14) {
        for(int j = -14; j <= 14; j += 14) {
            Vector2f newPos(mousePos.x + i, mousePos.y + j);
            if(isGoodToStart(newPos))
                addElement(newPos);
        }
    }
}

void drawAndUpdateAll(RenderWindow &window) {
    window.display();

    for(auto c : toDrawNow) {
        c -> drawTo(window);
    }

    for(int y = 0; y <= rows; ++y) {
        for(int x = 0; x <= cols; ++x) {
            for(auto c : dynamics[y][x]) {
                c -> update();
                c -> drawTo(window);
            }
        }
    }
    window.display();
    toDrawNow.clear();
}

int main()
{
    srand(time(NULL));
    ContextSettings settings; settings.antialiasingLevel = 8;
    RenderWindow window(VideoMode(W, H), "Circle Picturing!", Style::Default, settings);

    init();

    bool leftPressed = false;

    while(window.isOpen()) {

        Event e;
        while(window.pollEvent(e)) {
            switch(e.type) {
                case Event::Closed:
                    window.close();
                    break;
                case Event::MouseButtonPressed:
                    if(Mouse::isButtonPressed(Mouse::Button::Left)) {
                        leftPressed = !leftPressed;
                    }
            }
        }

        // if(leftPressed) editMode(window);

        lotteryPoint();
        checkCollisions();

        checkGrowing();
        drawAndUpdateAll(window);
    }

    return 0;
}
