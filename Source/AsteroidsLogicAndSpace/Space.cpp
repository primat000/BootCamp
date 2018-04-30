#include "Space.h"



Space::Space(const int totalCountAsteroids, const int totalCountStar, const sf::Vector2u windowSize) :
	_poolAsteroid(totalCountAsteroids), _poolStar(totalCountStar),_sizeSpace(windowSize), _poolExplosion(12)
{
	Dispatcher& dispatcher = Dispatcher::getInstance();
	_collisionWithRocket = dispatcher.Connect(collisionEventBetweenAsteroidAndRocketID, [&](const Event& event)
	{
		AddExplosionRocket(event);
	});
	_collisionWithBullet = dispatcher.Connect(collisionEventBetweenAsteroidAndBulletID, [&](const Event& event)
	{
		AddExplosionBullet(event);
	});
}


void Space::AddExplosionRocket(const Event& cEvent) {
	const CollisionEventBetweenAsteroidAndRocket &collisionEvent = dynamic_cast<const CollisionEventBetweenAsteroidAndRocket&>(cEvent);
	ExplosionParticle* explosion = _poolExplosion.Get();
  	auto position = collisionEvent._asteroid->GetCoordinates();
	explosion->SetPosition(position);
	explosion->Play();
	_explosions.push_back(explosion);
}

void Space::AddExplosionBullet(const Event& cEvent) {
	const CollisionEventBetweenAsteroidAndBullet &collisionEvent = dynamic_cast<const CollisionEventBetweenAsteroidAndBullet&>(cEvent);
	ExplosionParticle* explosion = _poolExplosion.Get();
	auto position = collisionEvent._asteroid->GetCoordinates();
	explosion->SetPosition(position);
	explosion->Play();
	_explosions.push_back(explosion);
}

void Space::AddAmmunition(ResourceManager *rm)
 {
	ammunition = new Ammunition(rm);
	ammunition->Init();
}


void Space::AddSomeStars(const int count)
{
	for (int i = 0; i < count; ++i)
	{
		Star* star = _poolStar.Get();
		star->Init(_sizeSpace);
		_stars.push_back(star);
	}
}

void Space::AddSomeAsteroids(const int count, const sf::Sprite& sprite)
{
	for (int i = 1; i <= count; ++i)
	{
		Asteroid* asteroid = _poolAsteroid.Get();
		asteroid->Init(sprite, _sizeSpace);
		asteroids.push_back(asteroid);
	}
}

void Space::SetColliderVisible(bool param)
{
	for (size_t i = 0; i < asteroids.size(); ++i)
	{
		asteroids[i]->SetColliderVisible(param);
	}
}

void Space::Update(const float physicsStepTargetFrameTime)
{
	for (size_t i = 0; i < asteroids.size(); ++i)
	{ 
		Asteroid *asteroid = asteroids[i];
		if (asteroid->_life == false)
		{
			if (asteroid->_type != AsteroidType::Small)
			{
				for (int j = 0; j < 4; ++j)
				{
					if (!_poolAsteroid.Empty())
					{
						Asteroid* asteroidNew = _poolAsteroid.Get();
						asteroidNew->InitFromCrash(asteroid->_sprite, asteroid->GetCoordinates(), asteroid->_type, _sizeSpace, asteroid->IsColliderVisible());
						asteroids.push_back(asteroidNew);
					}
				}
			}

			if (!(_poolAsteroid.Count() == _poolAsteroid.MaxCount()))
			{
				asteroid->Remove();
				_poolAsteroid.Put(asteroid);
				asteroids.erase(std::find(asteroids.begin(), asteroids.end(), asteroid));
				--i;
			}
		}
		asteroid->Update(physicsStepTargetFrameTime);
	}

	for (auto *star : _stars)
	{
		star->Update(physicsStepTargetFrameTime);
	}
	ammunition->Update(physicsStepTargetFrameTime);

	for (auto *explosion : _explosions)
	{
 		if (!explosion->IsEnd())
		{
			explosion->Update(sf::milliseconds(physicsStepTargetFrameTime*1000));
		}
		else
		{
			DrawableManager::getInstance().RemoveDrawableObject(static_cast<Drawable*>(&(explosion->particles)));
			_explosions.erase(std::find(_explosions.begin(), _explosions.end(), explosion));
			_poolExplosion.Put(explosion);
		}
	}
}


Space::~Space()
{

	for (auto &asteroid : asteroids)
	{
		if (!(_poolAsteroid.Count() == _poolAsteroid.MaxCount()))
		{
			asteroid->Remove();
			_poolAsteroid.Put(asteroid);
		}
	}

	for (auto &star : _stars)
	{
		if (!(_poolStar.Count() == _poolStar.MaxCount()))
		{
			star->Remove();
			_poolStar.Put(star);
		}
	}

	for (auto *explosion : _explosions)
	{
			_poolExplosion.Put(explosion);
	}
}
