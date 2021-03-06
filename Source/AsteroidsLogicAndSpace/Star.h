#pragma once
#include "Object.h"

class Star : public Object
{
public:

	Star();

	void Init(const sf::Vector2u &size);
	void Update(float time) override;
	void Draw(sf::RenderWindow &window) override;
	void Reset() override;
	void Remove() override;

	~Star() {};
private:

	sf::CircleShape _star;
	uint8_t _alfaColor;
	float _radius;
	sf::Vector2f GetRandomPosition();
	void DefaultInit();
	void RandomInit();

};
