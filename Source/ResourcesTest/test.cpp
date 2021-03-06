#include "ResourceManager.h"

int main()
{
	ResourceManager *rm = new ResourceManager();
	int counter = 0;

	// get and release	
	PictureResource* asteroid = rm->GetResource<PictureResource>("smallasteroid");
	counter = asteroid->GetRefCounter();

	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	window.clear();
	sf::Image img = asteroid->Get();
	sf::Texture* tx = new sf::Texture();
	tx->loadFromImage(img);
	sf::Sprite* sprite = new sf::Sprite();
	sprite->setTexture(*tx);
	window.draw(*sprite);
	window.display();

	rm->ReleaseResource("smallasteroid");
	counter = asteroid->GetRefCounter();	

	// uncomment for showing assert
	//window.clear();
	//img = asteroid->Get();
	//tx->loadFromImage(img);
	//sprite->setTexture(*tx);
	//window.draw(*sprite);
	//window.display();

	// release resouce with counter != 0
	counter = asteroid->GetRefCounter();
	asteroid = rm->GetResource<PictureResource>("smallasteroid");
	asteroid = rm->GetResource<PictureResource>("smallasteroid");
	asteroid = rm->GetResource<PictureResource>("smallasteroid");
	asteroid = rm->GetResource<PictureResource>("smallasteroid");
	counter = asteroid->GetRefCounter();
	rm->ReleaseResource("smallasteroid");
	rm->ReleaseResource("smallasteroid");
	counter = asteroid->GetRefCounter();	
	//rm->ReleaseAllResources();         // uncomment for showing assert

	// loading file not exist
	//AudioResource* sound = rm->GetResource<AudioResource>("booom");    // uncomment for showing assert

	// invalid resource key
	//AudioResource* sound = rm->GetResource<AudioResource>("booo");	 // uncomment for showing assert

	// release not loaded resource
	//rm->ReleaseResource("asteroid");            // uncomment for showing assert

	// release resource with invalid key
	//rm->ReleaseResource("aster");              // uncomment for showing assert

	// release 
	counter = asteroid->GetRefCounter();
	while (asteroid->GetRefCounter() != 0)
	{
		rm->ReleaseResource("smallasteroid");
	}
	counter = asteroid->GetRefCounter();

	// release resource with counter == 0
	counter = asteroid->GetRefCounter();
	asteroid = rm->GetResource<PictureResource>("smallasteroid");
	asteroid = rm->GetResource<PictureResource>("smallasteroid");
	counter = asteroid->GetRefCounter();
	rm->ReleaseResource("smallasteroid");
	rm->ReleaseResource("smallasteroid");
	//rm->ReleaseResource("smallasteroid");         // uncomment for showing assert
	counter = asteroid->GetRefCounter();

	// uncorrect resource type
	//AudioResource* sound = rm->GetResource<AudioResource>("smallasteroid");         // uncomment for showing assert

	window.close();

    return 0;
}

