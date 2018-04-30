#pragma once
#include "Ammunition.h"
#include "Asteroid.h"
#include "ParticleAssets.h"
#include "Star.h"
#include "ResourceManager.h"

class Space
{
public:
	Space(const int totalCountAsteroids, const int totalCountStar, const sf::Vector2u windowSize);

	void AddExplosionRocket(const Event& cEvent);

	void AddExplosionBullet(const Event & cEvent);

	void AddAmmunition(ResourceManager * rm);
	
	void AddSomeStars(const int count);
	void AddSomeAsteroids(const int count, const sf::Sprite& sprite);
	void Update(const float physicsStepTargetFrameTime);
	void SetColliderVisible(bool param);
	Token_t _collisionWithRocket;
	Token_t _collisionWithBullet;
	std::vector<Asteroid *> asteroids;
	Ammunition* ammunition;

	~Space();
private:
	Pool<Asteroid> _poolAsteroid;
	std::vector<Star *> _stars;
	Pool<Star> _poolStar;
	Pool<ExplosionParticle> _poolExplosion;
	std::vector<ExplosionParticle *> _explosions;
	sf::Vector2u _sizeSpace;

};
