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

    Animation() {
        time = 0;
        currentFrame = 0;
    }

    Animation(Animation &a) {
        sprite = a.sprite;
        w = a.w; h = a.h;
        frameCount = a.frameCount;
        currentFrame = a.currentFrame;
        animationSpeed = a.animationSpeed;
        time = a.time;
    }

    void setup(Texture &texture, float w_, float h_, int frameCount_, int animationSpeed_) {
        w = w_; h = h_;
        frameCount = frameCount_;
        animationSpeed = animationSpeed_;

        sprite.setTexture(texture);
        sprite.setTextureRect(IntRect(0, 0, w, h));
        sprite.setOrigin(w / 2, h / 2);
    }

    void loadNextFrame() {
        if(currentFrame + 1 <= frameCount) {
            sprite.setTextureRect(IntRect(currentFrame * w, 0, w, h));
        }
    }

    void update() {
        time += 1;
        if(time >= animationSpeed) {
            time = 0;
            ++currentFrame;
            loadNextFrame();
        }
    }

    void setPosition(Vector2f pos) {
        sprite.setPosition(pos);
    }

    bool isEnd() {
        return currentFrame + 1 > frameCount;
    }

    void drawTo(RenderWindow *window) {
        window->draw(sprite);
    }
};
