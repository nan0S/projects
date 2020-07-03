#include "Animation.hpp"

Animation::Animation() {
	time = 0;
	currentFrame = 0;
}

Animation::Animation(Animation &a) {
	sprite = a.sprite;
	w = a.w; h = a.h;
	frameCount = a.frameCount;
	currentFrame = a.currentFrame;
	animationSpeed = a.animationSpeed;
	time = a.time;
}

void Animation::setup(Texture &texture, float w_, float h_, int frameCount_, int animationSpeed_) {
	w = w_; h = h_;
	frameCount = frameCount_;
	animationSpeed = animationSpeed_;

	sprite.setTexture(texture);
	sprite.setTextureRect(IntRect(0, 0, w, h));
	sprite.setOrigin(w / 2, h / 2);
}

void Animation::loadNextFrame() {
	if(currentFrame + 1 <= frameCount) {
		sprite.setTextureRect(IntRect(currentFrame * w, 0, w, h));
	}
}

void Animation::update() {
	time += 1;
	if(time >= animationSpeed) {
		time = 0;
		++currentFrame;
		loadNextFrame();
	}
}

void Animation::setPosition(Vector2f pos) {
	sprite.setPosition(pos);
}

bool Animation::isEnd() {
	return currentFrame + 1 > frameCount;
}

void Animation::drawTo(RenderWindow *window) {
	window->draw(sprite);
}
