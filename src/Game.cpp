#include "Game.h"

void Game::render() const {
  if (platform) {
    platform->render(window);
  }
  else {
    std::cout << "Platform is null!\n";
  }

  gameUI->render(window);

  for (const auto& block : blocks) {
    if (block) {
      block->render(window);
    }
    else {
      std::cout << "Block is null!\n";
    }
  }
  for (const auto& bonus : bonuses) {
    if (bonus) {
      bonus->render(window);
    }
    else {
      std::cout << "Bonus is null!\n";
    }
  }
  for (const auto& ball : balls) {
    if (ball) {
      ball->render(window);
    }
    else {
      std::cout << "Ball is null!\n";
    }
  }
}

void Game::handleEvent(const sf::Event& event){
  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Left) {
      moveLeftKeyPressed = true; // Устанавливаем флаг нажатия клавиши влево
    }
    else if (event.key.code == sf::Keyboard::Right) {
      moveRightKeyPressed = true; // Устанавливаем флаг нажатия клавиши вправо
    }
  }
  else if (event.type == sf::Event::KeyReleased) {
    if (event.key.code == sf::Keyboard::Left) {
      moveLeftKeyPressed = false; // Сбрасываем флаг нажатия клавиши влево
    }
    else if (event.key.code == sf::Keyboard::Right) {
      moveRightKeyPressed = false; // Сбрасываем флаг нажатия клавиши вправо
    }
  }
  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Space) {
      startGame(); 
    }
  }
}

void Game::update() {
  if (gameState == GameState::BeforeStart) {
    return;
  }
  for (int i = 0; i < balls.size(); ++i) {
    balls[i]->update(); // Обновляем положение каждого шара
    bool isBallDead = false;
    isBallDead = balls[i]->handleCollision(window,platform,blocks,score,bonuses,balls,extraPlatforms,gameUI);
    if (isBallDead) {
      balls.erase(balls.begin() + i); // Удаляем шар с индексом i
      --i; // Уменьшаем счетчик, так как индексы сдвигаются влево
    }
  }
  platform->handleBonusCollisions(bonuses, balls, extraPlatforms, gameUI);
  if (moveLeftKeyPressed) {
    platform->moveLeft(5.f); // Двигаем платформу влево с учетом времени
  }
  else if (moveRightKeyPressed) {
    platform->moveRight(5.f, window.getSize().x); // Двигаем платформу вправо с учетом времени
  }
  for (int i = 0; i < bonuses.size(); ++i) {
    bonuses[i]->update();
  }
  gameUI->updateScore(score);
  gameUI->updateExtraPlatforms(extraPlatforms);
}

void Game::createBlock(int type, sf::Vector2f blockPosition) {

  if (type > 4 || type < 0) return;

  std::ifstream file("settings.json");
  json settings;
  file >> settings;

  float width = settings["blocks"]["block"]["width"];
  float height = settings["blocks"]["block"]["height"];

  BlockType blockType = static_cast<BlockType>(type);

  blocks.push_back(std::make_shared<Block>(width, height, blockPosition, blockType));

}

void Game::setBlocks(int level) {

  std::ifstream file("settings.json");
  json settings;
  file >> settings;

  float spacing = settings["blocks"]["spacing"];
  float startX = settings["blocks"]["startX"];
  float startY = settings["blocks"]["startY"];

  float block_width = settings["blocks"]["block"]["width"];
  float block_height = settings["blocks"]["block"]["height"];

  // Генератор случайных чисел
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distrib(0, 3); 

  std::ifstream levels("Levels.json");
  json tilemapData;
  levels >> tilemapData;

  auto tiles = tilemapData["level1"]["tiles"];
  int width = tilemapData["level1"]["width"];
  int height = tilemapData["level1"]["height"];

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      // Получаем тип блока для текущей ячейки
      int blockType = tiles[y][x];
      // Создаем блок в соответствии с типом
      if (blockType != 4) {
        sf::Vector2f blockPosition(startX + x * (spacing)+x * block_width, startY + y * (block_height + spacing));
        createBlock(blockType, blockPosition);
      }
    }
  }

};

void Game::init() {
  std::ifstream file("settings.json");
  json settings;
  file >> settings;

  float platform_width = settings["platform"]["width"];
  float platform_height = settings["platform"]["height"];

  float ball_radius = settings["ball"]["radius"];
  float speed = settings["ball"]["speed"];

  balls.push_back(std::make_shared<Ball>(ball_radius, sf::Vector2f(300, 300), speed));

  this->platform = new Platform(
    platform_width,
    platform_height,
    sf::Vector2f((float)(window.getSize().x / 2 - 50), (float)window.getSize().y - 50)
  );

  setBlocks(0);
  score = 0;
  extraPlatforms = 0;
  gameState = GameState::BeforeStart;
};

void Game::resetGame() {
  delete platform;
  balls.clear();
  blocks.clear();
  bonuses.clear();
  init();
}