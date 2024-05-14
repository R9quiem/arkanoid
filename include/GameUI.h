#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"

#include <nlohmann/json.hpp> 
using json = nlohmann::json;

#include <fstream>

class GameUI {
public:
    GameUI(GameState& gameState) : gameState(gameState) {

        font.loadFromFile("C:\\dev\\fonts\\FormaDJRCyrillicText-Regular-Testing.ttf");

        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(480, 10); // Установка позиции текста

        extraPlatformsText.setFont(font);
        extraPlatformsText.setCharacterSize(24);
        extraPlatformsText.setFillColor(sf::Color::White);
        extraPlatformsText.setPosition(20, 10); // Установка позиции текста

        tempBonusText.setFont(font);
        tempBonusText.setCharacterSize(18);
        tempBonusText.setFillColor(sf::Color::White);
        tempBonusText.setPosition(10, 300);

        startGameText.setFont(font);
        startGameText.setCharacterSize(32);
        startGameText.setFillColor(sf::Color::White);
        startGameText.setPosition(85, 200);
        startGameText.setString("Press Space to start the game");

    }
    
    void updateScore(int score) {
        scoreText.setString("Score: " + std::to_string(score));
    }

    void updateExtraPlatforms(int extraPlatforms) {
        extraPlatformsText.setString("Extra platforms: " + std::to_string(extraPlatforms));
    }

    void showTempBonus(const std::string& bonusMessage) {
        tempBonusText.setString(bonusMessage);
        bonusClock.restart();
    }
    void render(sf::RenderWindow &window) {
        window.draw(scoreText);
        window.draw(extraPlatformsText);
        if (bonusClock.getElapsedTime().asSeconds() <= 2.0f) {
            window.draw(tempBonusText);
        }
        if (gameState == GameState::BeforeStart) {
            window.draw(startGameText);
        }
    }

private:
    sf::Font font;
    sf::Text scoreText;
    sf::Text tempBonusText;
    sf::Text extraPlatformsText;
    sf::Text startGameText;

    GameState& gameState;

    sf::Clock bonusClock;
};

