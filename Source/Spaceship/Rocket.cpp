#include "Rocket.h"

Rocket::Rocket()
	: _speedValue(200.0f)
	, _rocketSprite()
	, _rocketTexture()
	, _speedDelayTime(sf::seconds(1.0f))
	, _deltaSpeedValue(600.0f)
	, _rocketScale(1.0f, 1.0f)
{
	_zOrder = 3;	
}

void Rocket::Init(const sf::Vector2f& position, const sf::Vector2f& rocketDirection, sf::Texture& rocketTexture)
{
	Add();
	
	_rocketScale = sf::Vector2f(0.8f, 0.8f);
	_direction = rocketDirection;
	_rocketTexture = &rocketTexture;
	_rocketSprite.setTexture(*_rocketTexture);
	_rocketSprite.setScale(_rocketScale);
	_rocketSprite.setOrigin(sf::Vector2f(_rocketTexture->getSize().x / 2.0f, _rocketTexture->getSize().y / 2.0f));
	float angle = std::atan(_direction.x / -_direction.y) * 180.0f / M_PI;
	if (_direction.y > 0)
		angle += 180.0f;

	_rocketSprite.setRotation(angle);
	
	SetSpeed(rocketDirection * _speedValue);
	_timeAfterShot = sf::seconds(0.0f);
	SetCoordinates(sf::Vector2f(position.x + rocketDirection.x * _rocketTexture->getSize().y / 2.0f, position.y + rocketDirection.y * _rocketTexture->getSize().y / 2.0f));
}

const sf::Sprite* Rocket::GetSprite()
{
	return &_rocketSprite;
}

void Rocket::Draw(sf::RenderWindow& window)
{
	window.draw(_rocketSprite);
}

void Rocket::Update(const sf::Time& deltaTime)
{
	RigidBody::Update(deltaTime.asSeconds());
	
	_timeAfterShot += deltaTime;
	if (_timeAfterShot.asSeconds() > _speedDelayTime.asSeconds())
	{
		SetSpeed(_direction * (_speedValue + _deltaSpeedValue));
	}

	_rocketSprite.setPosition(GetCoordinates());
}

void Rocket::Reset()
{
	_rocketSprite.setOrigin(0, 0);
	_rocketSprite.setPosition(0, 0);
	_rocketSprite.setRotation(0);
	_direction = sf::Vector2f(0, 0);
	_timeAfterShot = sf::seconds(0);
	SetSpeed(sf::Vector2f(0, 0));
}

void Rocket::Add()
{
	DrawableManager::getInstance()._drawableObjects.push_back(this);
	DrawableManager::getInstance().SortDrawableVector();
}

int Rocket::GetZOrder() const
{
	return _zOrder;
}
