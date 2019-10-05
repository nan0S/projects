#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

class Collectable : public HitBox, public RectangleFundament {
public:
    float attributeAmount;
    collectableType type;

    Collectable() {}
    Collectable(Vector2f pos_, Texture &texture, float attributeAmount_, collectableType type_) {
        pos = pos_;
        attributeAmount = attributeAmount_;
        type = type_;

        shape.setTexture(texture);
        shape.setScale(0.2f, 0.2f);
        shape.setOrigin(shape.getGlobalBounds().width / 2, shape.getGlobalBounds().height / 2);
        shape.setPosition(pos);

        addElementToHitBox(Circle(Vector2f(0, 0), pos, shape.getGlobalBounds().width / 2));
    }

    void applyAttributeTo(Player &player) {
        switch(type) {
            case FUEL:
                player.fuel += attributeAmount;
        }
    }
};
