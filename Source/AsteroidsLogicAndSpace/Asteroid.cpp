#include "Asteroid.h"
#include "Mathematics.h"
#include <iostream>

//Pool<Asteroid> *Asteroid::poolAsteroid = nullptr;
//std::vector<RigidBody *> Asteroid::rigidBodies;

void Asteroid::DefaultInit()
{
	_angularVelocity = 0.0f;
	_rotationSpeed = 0;
	_sprite.setRotation(_angularVelocity);

	_linearVelocity = sf::Vector2f(0, 0);
	SetSpeed(_linearVelocity);

	SetCoordinates(sf::Vector2f(0, 0));
	_sprite.setPosition(GetCoordinates());

	_radius = 1.0f;
	SetRadius(_radius);

	_type = AsteroidType::Small;
	SetParametersFromType(_type);

	_sprite.setScale(_startScale, _startScale);
	_sprite.setOrigin(0, 0);

	_spawnBoundHeight = 100.0f;
	_spawnBoundWidth = 100.0f;

	SetMass(0.005f);
	
}

void Asteroid::RandomInit()
{
	_angularVelocity = GetFloatRandomValue(-15.0, 15.0);
	_rotationSpeed = GetFloatRandomValue(1, 80);
	_sprite.setRotation(_angularVelocity);
		
	_linearVelocity = sf::Vector2f(GetFloatRandomValue(-300, 300), GetFloatRandomValue(-300, 300));
	SetSpeed(_linearVelocity);

	float positionBoundX = GetFloatRandomValue(-_spawnBoundWidth, _spawnBoundWidth);
	float positionBoundY = GetFloatRandomValue(-_spawnBoundHeight, _spawnBoundHeight);
	float positionX = positionBoundX > 0 ? GetSizeWindow().x + positionBoundX : positionBoundX;
	float positionY = positionBoundY > 0 ? GetSizeWindow().y + positionBoundY : positionBoundY;

	SetCoordinates(sf::Vector2f(positionX, positionY));
	_sprite.setPosition(GetCoordinates());
}

void Asteroid::SetParametersFromType(AsteroidType type)
{
	switch (type)
	{
	case AsteroidType::Big:
		_startScale = 1.5;
		_health = 800.0;
		_damage = 60;
		_defense = 20;
		SetMass(0.015f);
		break;
	case AsteroidType::Middle:
		_startScale = 1.0;
		_health = 400.0;
		_damage = 40;
		_defense = 10;
		SetMass(0.01f);
		break;
	case AsteroidType::Small:
	default:
		_startScale = 0.5;
		_health = 100.0;
		_damage = 20;
		_defense = 5;
		SetMass(0.01f);
		break;
	}
	_life = true;
}

void Asteroid::InitFromCrash(const sf::Sprite &sprite, const sf::Vector2f &position, const AsteroidType type, const sf::Vector2u &size, bool isColliderVisible)
{
	_sprite = sprite;
	_sizeSpace = size;

	switch (type)
	{
	case AsteroidType::Small:
		_type = AsteroidType::Big;
		break;
	case AsteroidType::Big:
		_type = AsteroidType::Middle;
		break;
	case AsteroidType::Middle:
	default:
		_type = AsteroidType::Small;
		break;
	}

	SetParametersFromType(_type);
	
	RandomInit();

	_radius = _sprite.getLocalBounds().width / 2 * _startScale;
	SetRadius(_radius);

	_sprite.setOrigin(_sprite.getLocalBounds().width / 2, _sprite.getLocalBounds().width / 2);
	_sprite.setScale(sf::Vector2f(_startScale, _startScale));

	_sprite.setPosition(sf::Vector2f(GetFloatRandomValue(position.x - 50.0f, position.x + 50.0f), GetFloatRandomValue(position.y - 50.0f, position.y + 50.0f)));
	SetCoordinates(_sprite.getPosition());

	_halfLenght = GetLenght(sf::Vector2f(_sprite.getLocalBounds().width, _sprite.getLocalBounds().height)) / 2;

	SetColliderVisible(isColliderVisible);
	
	AddToDrawableManager();
}

Asteroid::Asteroid()
{
	_sprite = sf::Sprite(sf::Texture::Texture());
	DefaultInit();
}

void Asteroid::Reset()
{
	_sprite.setTexture(sf::Texture::Texture(), true);
	DefaultInit();
}

sf::Vector2f Asteroid::GetCoordinates() {
 	return RigidBody::GetCoordinates();
}

void Asteroid::OnCollisionHandler(const Event& cEvent)
{
	const CollisionEventBetweenAsteroids &collisionEvent = dynamic_cast<const CollisionEventBetweenAsteroids&>(cEvent);
	
	Asteroid *obj1 = collisionEvent._asteroid1;
	Asteroid *obj2 = collisionEvent._asteroid2;
	if (this == obj1)
	{
		if (_defense < obj2->_damage)
			this->_health -= obj2->_damage - _defense;
	}
	else if (this == obj2)
	{
		if (_defense < obj1->_damage)
			this->_health -= obj1->_damage - _defense;		
	}
	else
	{
		//Log
	}
	 if (_health <= 0)
	{
		_life = false;
 	}	
}  


void Asteroid::OnRocketCollisionHandler(const Event& cEvent)
{
	//const CollisionEventBetweenAsteroidAndRocket &collisionEvent = dynamic_cast<const CollisionEventBetweenAsteroidAndRocket&>(cEvent);
	
	this->_health -= 1000;

	if (_health <= 0)
	{
		_life = false;
	}
}


void Asteroid::OnBulletCollisionHandler(const Event& cEvent)
{
	//const CollisionEventBetweenAsteroidAndBullet &collisionEvent = dynamic_cast<const CollisionEventBetweenAsteroidAndBullet&>(cEvent);

	this->_health -= 1000;
	
	if (_health <= 0)
	{
		_life = false;
	}
}


void Asteroid::Init(const sf::Sprite &sprite, const sf::Vector2u &size)
{		
	_sprite = sprite;
	_sizeSpace = size;

	_type = static_cast<AsteroidType>(std::rand() % (static_cast<uint8_t>(AsteroidType::Count)-1));
	
	SetParametersFromType(_type);
	
	_sprite.setScale(sf::Vector2f(_startScale, _startScale));

	_radius = _sprite.getLocalBounds().width / 2 * _startScale;
	SetRadius(_radius);
	_sprite.setOrigin(_sprite.getLocalBounds().width / 2, _sprite.getLocalBounds().width / 2);

	RandomInit();

	_halfLenght = GetLenght(sf::Vector2f(_sprite.getLocalBounds().width, _sprite.getLocalBounds().height)) / 2;

	_zOrder = 2;
	AddToDrawableManager();
}

void Asteroid::AddToDrawableManager()
{
	Object::AddToDrawableManager();

	_tokens[collisionEventBetweenAsteroidsID] = Dispatcher::getInstance().Connect(EventTypes::collisionEventBetweenAsteroidsID,
		[&](const Event& event)
	{
		OnCollisionHandler(event);
	});

	_tokens[collisionEventBetweenAsteroidAndBulletID] = Dispatcher::getInstance().Connect(EventTypes::collisionEventBetweenAsteroidAndBulletID,
		[&](const Event& event)
	{
		OnBulletCollisionHandler(event);
	});

	_tokens[collisionEventBetweenAsteroidAndRocketID] = Dispatcher::getInstance().Connect(EventTypes::collisionEventBetweenAsteroidAndRocketID,
		[&](const Event& event)
	{
		OnRocketCollisionHandler(event);
	});
}

void Asteroid::Remove()
{
	Object::Remove();

	Dispatcher::getInstance().Disconnect(EventTypes::collisionEventBetweenAsteroidsID, _tokens[collisionEventBetweenAsteroidsID]);
	Dispatcher::getInstance().Disconnect(EventTypes::collisionEventBetweenAsteroidAndBulletID, _tokens[collisionEventBetweenAsteroidAndBulletID]);
	Dispatcher::getInstance().Disconnect(EventTypes::collisionEventBetweenAsteroidAndRocketID, _tokens[collisionEventBetweenAsteroidAndRocketID]);
}

void Asteroid::Update(float time)
{
	RigidBody::Update(time);

	_angularVelocity = _angularVelocity + time * _rotationSpeed;
	sf::Vector2f nextPosition = GetCoordinates() + GetSpeed() * time;

	if (nextPosition.y - _halfLenght > GetSizeWindow().y)
	{
		nextPosition.y -= GetSizeWindow().y + _halfLenght * 2;
		SetCoordinates(nextPosition);
	}
	else if (nextPosition.y + _halfLenght < 0)
	{
		nextPosition.y += GetSizeWindow().y + _halfLenght * 2;
		SetCoordinates(nextPosition);
	}

	if (nextPosition.x - _halfLenght > GetSizeWindow().x)
	{
		nextPosition.x -= GetSizeWindow().x + _halfLenght * 2;
		SetCoordinates(nextPosition);
	}
	else if (nextPosition.x + _halfLenght < 0)
	{
		nextPosition.x += GetSizeWindow().x + _halfLenght * 2;
		SetCoordinates(nextPosition);
	}

	_sprite.setPosition(GetCoordinates().x + GetRadius(), GetCoordinates().y + GetRadius());
	_sprite.setRotation(_angularVelocity);
}

void Asteroid::Draw(sf::RenderWindow &window)
{
	window.draw(_sprite);
	
	if (IsColliderVisible())
	{
		sf::CircleShape physicsShape(GetRadius());
		physicsShape.setPosition(GetCoordinates());

		float color;

		switch (_type)
		{
		case AsteroidType::Small:
			color = _health / 200.0f * 255.0f;
			break;
		case AsteroidType::Middle:
			color = _health / 300.0f * 255.0f;
			break;
		case AsteroidType::Big:
		default:
			color = _health / 400.0f * 255.0f;
			break;
		}

		physicsShape.setOutlineColor(sf::Color(255, static_cast<uint8_t>(color), static_cast<uint8_t>(color), 255));
		physicsShape.setFillColor(sf::Color::Transparent);
		physicsShape.setOutlineThickness(1);

		window.draw(physicsShape);
	}
}

