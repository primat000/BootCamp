#pragma once
#include "Bullet.h"
#include "ParticleAssets.h"
#include "EventSystem.h"
#include "CollisionEvent.h"


class Rocket final : public Bullet
{
private:
	

	sf::Texture* _rocketTexture;
	sf::Sprite _rocketSprite;
	const float _speedValuePixelsPerSecond;
	const sf::Time _speedDelayTime;
	sf::Vector2f _direction;
	const float _deltaSpeedValue;
	sf::Time _timeAfterShot;
	sf::Vector2f _rocketScale;
	float _halfSpriteLength;
public:
	Rocket();
	RocketParticle * _rocketParticle;
	bool isAlive = true;
	void Init(const sf::Vector2f& position, const sf::Vector2f& rocketDirection, sf::Texture& rocketTexture, RocketParticle& rocketParticle, bool isColliderVisible);
	const sf::Sprite* GetSprite();
	const float GetHalfSpriteLength();
	bool GetLifeStatus() const;
	void OnCollisionHandler(const Event & cEvent);
	void AddToDrawableManager() override;
	int GetZOrder() const override;
	void Draw(sf::RenderWindow& window) override;
	void Update(const sf::Time& deltaTime) override;
	void Reset() override;
};


