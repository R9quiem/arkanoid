#pragma once

#include <SFML/Graphics.hpp>

#include "Ball.h"
#include "Entity.h"
#include "BonusItem.h"
#include "GameUI.h"

#include <nlohmann/json.hpp> 
using json = nlohmann::json;

#include <fstream>
#include <iostream>
#include <memory>

class Ball;

class Platform : public Entity{
public:
	Platform(float width, float height, const sf::Vector2f& position)
		: Entity(
			position,
			new sf::RectangleShape(sf::Vector2f(width,height))
		){}

	void moveLeft(float distance) {
		if (position.x <= 0) return;
		position.x -= distance;
		shape->setPosition(position);
	}
	void increaseSize() {
		sf::RectangleShape* rect = dynamic_cast<sf::RectangleShape*>(shape);
		float newWidth = rect->getSize().x + 20;
		float height = rect->getSize().y;
		rect->setSize(sf::Vector2f(newWidth, height));
	}
	void handleBonusCollisions(
		std::vector<std::shared_ptr<BonusItem>>& bonuses,
		std::vector<std::shared_ptr<Ball>>& balls,
		int& extraPlatforms,
		GameUI* gameUI
	); 
	void moveRight(float distance,int window_width) {
		if (position.x+getSize().x >= window_width) return;
		position.x += distance;
		shape->setPosition(position);
	}
};

