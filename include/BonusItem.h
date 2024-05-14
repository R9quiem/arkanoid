#pragma once
#include "Entity.h"

enum BonusType {
    IncreasePlatformSize,
    IncreaseBallSpeed,
    ExtraPlatform,
    ExtraBall
};

class BonusItem : public Entity {
public:
    BonusItem(const sf::Vector2f& position, BonusType type)
        : Entity(
            position,
            new sf::ConvexShape(10),
            sf::Color::Yellow
        ) {
        sf::ConvexShape* star = dynamic_cast<sf::ConvexShape*>(shape);
        star->setPoint(0, sf::Vector2f(0, -12));
        star->setPoint(1, sf::Vector2f(4, 0));
        star->setPoint(2, sf::Vector2f(12, 0));
        star->setPoint(3, sf::Vector2f(6, 6));
        star->setPoint(4, sf::Vector2f(8, 16));
        star->setPoint(5, sf::Vector2f(0, 10));
        star->setPoint(6, sf::Vector2f(-8, 16));
        star->setPoint(7, sf::Vector2f(-6, 6));
        star->setPoint(8, sf::Vector2f(-12, 0));
        star->setPoint(9, sf::Vector2f(-4, 0));
        this->type = type;
    }
    void update() {
        position.y+=1;
        shape->setPosition(position);
    }
    BonusType getType() {
        return type;
    }
private:
    BonusType type;
};  