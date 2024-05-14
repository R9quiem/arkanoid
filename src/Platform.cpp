#include "Platform.h"

void Platform::handleBonusCollisions(
	std::vector<std::shared_ptr<BonusItem>>& bonuses,
	std::vector<std::shared_ptr<Ball>>& balls,
	int& extraPlatforms,
	GameUI* gameUI
) {
	float platformLeft = getPosition().x;
	float platformRight = getPosition().x + getSize().x;
	float platformTop = getPosition().y;
	float platformBottom = getPosition().y + getSize().y;

	// Проверяем столкновение с бонусами
	for (auto it = bonuses.begin(); it != bonuses.end();) {
		// Получаем границы бонуса
		float bonusLeft = (*it)->getPosition().x;
		float bonusRight = (*it)->getPosition().x + (*it)->getSize().x;
		float bonusTop = (*it)->getPosition().y;
		float bonusBottom = (*it)->getPosition().y + (*it)->getSize().y;

		// Проверяем столкновение мяча с платрмой
		if (platformRight >= bonusLeft && platformLeft <= bonusRight && platformBottom >= bonusTop && platformTop <= bonusBottom) {


			if ((*it)->getType() == BonusType::IncreasePlatformSize) {
				increaseSize();
			}
			if ((*it)->getType() == BonusType::ExtraBall) {
				std::ifstream file("settings.json");
				json settings;
				file >> settings;

				float ball_radius = settings["ball"]["radius"];
				float speed = settings["ball"]["speed"];

				balls.push_back(std::make_shared<Ball>(ball_radius, sf::Vector2f(300, 300), speed));
			}
			if ((*it)->getType() == BonusType::IncreaseBallSpeed) {
				for (auto ball : balls) {
					ball->increaseSpeed(0.3);
				}
			}
			if ((*it)->getType() == BonusType::ExtraPlatform) {
				++extraPlatforms;
			}
			switch ((*it)->getType()) {
			case(BonusType::ExtraPlatform):
				gameUI->showTempBonus("+1 extra platform");
				break;
			case(BonusType::ExtraBall):
				gameUI->showTempBonus("+1 ball");
				break;
			case(BonusType::IncreaseBallSpeed):
				gameUI->showTempBonus("ball speed increased on 0.3");
				break;
			case(BonusType::IncreasePlatformSize):
				gameUI->showTempBonus("platform size incresed on 20");
				break;
			}
			it = bonuses.erase(it);

		}
		else {
			++it;
		}
	}
}