#include "AchievementsManager.h"

AchievementsManager::AchievementsManager(ConfigManager* achievementCM)
{
	auto achievementsListCategory = achievementCM->GetCategory("TimeAchievementsList").GetParams();
	for (const auto& achiev : achievementsListCategory)
	{
		std::multimap<const std::string, const std::string> achievementsConfig = achievementCM->GetCategory("TimeAchievementsList." + achiev.first).GetParams();
		Achievement achievement(achievementsConfig);
		_achievementsStorage["TimeAchievementsList"].push_back(achievement);
	}
	achievementsListCategory = achievementCM->GetCategory("DestroyAchievementsList").GetParams();
	for (const auto& achiev : achievementsListCategory)
	{
		std::multimap<const std::string, const std::string> achievementsConfig = achievementCM->GetCategory("DestroyAchievementsList." + achiev.first).GetParams();
		Achievement achievement(achievementsConfig);
		_achievementsStorage["DestroyAchievementsList"].push_back(achievement);
	}
	achievementsListCategory = achievementCM->GetCategory("DestroyAndTimeAchievementsList").GetParams();
	for (const auto& achiev : achievementsListCategory)
	{
		std::multimap<const std::string, const std::string> achievementsConfig = achievementCM->GetCategory("DestroyAndTimeAchievementsList." + achiev.first).GetParams();
		Achievement achievement(achievementsConfig);
		_achievementsStorage["DestroyAndTimeAchievementsList"].push_back(achievement);
	}

	Dispatcher& dispatcher = Dispatcher::getInstance();
	_tokenForCollisionEventBetweenAsteroidAndBullet = dispatcher.Connect(EventTypes::collisionEventBetweenAsteroidAndBulletID,
		[&](const Event& event)
	{
		const CollisionEventBetweenAsteroidAndBullet& currentEvent = static_cast<const CollisionEventBetweenAsteroidAndBullet&>(event);
		DestroyAndTimeAchievementsHandler(currentEvent._asteroid->_type);
		DestroyAchievementsHandler(currentEvent._asteroid->_type);
	});
}

void AchievementsManager::DestroyAchievementsHandler(const AsteroidType& type)
{
	if (type == AsteroidType::Big)
	{
		for (auto& achiev : _achievementsStorage["DestroyAchievementsList"])
		{
			if (achiev.GetIdAchievement() == 4 || achiev.GetIdAchievement() == 5)
			{
				achiev.SetProgressState(achiev.GetProgressState() + 1);
			}
		}
	}
	if (type == AsteroidType::Middle)
	{
		for (auto& achiev : _achievementsStorage["DestroyAchievementsList"])
		{
			if (achiev.GetIdAchievement() == 6 || achiev.GetIdAchievement() == 7)
			{
				achiev.SetProgressState(achiev.GetProgressState() + 1);
			}
		}
	}
}

void AchievementsManager::TimeAchievementsHandler()
{
}

void AchievementsManager::DestroyAndTimeAchievementsHandler(const AsteroidType& type)
{
}


void AchievementsManager::Update(sf::Time deltaTime)
{
	for (auto& achiev : _achievementsStorage)
	{
		for (auto it = achiev.second.begin(); it != achiev.second.end(); ++it)
		{
			if (it->GetProgressState() == it->GetProgressFinishState())
			{
				it->SetAchievedActive(true);
			}
		}
	}
}

void AchievementsManager::ShowAchievement(sf::RenderWindow& window)
{
	for (auto& achiev : _achievementsStorage)
	{
		for (auto it = achiev.second.begin(); it != achiev.second.end(); ++it)
		{
			if (it->GetAchievedActive())
			{
				//draw
			}
		}
	}
}
