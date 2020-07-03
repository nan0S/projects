#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <list>
#include <math.h>
#include "common/useful.hpp"
#include "Animation.hpp"
#include "Const.hpp"
#include "Interface.hpp"
#include "Fundament.hpp"
#include "Planet.hpp"
#include "Bullet.hpp"
#include "Player.hpp"
#include "Collectable.hpp"
#include "Camera.hpp"
#include "Enemy.hpp"
#include "Board.hpp"
#include "Game.hpp"
using namespace sf;

int main()
{
    srand(time(NULL));

    Game game;
    game.run();

    return 0;
}
