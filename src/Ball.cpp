#include "Ball.h"

bool Ball::handleCollision(
	sf::RenderWindow& window,
	Platform* platform,
	std::vector<std::shared_ptr<Block>>& blocks,
	int& score,
	std::vector<std::shared_ptr<BonusItem>>& bonuses,
	std::vector<std::shared_ptr<Ball>>& balls,
	int& extraPlatforms,
	GameUI* gameUI) {

	bool ballDestroyed = false;
	sf::CircleShape* circle = dynamic_cast<sf::CircleShape*>(shape);

	handlePlatformCollisions(platform, circle);

	ballDestroyed = handleWindowCollisions(window, circle, extraPlatforms);

	handleBlockCollisions(blocks, circle, window, score, bonuses);

	handleBallCollisions(circle, balls);

	return ballDestroyed;

}
void Ball::handleBallCollisions(
	sf::CircleShape* circle,
	std::vector<std::shared_ptr<Ball>>& balls
) {
	for (const auto& otherBall : balls) {
		// Проверяем, что это не тот же самый мяч
		if (otherBall && otherBall != shared_from_this()) {
			sf::Vector2f otherBallPosition = otherBall->getPosition();

			sf::Vector2f otherBallCenter = otherBallPosition + sf::Vector2f(otherBall->getRadius(), otherBall->getRadius());

			sf::Vector2f thisBallCenter = position + sf::Vector2f(getRadius(), getRadius());

			sf::Vector2f distanceVector = thisBallCenter - otherBallCenter;
			float distance = std::sqrt(distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y);

			if (distance <= getRadius() + otherBall->getRadius()) {
				// Вычисляем разность координат по x и y
				float dx = thisBallCenter.x - otherBallCenter.x;
				float dy = thisBallCenter.y - otherBallCenter.y;

				// Определяем направление отталкивания
				if (std::abs(dx) > std::abs(dy)) {
					direction.x += dx > 0 ? 1 : -1;
				}
				else {
					direction.y += dy > 0 ? 1 : -1;
				}
			}
		}
	}
}


bool Ball::handleWindowCollisions(sf::RenderWindow& window, sf::CircleShape* circle, int& extraPlatforms) {

	// Получаем границы окна
	int windowWidth = window.getSize().x;
	int windowHeight = window.getSize().y;

	// Проверяем столкновение со стенами
	if (position.x + circle->getRadius() >= windowWidth || position.x - circle->getRadius() <= 0) {
		direction.x = -direction.x;
	}
	if (position.y - circle->getRadius() <= 0) {
		direction.y = -direction.y;
	}
	// Проверяем столкновение снизу (если шар ушел за пределы окна)
	if (position.y + circle->getRadius() >= windowHeight) {
		if (extraPlatforms > 0) {
			direction.y = -direction.y;
			--extraPlatforms;
		}
		else {
			return true;
		}
	}
	return false;
}

void Ball::handlePlatformCollisions(Platform* platform, sf::CircleShape* circle) {
	// Получаем границы шарика
	float ballLeft = position.x;
	float ballRight = position.x + circle->getRadius() * 2; // Учитываем диаметр шарика
	float ballTop = position.y;
	float ballBottom = position.y + circle->getRadius() * 2; // Учитываем диаметр шарика

	// Получаем границы платформы
	float platformLeft = platform->getPosition().x;
	float platformRight = platform->getPosition().x + platform->getSize().x;
	float platformTop = platform->getPosition().y;
	float platformBottom = platform->getPosition().y + platform->getSize().y;

	// Проверяем столкновение по горизонтали
	if (ballRight >= platformLeft && ballLeft <= platformRight) {
		// Проверяем столкновение по вертикали
		if (ballBottom >= platformTop && ballTop <= platformBottom) {

			// Определяем координаты центра платформы
			float platformCenterX = (platformLeft + platformRight) / 2.0f;
			float platformCenterY = (platformTop + platformBottom) / 2.0f;

			// Определяем направление от центра платформы к центру мяча
			float dx = position.x + circle->getRadius() - platformCenterX;
			float dy = position.y + circle->getRadius() - platformCenterY;

			// Нормализуем вектор направления
			float length = sqrt(dx * dx + dy * dy);
			dx /= length;
			dy /= length;

			direction.x = dx;
			direction.y = dy;
		}
	}
}

void Ball::handleBlockCollisions(
	std::vector<std::shared_ptr<Block>>& blocks,
	sf::CircleShape* circle,
	sf::RenderWindow& window,
	int& score,
	std::vector<std::shared_ptr<BonusItem>>& bonuses){
		// Получаем границы шарика
		float ballLeft = position.x;
		float ballRight = position.x + circle->getRadius() * 2; // Учитываем диаметр шарика
		float ballTop = position.y;
		float ballBottom = position.y + circle->getRadius() * 2; // Учитываем диаметр шарика


		// Проверяем столкновение с блоками
		for (auto it = blocks.begin(); it != blocks.end();) {
			// Получаем границы блока
			float blockLeft = (*it)->getPosition().x;
			float blockRight = (*it)->getPosition().x + (*it)->getSize().x;
			float blockTop = (*it)->getPosition().y;
			float blockBottom = (*it)->getPosition().y + (*it)->getSize().y;

			// Проверяем столкновение мяча с блоком
			if (ballRight >= blockLeft && ballLeft <= blockRight && ballBottom >= blockTop && ballTop <= blockBottom) {
				float overlapX = std::min(ballRight - blockLeft, blockRight - ballLeft);
				float overlapY = std::min(ballBottom - blockTop, blockBottom - ballTop);

				// Если блок неубиваемый, изменяем направление мяча и продолжаем
				if ((*it)->getType() == BlockType::Unbreakable) {
					changeDirection(overlapX, overlapY);
					++it;
					continue; // Продолжаем цикл обработки столкновений
				}

				// Если блок ускоряющий, увеличиваем скорость мяча
				if ((*it)->getType() == BlockType::SpeedUp) {
					increaseSpeed(0.2);
				}

				if ((*it)->getType() == BlockType::Bonus) {
					int randomX = getRandomNumberBetween(10,590);
					BonusType bonusType = (BonusType)getRandomNumberBetween(0,3);
					bonuses.push_back(std::make_shared<BonusItem>(sf::Vector2f(randomX, 0), bonusType));
				}

				changeDirection(overlapX, overlapY);

				(*it)->decreaseHealth();

				// Если здоровье блока достигло нуля, удаляем его
				if ((*it)->getHealth() == 0) {
					++score;
					it = blocks.erase(it); // Удаляем блок из вектора
				}
				else {
					// Уменьшаем прозрачность блока
					sf::Color color = (*it)->getColor();
					color.a -= 70; // Уменьшаем значение альфа-канала на 20
					(*it)->setColor(color);
					++it;
				}
				break;
			}
			else {
				++it;
			}
		}
	}