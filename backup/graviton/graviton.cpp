#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <list>
#include <math.h>
#include "../universalfunctions/useful.h"
#include "Animation.h"
#include "Const.h"
#include "Interface.h"
#include "Fundament.h"
#include "Planet.h"
#include "Bullet.h"
#include "Player.h"
#include "Collectable.h"
#include "Camera.h"
#include "Enemy.h"
#include "Board.h"
#include "Game.h"
using namespace sf;

int main()
{
    srand(time(NULL));

    Game game;
    game.run();

    return 0;
}
