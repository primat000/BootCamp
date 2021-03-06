#include <sstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "Particles.h"
#include "Drawable.h"
#include "ParticleAssets.h"


int main()
{
	sf::VideoMode videoMode(800, 800);
	sf::RenderWindow window(
		sf::VideoMode(videoMode),
		"Particle");
	window.setVerticalSyncEnabled(true);

	DrawableManager& dm = DrawableManager::getInstance();

	sf::Clock clock;
	sf::Time timer = clock.getElapsedTime();
	sf::Time deltaTime;

	int count = 0;
	sf::Vector2f LastMousePos(0, 0);
	RocketParticle rocket(1000);
	rocket.SetPosition(sf::Vector2f(600, 233));
	rocket.SetVelocity(sf::Vector2f(-0, -800));

	ExplosionParticle explosion(1000);
	explosion.SetPosition(sf::Vector2f(500, 500));
	explosion.Play();
	/*
	Spaceship spaceship(1000, window.getSize());
	spaceship.SetPosition(sf::Vector2f(300, 500));*/
	
	sf::Event event;
	
	while (window.isOpen())
	{

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		auto now = clock.getElapsedTime();
		deltaTime = now - timer;
		timer = now;

		rocket.Update(deltaTime * 1.f);
		explosion.Update(deltaTime * 1.f);
		//spaceship.Update(deltaTime * 1.f);
		window.clear(sf::Color::Black);
		
		dm.DrawScene(window);
		//~~~~~~~~~~~~~~just for testing~~~~~~~~~~~~~~~~~
		/*if (count % 269 == 0) {
			explosion.Stop();
			spaceship.SetVelocity(sf::Vector2f(std::rand() % 70 - 20, std::rand() % 70 - 20));
		}
		if (count % 41 == 0) {
			spaceship.Play();
		}*/
		if (count % 269 == 0) {

			if (explosion.IsEnd())
			{
				explosion.Reset();
				explosion.Play();
				//explosion.SetPosition(sf::Vector2f(std::rand() % 50 + 500, std::rand() % 50 + 500));
			}
			else {
				explosion.Stop();
				//DrawableManager::getInstance().RemoveDrawableObject(static_cast<*Drawable>(explosion))
			}
		}
		window.display();
		count++;
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	}
	return 0;
}