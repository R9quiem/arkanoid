#pragma once
#include <SFML/Graphics.hpp>

#include <nlohmann/json.hpp> 
using json = nlohmann::json;

#include <fstream>
#include <iostream>
#include <random>

#include <memory>
#include "Platform.h"
#include "Block.h"
#include "Entity.h"
#include "BonusItem.h"
#include "GameUI.h"

class Platform;

class Ball : public Entity, public std::enable_shared_from_this<Ball> {
public:
	Ball(float radius, const sf::Vector2f& position,float speed, sf::Color color = sf::Color::Red)
		: Entity (
			position,
			new sf::CircleShape(radius),
			color
	) {
		this->speed = speed;
		direction.x = 0;
		direction.y = -1.f;
	}
	void update(){
		position.x += speed * direction.x;
		position.y += speed * direction.y;
		shape->setPosition(position);
	}
	bool handleCollision(
		sf::RenderWindow& window,
		Platform* platform,
		std::vector<std::shared_ptr<Block>>& blocks,
		int& score,
		std::vector<std::shared_ptr<BonusItem>>& bonuses,
		std::vector<std::shared_ptr<Ball>>& balls,
		int& extraPlatforms,
		GameUI* gameUI
	); 
	void increaseSpeed(int delta) {this->speed += delta;}
private:
	float speed;
	sf::Vector2f direction;
	void changeDirection(float overlapX,float overlapY) {
		if (overlapX < overlapY) {
			direction.x = -direction.x;
		} else {
			direction.y = -direction.y;
		}
	}
	float getRadius() {
		return dynamic_cast<sf::CircleShape*>(shape)->getRadius();
	}
	void handleBallCollisions(
		sf::CircleShape* circle,
		std::vector<std::shared_ptr<Ball>>& balls
	);
	bool handleWindowCollisions(sf::RenderWindow& window, sf::CircleShape* circle, int& extraPlatforms);
	void handlePlatformCollisions(Platform* platform, sf::CircleShape* circle); 
	void handleBlockCollisions(std::vector<std::shared_ptr<Block>>& blocks,
		sf::CircleShape* circle,
		sf::RenderWindow& window,
		int& score,
		std::vector<std::shared_ptr<BonusItem>>& bonuses); 
	int getRandomNumberBetween(int start,int end) {
		std::random_device rd;  
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distrib(start, end);

		return distrib(gen);
	}
};

