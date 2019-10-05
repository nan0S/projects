#include <iostream>
#include <SFML/Graphics.hpp>
using namespace sf;

//okno
const float W = 1920, H = 960;

//stale
const float ro = 0.1;
const float DEGTORAD = 0.017453f;
const float PI = 3.14159265;

//grawitacja
const float GRAVITY = 0.1;

//pociski
const float friendlyMissileSpeed = 5;
const float nonFriendlyMissileSpeed = 5;
const float missileRadius = 10;

Texture missile, enemyMissile;
Texture missileExplosionTexture;

//czas
float timeSlowDown = 1;

//przeciwnicy
const Vector2f dim(30, 10);
const float gunSpeed = 0.5;

//tlo
Texture background;
Texture sun;
Sprite sBackground;

//statek
const float pThrust = 0.05;
const float maxSpeed = 1.7;
const float angleSpeed = 3;

const Vector2f fuelSize(300, 25);
const Vector2f fuelPos(W - 400, 60);
const Vector2f fuelTextPos(W - 400, 90);
const float fuelUseSpeed = 1;
const float fuelTextSize = 20;

Texture shipOn, shipOff;
Texture machineExplosionTexture;

//czcionki
Font fuelFont;

//animacje
Animation machineExplosion;
Animation missileExplosion;

//dźwięki
sf::SoundBuffer buffer;

//enumeracje
enum collectableType {FUEL};
enum playerState {normal, accelerating};

//paliwo
Texture fuelTexture;
