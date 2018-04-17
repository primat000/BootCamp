#include "Asteroid.h"
#include "ParticleSpace.h"
#include "ResourceManager.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

std::vector<Object *> objects;

/*void Init(sf::RenderWindow *window)
{
	std::srand(std::time(nullptr));

	/*Pool<Asteroid*> _poolAsteroid;
	Pool<ParticleSpace*> _poolParticle;

	ResourceManager *rm = new ResourceManager();
	int counterImageSequence = 0;

	PictureResource* asteroid = rm->GetResource<PictureResource>("asteroid");
	sf::Image image(*asteroid->Get());
	image.createMaskFromColor(sf::Color(0, 35, 57, 255), 0);

	sf::Sprite sprite;
	sf::Texture asteroidTexture;
	asteroidTexture.loadFromImage(image, sf::IntRect(300, 90, 85, 85));
	sprite.setTexture(asteroidTexture);

	int _nParticleSpace = (WINDOW_WIDTH / 50) * (WINDOW_HEIGHT / 50);
	int _nAsteroids = (WINDOW_WIDTH / 200) * (WINDOW_HEIGHT / 200);
	for (int i = 0; i < _nParticleSpace; ++i)
	{
		ParticleSpace* particle = new ParticleSpace();
		particle->window = window;
		particle->Init();

		objects.push_back(particle);
	}
	for (int i = 1; i <= _nAsteroids; ++i)
	{
		Asteroid* asteroid = new Asteroid();
		asteroid->window = window;
		asteroid->Init(sprite);

		objects.push_back(asteroid);
	}

}*/

int main()
{

	
	
	sf::Clock clock;
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Asteroid!");

	std::srand(std::time(nullptr));

	/*Pool<Asteroid*> _poolAsteroid;
	Pool<ParticleSpace*> _poolParticle;*/

	ResourceManager *rm = new ResourceManager();
	int counterImageSequence = 0;

	PictureResource* asteroid = rm->GetResource<PictureResource>("asteroid");
	sf::Image image(*asteroid->Get());
	image.createMaskFromColor(sf::Color(0, 35, 57, 255), 0);

	sf::Sprite sprite;
	sf::Texture asteroidTexture;
	asteroidTexture.loadFromImage(image, sf::IntRect(300, 90, 85, 85));
	sprite.setTexture(asteroidTexture);

	int _nParticleSpace = (WINDOW_WIDTH / 50) * (WINDOW_HEIGHT / 50);
	int _nAsteroids = (WINDOW_WIDTH / 200) * (WINDOW_HEIGHT / 200);
	for (int i = 0; i < _nParticleSpace; ++i)
	{
		ParticleSpace* particle = new ParticleSpace();
		particle->window = &window;
		particle->Init();

		objects.push_back(particle);
	}
	for (int i = 1; i <= _nAsteroids; ++i)
	{
		Asteroid* asteroid = new Asteroid();
		asteroid->window = &window;
		asteroid->Init(sprite);

		objects.push_back(asteroid);
	}

	while (window.isOpen()) 
	{
		sf::Event event;
		while (window.pollEvent(event)) 
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) 
			{
				window.close();
			}
		}

		float time = clock.getElapsedTime().asSeconds();
		clock.restart();

		window.clear();

		for (auto *object : objects)
		{
			object->Update(time);
			object->Draw();
		}

		window.display();
	}

	return 0;
}