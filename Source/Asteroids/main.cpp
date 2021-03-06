#include "Asteroids.h"

std::string GetNameForState(ButtonsState bState) {

	switch (bState)
	{
	case ButtonsState::JustPressed:
		return "JustPressed";
	case ButtonsState::Pressed:
		return "Pressed";
	case ButtonsState::JustReleased:
		return "JustReleased";
	case ButtonsState::Released:
		return "Released";
	default:
		return "Undefined";
		break;
	}
}

static int score = 0;

void Scoring(AsteroidType type)
{
	switch (type)
	{
	case AsteroidType::Middle:
		score += 15;
		break;
	case AsteroidType::Small:
		score += 20;
		break;
	case AsteroidType::Big:
	default:
		score += 10;
		break;
	}
};

int main()
{
	/*
	ConfigManagers, Leaderboard, DrawableManager and Dispatcher Initialization
	*/
	ConfigManager* cm1 = ConfigManager::Create("GameConfig.INI");
	ConfigManager* achievementCM = ConfigManager::Create("AchievementsConfig.INI");
	Dispatcher &   dispatcher = Dispatcher::getInstance();
	DrawableManager& drawableManager = DrawableManager::getInstance();
	bool isReset = false;
	Leaderboard *leaderboard = Leaderboard::Create();
	bool createAccount = true;
	std::string name;
	
	std::cout << "Enter your name:" << std::endl;
	std::cin >> name;
	leaderboard->Login(name, createAccount);
	leaderboard->Login(name);
	leaderboard->UpdateUserTitleDisplayName(name);
	leaderboard->UpdatePlayerStatistic(score);


	/*
	ResourceManager Initialization
	*/
	std::map<std::string, std::multimap<const std::string, const std::string>> resourceConfig;
	resourceConfig.insert(std::make_pair("AudioResource", cm1->GetCategory("AudioResource").GetParams()));
	resourceConfig.insert(std::make_pair("PictureResource", cm1->GetCategory("PictureResource").GetParams()));
	resourceConfig.insert(std::make_pair("TextureResource", cm1->GetCategory("TextureResource").GetParams()));
	std::multimap<const std::string, const std::string> imageSequenceCategory = cm1->GetCategory("ImageSequenceResource").GetParams();
	resourceConfig.insert(std::make_pair("ImageSequenceResource", imageSequenceCategory));
	std::vector<std::multimap<const std::string, const std::string>> imageSequenceSettings(imageSequenceCategory.size());

	for (auto i : imageSequenceCategory)
	{
		resourceConfig.insert(std::make_pair("ImageSequenceResource." + i.first,
			cm1->GetCategory("ImageSequenceResource." + i.first).GetParams()));
	}

	ResourceManager *rm = new ResourceManager(resourceConfig);
	GameOverScreen gameOverManager(rm);


	/*
	InputManager Initialization
	*/
	std::multimap<int, ButtonKey_t> actions;
	LogCategory category = cm1->GetCategory("Input");
	std::multimap<const std::string, const std::string> inputCategory = category.GetParams();

	for (auto i : inputCategory)
	{
		int a = atoi(i.first.c_str());
		int b = atoi(i.second.c_str());
		actions.insert(std::pair<int, int>(a, b));
	}

	InputManager input(actions);

	ButtonsState stateMoveUp;
	ButtonsState stateMoveDown;
	ButtonsState stateMoveLeft;
	ButtonsState stateMoveRight;
	ButtonsState stateExit;
	ButtonsState stateShoot;
	ButtonsState statePowerfullShoot;


	/*
	For Audio
	*/
	AudioResource * achievementSound = rm->GetResource<AudioResource>("achievementSound");


	/*
	For SpaceShip
	*/
	ImageSequenceResource* spaceshipImgseq = rm->GetResource<ImageSequenceResource>("spaceship");
	ImageSequenceResource* flickeringImgseq = rm->GetResource<ImageSequenceResource>("spaceshipFlickering");
	TextureResource* bulletTexture = rm->GetResource<TextureResource>("bullet");
	TextureResource* rocketTexture = rm->GetResource<TextureResource>("rocket");

	std::multimap<const std::string, const std::string> spaceshipConfig = cm1->GetCategory("SpaceshipConfig").GetParams();
	Spaceship* spaceship = new Spaceship(spaceshipConfig, input, *spaceshipImgseq, *flickeringImgseq, *rm);
	spaceship->AddToDrawableManager();
	BulletManager bulletManager(*bulletTexture, *rocketTexture);


	/*
	For Physics
	*/
	CollisionEventBetweenAsteroids collisionAsteroidVsAsteroid;
	CollisionEventBetweenAsteroidAndSpaceship collisionAsteroidVsSpaceship;
	CollisionEventBetweenAsteroidAndRocket collisionAsteroidVsRocket;
	CollisionEventBetweenAsteroidAndBullet collisionAsteroidVsBullet;
	DeleteBulletEvent deleteBulletEvent;
	CreateExplosionEvent createExplosion;

	CollisionEventBetweenAmmunitionAndBullet collisionAmmunitionVsBullet;
	CollisionEventBetweenAmmunitionAndRocket collisionAmmunitionVsRocket;
	CollisionEventBetweenAmmunitionAndSpaceship collisionAmmunitionVsSpaceship;
	CollisionEventBetweenAmmunitionAndAsteroid collisionAmmunitionVsAsteroid;


	/*
	Game Loop
	*/
	sf::RenderWindow rw(sf::VideoMode::getDesktopMode(), "Asteroids");
	WindowResolution::SetResolution(rw);

	TextureResource* resetButton = rm->GetResource<TextureResource>("resetButton");

	AnimationPlayer* gameOverFlickering;
	ImageSequenceResource* gameOverImseq = rm->GetResource<ImageSequenceResource>("gameOver");;
	sf::Sprite gameOverSprite;
	gameOverFlickering = new AnimationPlayer(&gameOverSprite, gameOverImseq, true);
	gameOverFlickering->Start();
	
	sf::Event sysEvent;
	

	/*
	For UI
	*/
	UI ui(rw);
	sf::Font font;
	font.loadFromFile("Resources/font/arial.ttf");

	TextureResource* healthHearth = rm->GetResource<TextureResource>("health");
	TextureResource* bullets = rm->GetResource<TextureResource>("bullets");
	TextureResource* rockets = rm->GetResource<TextureResource>("rockets");
	PictureResource* achievements = rm->GetResource<PictureResource>("achievements");;

	ui.CreateLabel("90", font, PercentXY(17, 1), "bulletCount");
	ui.CreatePicture(bullets->Get(), PercentXY(14, 2), "bullets");
	ui.CreateLabel("10", font, PercentXY(25, 1), "rocketCount");
	ui.CreatePicture(rockets->Get(), PercentXY(22, 2), "rockets");

	ui.CreatePicture(healthHearth->Get(), PercentXY(5, 2), "Life0");
	ui.CreatePicture(healthHearth->Get(), PercentXY(7, 2), "Life1");
	ui.CreatePicture(healthHearth->Get(), PercentXY(9, 2), "Life2");
	ui.CreateLabel("100", font, PercentXY(2, 1), "HP");
	ui.CreateLabel("0", font, PercentXY(93, 1), "score");

	ui.CreateAchivementShower(font, PercentXY(1, 1));

	ui.CreatePictureButton(resetButton->Get(), PercentXY(50, 90), "resetButton");
	ui.Get<PictureButton>("resetButton")->_isVisible = false;

	TextureResource* pause = rm->GetResource<TextureResource>("pause");
	ui.CreatePicture(pause->Get(),PercentXY(50,50),"pause");
	ui.Get<Picture>("pause")->_isVisible = false;

	sf::Image* ptrAchievements = achievements->Get();
	AchievementsManager achievementsManager(achievementCM, ptrAchievements, *achievementSound);
	/*
	For Menu
	*/
	TextureResource* background = rm->GetResource<TextureResource>("rocket");
	TextureResource* play = rm->GetResource<TextureResource>("startButton");
	TextureResource* exit = rm->GetResource<TextureResource>("exitButton");
	TextureResource* leaders = rm->GetResource<TextureResource>("scoreButton");
	UI menu(rw);
	menu.CreatePictureButton(play->Get(), PercentXY(50, 35), "startButton");
	menu.CreatePictureButton(leaders->Get(), PercentXY(50, 50), "scoreButton");
	menu.CreatePictureButton(exit->Get(), PercentXY(50, 65), "exitButton");
	
	menu.SetBackground(background->Get());
	/*
	For Space
	*/
	TextureResource* asteroid = rm->GetResource<TextureResource>("asteroidTexture");

	sf::Texture asteroidTexture = asteroid->Get();
	sf::Sprite spriteAsteroid(asteroidTexture);

	std::srand(std::time(nullptr));

	const int totalCountAsteroids = 100;
	const int totalCountStar = (WindowResolution::_W / 50) * (WindowResolution::_H / 50) + 10;
	Space space(totalCountAsteroids, totalCountStar, rw.getSize(), *rm);

	int _nStars = (WindowResolution::_W / 50) * (WindowResolution::_H / 50) - 300;
	int _nAsteroids = (WindowResolution::_W / 200) + (WindowResolution::_H / 200);

	space.AddSomeStars(_nStars);
	space.AddSomeAsteroids(_nAsteroids, spriteAsteroid);
	space.AddAmmunition(rm);


	/*
	DebugCommandManager manager
	*/
	DebugCommandManager manager;
	manager.addConsoleCommand({ "setInvincibility", [&spaceship](const std::vector<std::string>& args)
	{
		spaceship->SetDamage(0);
	} });

	manager.addConsoleCommand({ "unsetInvincibility", [&spaceship, &spaceshipConfig](const std::vector<std::string>& args)
	{
		spaceship->SetDamage(atoi(spaceshipConfig.find("Damage")->second.c_str()));
	} });

	manager.addConsoleCommand({ "setCollidersVisible", [&spaceship, &space, &bulletManager](const std::vector<std::string>& args)
	{
		spaceship->SetColliderVisible(true);
		space.SetColliderVisible(true);
	} });

	manager.addConsoleCommand({ "setCollidersInvisible", [&spaceship, &space, &bulletManager](const std::vector<std::string>& args)
	{
		spaceship->SetColliderVisible(false);
		space.SetColliderVisible(false);
	} });

	spaceship->SetColliderVisible(false);
	space.SetColliderVisible(false);


	/*
	For Debug Console
	*/
	DebugConsole debugConsole(rw);


	/*
	For Log
	*/
	Logger& log = Logger::GetInstance();

	Token_t gameOver = dispatcher.Connect(resetGameEventID, [&](const Event& event)
	{
		space.Reset(_nAsteroids, spriteAsteroid);
		spaceship->Reset(spaceshipConfig);
		bulletManager.Reset();
		achievementsManager.Reset();
		leaderboard->UpdatePlayerStatistic(score);
		leaderboard->UpdateLocalLeaderboard();
		leaderboard->GetLeaderboard();	
		score = 0;
		ui.OnChangeScore(score);
		isReset = true;
	});
	std::string topScore = leaderboard->GetLeaderboard();
	menu.CreateLabel(topScore, font, PercentXY(10, 50), "leaderboard");
	menu.Get<Label>("leaderboard")->_isVisible = false;
	sf::Clock clock;
	sf::Time fixedTime;
	sf::Time deltaTime;
	const sf::Time fixedUpdateTime = sf::milliseconds(3);

	bool isGame = false;
	int iteration = 0;
	while (rw.isOpen())
	{
		while (!isGame)
		{
			rw.clear();
			menu.Render();
			if (rw.pollEvent(sysEvent))
			{
				switch (sysEvent.type)
				{
				case sf::Event::MouseButtonPressed:
					if(menu.Get<SfmlButton>("startButton")->IsClicked(sf::Mouse::getPosition(rw)))
					{
						isGame = true;
						clock.restart();
					}
					if (menu.Get<SfmlButton>("exitButton")->IsClicked(sf::Mouse::getPosition(rw)))
					{
						isGame = true;
						rw.close();
					}
					if (menu.Get<SfmlButton>("scoreButton")->IsClicked(sf::Mouse::getPosition(rw)))
					{
						menu.Get<Label>("leaderboard")->_isVisible == true ?
							menu.Get<Label>("leaderboard")->_isVisible = false :
							menu.Get<Label>("leaderboard")->_isVisible = true;						
					}
					break;
					
				default:
					break;
				}
				
			}
		}
		rw.clear();
		deltaTime = clock.restart();
		fixedTime += deltaTime;

		input.Update();
		ui.Get<Picture>("pause")->_isVisible = false;
		if (input.GetMode() == InputMode::GameOver) {
			gameOverManager.Update(fixedTime);
		}
		if (input.GetMode() == InputMode::Paused || input.GetMode() == InputMode::PausedRaw)
		{
			ui.Get<Picture>("pause")->_isVisible = true;
			fixedTime = sf::Time::Zero;	
		}


		if (rw.pollEvent(sysEvent))
		{
			if (sysEvent.type == sf::Event::MouseButtonPressed && ui.Get<PictureButton>("resetButton")->_isVisible && 
					ui.Get<PictureButton>("resetButton")->IsClicked(sf::Vector2i(sysEvent.mouseButton.x, sysEvent.mouseButton.y)))
			{
				ResetGameEvent resetGameEvent;
				dispatcher.Send(resetGameEvent, EventTypes::resetGameEventID);
			}

			if (input.GetMode() == InputMode::Raw || input.GetMode() == InputMode::PausedRaw)
			{
				input.HandleRawEvent(sysEvent);
				debugConsole.setActiveConsoleStatus(true);
			}
			else
			{
				debugConsole.setActiveConsoleStatus(false);
			}
		}

		if (fixedTime > fixedUpdateTime)
		{

			size_t bulletsSize = bulletManager.bullets.size();
			size_t rocketSize = bulletManager.rockets.size();
			//Input update
			{
				if (input.GetState(static_cast<int>(GameActions::Exit), stateExit) && (stateExit == ButtonsState::JustPressed || stateExit == ButtonsState::Pressed))
				{
					rw.close();
				}
			}
			//Audio update
			//Logic update

			size_t n = space.asteroids.size();
			size_t m = space.asteroids.size();

			for (size_t i = 0; i < n; ++i)
			{
				for (size_t j = i + 1; j < m; ++j)
				{
					
					if (Collided(*space.asteroids[i], *space.asteroids[j]))
					{
						collisionAsteroidVsAsteroid._asteroid1 = space.asteroids[i];
						collisionAsteroidVsAsteroid._asteroid2 = space.asteroids[j];
						ResolveCollision(*space.asteroids[i], *space.asteroids[j]);
						dispatcher.Send(collisionAsteroidVsAsteroid, collisionEventID, space.asteroids[i]->_tokens[collisionEventID]);
						dispatcher.Send(collisionAsteroidVsAsteroid, collisionEventID, space.asteroids[j]->_tokens[collisionEventID]);
					}
				}

				if (Collided(*space.asteroids[i], *spaceship))
				{
					collisionAsteroidVsSpaceship._asteroid = space.asteroids[i];
					collisionAsteroidVsSpaceship._spaceship = spaceship;
					ResolveCollision(*space.asteroids[i], *spaceship);
					dispatcher.Send(collisionAsteroidVsSpaceship, collisionEventBetweenAsteroidAndSpaceshipID, space.asteroids[i]->_tokens[collisionEventBetweenAsteroidAndSpaceshipID]);
					dispatcher.Send(collisionAsteroidVsSpaceship, collisionEventBetweenAsteroidAndSpaceshipID, spaceship->_tokens[collisionEventBetweenAsteroidAndSpaceshipID]);
					dispatcher.Send(collisionAsteroidVsSpaceship, collisionEventBetweenAsteroidAndSpaceshipID, achievementsManager.tokenForCollisionEventBetweenAsteroidAndSpaceship);
				}

				if (isReset)
				{
					isReset = false;
					continue;
				}

				for (size_t j = 0; j < rocketSize; ++j)
				{
					if (Collided(*space.asteroids[i], *bulletManager.rockets[j]))
					{
						collisionAsteroidVsRocket._asteroid = space.asteroids[i];
						collisionAsteroidVsRocket._rocket = bulletManager.rockets[j];
						createExplosion.position = space.asteroids[i]->GetCoordinates();
						ResolveCollision(*space.asteroids[i], *bulletManager.rockets[j]);
						dispatcher.Send(collisionAsteroidVsRocket, collisionEventBetweenAsteroidAndRocketID, space.asteroids[i]->_tokens[collisionEventBetweenAsteroidAndRocketID]);
						dispatcher.Send(collisionAsteroidVsRocket, collisionEventBetweenAsteroidAndRocketID, achievementsManager.tokenForCollisionEventBetweenAsteroidAndRocket);
						dispatcher.Send(createExplosion, createExplosionEvent, space._createExplosion);
						dispatcher.Send(collisionAsteroidVsRocket, collisionEventBetweenAsteroidAndRocketID, bulletManager.rockets[j]->_tokens[collisionEventBetweenAsteroidAndRocketID]);
						Scoring(space.asteroids[i]->_type);
						ui.OnChangeScore(score);

						for (size_t k = 0; k < n; ++k) 
						{
							if (Collided(*space.asteroids[k], *bulletManager.rockets[j]))
							{
								collisionAsteroidVsRocket._asteroid = space.asteroids[k];
								collisionAsteroidVsRocket._rocket = bulletManager.rockets[j];
								ResolveCollision(*space.asteroids[k], *bulletManager.rockets[j]);
								createExplosion.position = space.asteroids[k]->GetCoordinates();
								dispatcher.Send(createExplosion, createExplosionEvent, space._createExplosion);
								dispatcher.Send(collisionAsteroidVsRocket, collisionEventBetweenAsteroidAndRocketID, space.asteroids[k]->_tokens[collisionEventBetweenAsteroidAndRocketID]);
								Scoring(space.asteroids[i]->_type);
								ui.OnChangeScore(score);
							}
						}
						bulletManager.DeleteRocket(bulletManager.rockets[j]);
					}
				}

				for (auto bullet : bulletManager.bullets)
				{
					if (Collided(*space.asteroids[i], *bullet))
					{
						collisionAsteroidVsBullet._asteroid = space.asteroids[i];
						collisionAsteroidVsBullet._bullet = bullet;
						deleteBulletEvent._deletedBullet = bullet;
						ResolveCollision(*space.asteroids[i], *bullet);
						createExplosion.position = space.asteroids[i]->GetCoordinates();
						dispatcher.Send(createExplosion, createExplosionEvent, space._createExplosion);
						dispatcher.Send(collisionAsteroidVsBullet, collisionEventBetweenAsteroidAndBulletID, space.asteroids[i]->_tokens[collisionEventBetweenAsteroidAndBulletID]);
						dispatcher.Send(collisionAsteroidVsBullet, collisionEventBetweenAsteroidAndBulletID, achievementsManager.tokenForCollisionEventBetweenAsteroidAndBullet);
						dispatcher.Send(deleteBulletEvent, deleteBulletEventID, bulletManager.deleteBullet);
						Scoring(space.asteroids[i]->_type);
						ui.OnChangeScore(score);
					}
				}
			}
			{
				for (auto asteroid : space.asteroids)
				{
					if (Collided(*asteroid, *space.ammunition))
					{
						collisionAmmunitionVsAsteroid.asteroid = asteroid;
						collisionAmmunitionVsAsteroid.ammunition = space.ammunition;
						ResolveCollision(*asteroid, *space.ammunition);
						dispatcher.Send(collisionAmmunitionVsAsteroid, collisionEventID, asteroid->_tokens[collisionEventID]);
						dispatcher.Send(collisionAmmunitionVsAsteroid, collisionEventBetweenAmmunitionAndAsteroidId, space.ammunition->_tokens[collisionEventBetweenAmmunitionAndAsteroidId]);
					}
				}

				if (Collided(*spaceship, *space.ammunition))
				{
					collisionAmmunitionVsSpaceship.spaceship = spaceship;
					collisionAmmunitionVsSpaceship.ammunition = space.ammunition;
					ResolveCollision(*spaceship, *space.ammunition);
					dispatcher.Send(collisionAmmunitionVsSpaceship, collisionEventBetweenAmmunitionAndSpaceshipId, spaceship->_tokens[collisionEventBetweenAmmunitionAndSpaceshipId]);
					space.ammunition->capacity = 0;
					dispatcher.Send(collisionAmmunitionVsSpaceship, collisionEventBetweenAmmunitionAndSpaceshipId, space.ammunition->_tokens[collisionEventBetweenAmmunitionAndSpaceshipId]);

				}

				for (auto rocket : bulletManager.rockets)
				{
					if (Collided(*rocket, *space.ammunition))
					{
						collisionAmmunitionVsRocket.rocket = rocket;
						collisionAmmunitionVsRocket.ammunition = space.ammunition;
						ResolveCollision(*rocket, *space.ammunition);
						createExplosion.position = space.ammunition->GetCoordinates();
						dispatcher.Send(createExplosion, createExplosionEvent, space._createExplosion);
						dispatcher.Send(collisionAmmunitionVsRocket, collisionEventBetweenAmmunitionAndRocketId, rocket->_tokens[collisionEventBetweenAmmunitionAndRocketId]);
						dispatcher.Send(collisionAmmunitionVsRocket, collisionEventBetweenAmmunitionAndRocketId, space.ammunition->_tokens[collisionEventBetweenAmmunitionAndRocketId]);
						bulletManager.DeleteRocket(rocket);
					}
					
				}

				for (auto bullet : bulletManager.bullets)
				{
					if (Collided(*bullet, *space.ammunition))
					{
						collisionAmmunitionVsBullet.bullet = bullet;
						collisionAmmunitionVsBullet.ammunition = space.ammunition;
						ResolveCollision(*bullet, *space.ammunition);
						createExplosion.position = space.ammunition->GetCoordinates();
						deleteBulletEvent._deletedBullet = bullet;
						dispatcher.Send(createExplosion, createExplosionEvent, space._createExplosion);
						dispatcher.Send(deleteBulletEvent, deleteBulletEventID, bulletManager.deleteBullet); 
						dispatcher.Send(collisionAmmunitionVsBullet, collisionEventBetweenAmmunitionAndBulletId, space.ammunition->_tokens[collisionEventBetweenAmmunitionAndBulletId]);
					}
				}
			}

			space.Update(fixedTime.asSeconds());
			spaceship->Update(fixedTime);
			bulletManager.Update(fixedTime);
			achievementsManager.Update(fixedTime, ui);
			fixedTime = sf::Time::Zero;
		}
	
		//Rendering update
		//DebugConsole 
		if (debugConsole.getActiveConsoleStatus())
		{
			debugConsole.Draw(rw);
		}
		
		++iteration;
		drawableManager.DrawScene(rw);
		ui.Render();
		if (iteration % 10 == 0)
		{
			sf::Vector2u windowSize = rw.getSize();
			sf::Texture texture;
			texture.create(windowSize.x, windowSize.y);
			texture.update(rw);
			sf::Image screenshot = texture.copyToImage();

			sf::String file = std::to_string(iteration) + ".png";
			screenshot.saveToFile(file);
		}
	}
	
	if (leaderboard != nullptr) {
		delete leaderboard;
	}

	dispatcher.Disconnect(gameOverEventID, gameOver);
	delete cm1;
	delete achievementCM;
	return 0;
}