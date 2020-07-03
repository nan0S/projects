#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Board.hpp"
#include "Fundament.hpp"
#include "Enemy.hpp"
using namespace sf;

class Game {
public:
    RenderWindow *window;

    Board board;
    Interface interface;

    bool gravityToggle;
    bool spaceReleased;

    Text framesPerSecond;
    int drawnFramesCount;
    Clock clock;

    Game();

    void run();
    void processEvents();
    void update();

    void drawHitBoxes(RenderWindow *window);
    void drawBackground(RenderWindow *window);

    void updateFrameCounter();
    void updateFrameCounterPosition();

    void render();
};
