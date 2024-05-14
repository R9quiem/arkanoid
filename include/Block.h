#pragma once

#include <SFML/Graphics.hpp>

#include "Entity.h"

enum BlockType {
    Normal,         // Обычный блок
    Unbreakable,    // Неразрушаемый блок
    SpeedUp,        // Блок, увеличивающий скорость мячика
    Bonus           // Блок с бонусом
};
class Block : public Entity {
public:
    Block(float width, float height, const sf::Vector2f& position,BlockType type = BlockType::Normal)
        : Entity(
            position,
            new sf::RectangleShape(sf::Vector2f(width, height)),
            getColorForType(type)
        ) {
        this->type = type;
        this->health = 1;
    }
    unsigned int getHealth() {
        return health;
    }
    void decreaseHealth() {
        if (health >= 1) {
            --health;
        }
    }
    BlockType getType() {
        return type;
    }
    sf::Color getColor() {
        return shape->getFillColor();
    }
    void setColor(sf::Color color) {
        shape->setFillColor(color);
    }

private:
    BlockType type;
    unsigned int health;

    sf::Color getColorForType(BlockType type) {
        switch (type) {
        case BlockType::Normal:
            return sf::Color::White; // Пример цвета для обычного блока
        case BlockType::Unbreakable:
            return sf::Color(128, 128, 128); // Пример цвета для неразрушаемого блока
        case BlockType::SpeedUp:
            return sf::Color::Yellow; // Пример цвета для блока, увеличивающего скорость мячика
        case BlockType::Bonus:
            return sf::Color(0,191,255); // Пример цвета для блока с бонусом
        }
    }
};