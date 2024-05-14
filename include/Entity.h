#pragma once

#include <SFML/Graphics.hpp>


class Entity {
public:
    Entity(const sf::Vector2f &position, sf::Shape* shape,sf::Color color = sf::Color::White) : position(position), shape(shape) {
        shape->setPosition(position);
        shape->setFillColor(color);
    }

    virtual void render(sf::RenderWindow& window) const {
        window.draw(*shape);
    }

    sf::Vector2f getPosition() const {
        return position;
    }

    sf::Vector2f getSize() const {
        auto size = shape->getGlobalBounds();
        return sf::Vector2f(size.width,size.height);
    }

    virtual ~Entity() {
        delete shape;
    }

protected:
    sf::Vector2f position;
    sf::Shape *shape;
};
