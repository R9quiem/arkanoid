#pragma once

#include <SFML/Graphics.hpp>

#include "Platform.h"
#include "Ball.h"
#include "Block.h"
#include "BonusItem.h"

#include <nlohmann/json.hpp> 
using json = nlohmann::json;

#include <fstream>
#include <random>

#include "GameUI.h"
#include "GameState.h"



class Game {

public:
	Game(sf::RenderWindow& window) : window(window){
		gameUI = new GameUI(gameState);
	};
	~Game() {
		delete platform;
		delete gameUI;
	}
	void init();

	void startGame() {
		gameState = GameState::Active;
	}
	void endGame(GameState state) {
		gameState = state;
	}
  void resetGame();
	void setBlocks(int level);
	void createBlock(int type, sf::Vector2f blockPosition);
	void update();
	void handleEvent(const sf::Event& event);
	void render() const;
private:
	GameUI *gameUI;
	Platform *platform;
	sf::RenderWindow& window;
	std::vector<std::shared_ptr<Ball>> balls;
	std::vector<std::shared_ptr<Block>> blocks;
	std::vector<std::shared_ptr<BonusItem>> bonuses;
	int extraPlatforms;
	GameState gameState;
	int score;
	bool moveLeftKeyPressed;
	bool moveRightKeyPressed;
};
