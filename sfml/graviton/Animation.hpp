#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

class Animation {
	public:
		Sprite sprite;
		float w, h;
		int frameCount, currentFrame;
		int animationSpeed, time;
		bool life;

		Animation();
		Animation(Animation &a);

		void setup(Texture &texture, float w_, float h_, int frameCount_, int animationSpeed_);
		void loadNextFrame();
		void update();
		void setPosition(Vector2f pos);
		bool isEnd();
		void drawTo(RenderWindow *window);
};
