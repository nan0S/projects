#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Animation.hpp" 
using namespace sf;

//okno
extern const float W, H;

//stale
extern const float ro;
extern const float DEGTORAD;
extern const float PI;

//grawitacja
extern const float GRAVITY;

//pociski
extern const float friendlyMissileSpeed;
extern const float nonFriendlyMissileSpeed;
extern const float missileRadius;

extern Texture missile, enemyMissile;
extern Texture missileExplosionTexture;

//czas
extern float timeSlowDown;

//przeciwnicy
extern const Vector2f dim;
extern const float gunSpeed;

//tlo
extern Texture background;
extern Texture sun;
extern Sprite sBackground;

//statek
extern const float pThrust;
extern const float maxSpeed;
extern const float angleSpeed;

extern const Vector2f fuelSize;
extern const Vector2f fuelPos;
extern const Vector2f fuelTextPos;
extern const float fuelUseSpeed;
extern const float fuelTextSize;

extern Texture shipOn, shipOff;
extern Texture machineExplosionTexture;

//czcionki
extern Font fuelFont;

//animacje
extern Animation machineExplosion;
extern Animation missileExplosion;

//dźwięki
extern sf::SoundBuffer buffer;

//enumeracje
enum collectableType {FUEL};
enum playerState {normal, accelerating};

//paliwo
extern Texture fuelTexture;
