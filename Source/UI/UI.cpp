#include "UI.h"

UI::UI(sf::RenderWindow & window) :
	_window(window),
	_background(nullptr)
{
	_tokenUI = Dispatcher::getInstance().Connect(EventTypes::updateSpaceshipStateEvent,
		[&](const Event& event)
		{
			OnChangedSpaceshipState(event);
		});
	_tokenWeaponUI = Dispatcher::getInstance().Connect(EventTypes::updateSpaceshipWeaponStorageEvent,
		[&](const Event& event)
	{
		OnChangedSpaceshipStorage(event);
	});


	if (_widgets.find("resetButton") != _widgets.cend())
	{
		Get<PictureButton>("resetButton")->_isVisible = true;
	}

	gameOver = Dispatcher::getInstance().Connect(gameOverEventID, [&](const Event& event)
	{
		if (_widgets.find("resetButton") != _widgets.cend())
		{
			Get<PictureButton>("resetButton")->_isVisible = true;
		}
	});

	gameReset = Dispatcher::getInstance().Connect(resetGameEventID, [&](const Event& event)
	{
		if (_widgets.find("resetButton") != _widgets.cend())
		{
			Get<PictureButton>("resetButton")->_isVisible = false;
		}
	});
}

void UI::SetBackground(const sf::Texture & texture)
{
	const sf::Vector2f scale(_window.getSize().x / texture.getSize().x, _window.getSize().y / texture.getSize().y);
	_background = new Picture(texture, RelativeCordToAbs(PercentXY(50,50)), "background", _window);
	_background->SetScale(scale);
	_background->OnResize();
}

void UI::OnChangedSpaceshipStorage(const Event & event)
{

	const UpdateSpaceshipWeaponStorageEvent&  updateStateEvent = static_cast<const UpdateSpaceshipWeaponStorageEvent&>(event);
	if (_widgets["bulletCount"] != nullptr)
	{
		Get<Label>("bulletCount")->SetString(std::to_string(updateStateEvent._bulletCount));
	}
	if (_widgets["rocketCount"] != nullptr)
	{
		Get<Label>("rocketCount")->SetString(std::to_string(updateStateEvent._rocketCount));
	}
}

void UI::OnChangedSpaceshipState(const Event & event)
{

	const UpdateSpaceshipStateEvent&  updateStateEvent = static_cast<const UpdateSpaceshipStateEvent&>(event);
	if (_widgets["HP"] != nullptr)
	{
		Get<Label>("HP")->SetString(std::to_string(updateStateEvent._HP));
	}
	std::string life("Life");
	for (int i = 0; i < updateStateEvent._maxCountLife; ++i)
	{
		char symbol = i + '0';
		life.push_back(symbol);
		if (_widgets.find(life) != _widgets.cend())
		{
			if (i < (updateStateEvent._countLife))
			{
				Get<Picture>(life)->_isVisible = true;
			}
			else
			{
				Get<Picture>(life)->_isVisible = false;
			}
		}
		life.pop_back();
	}
}

void UI::OnResize()
{
	
	auto newSize = sf::Vector2i(_window.getSize().x, _window.getSize().y);
	auto oldSize = _window.getView().getSize();
	for (auto it : _widgets)
	{
		it.second->SetScale(sf::Vector2f(newSize.x / oldSize.x, newSize.y / oldSize.y));
		it.second->Resize();
	}
	_window.setView(sf::View(sf::FloatRect(0, 0, newSize.x, newSize.y)));
	Render();
}

void UI::OnChangeScore(int score)
{
	if (_widgets["score"] != nullptr)
	{
		Get<Label>("score")->SetString(std::to_string(score));
	}
}

void UI::Render()
{
	if (_background != nullptr)
	{
		_background->Draw();
	}
	for (auto it : _widgets)
	{
		it.second->Draw();
	}
	_window.display();
}

void UI::OnAchive(const std::string & name, const std::string & description,sf::Image * picture)
{
	Get<AchievementShower>("achivementShower")->ImplementAchivement(name,description,picture,sf::Time(sf::seconds(2)));
}

void UI::SetPostion(const std::string & key, const PercentXY relCoord)
{	
	_widgets[key]->SetPosition(RelativeCordToAbs(relCoord));	
}


const sf::Vector2f UI::RelativeCordToAbs(const PercentXY relCoord) const 
{
	return sf::Vector2f(_window.getSize().x / 100 * relCoord._x, _window.getSize().y / 100 * relCoord._y);
}


sf::Vector2f UI::GetPosition(const std::string & key)
{
	return _widgets[key]->GetPosition();
}

Widget * UI::CreateButton(const sf::Font & font, const sf::Vector2f size, const PercentXY relativePos, const std::string & name)
{	

	return _widgets[name] = new SfmlButton(font,size, RelativeCordToAbs(relativePos), name, _window);
}

Widget * UI::CreatePictureButton(const sf::Texture & picture, const PercentXY relativePos, const std::string & name)
{
	return _widgets[name] = new PictureButton(picture, RelativeCordToAbs(relativePos), name, _window);
}

Widget * UI::CreateLabel(const std::string & content, const sf::Font & font, const PercentXY relativePos, const std::string & name)
{
	return _widgets[name] = new Label(content, font, RelativeCordToAbs(relativePos), name, _window);
}

Widget * UI::CreateScrollBar(const float length, const PercentXY relativePos, const std::string & name)
{
	return _widgets[name] = new ScrollBar(length, RelativeCordToAbs(relativePos), name, _window);
}

Widget * UI::CreateAchivementShower(const sf::Font & font, const PercentXY relativePos)
{
	return _widgets["achivementShower"] = new AchievementShower(font, RelativeCordToAbs(relativePos), "achivementShowerName", "achivementShowerDescription", _window);
}

Widget * UI::CreatePicture(const sf::Texture & img, const PercentXY relativePos, const std::string & name)
{
	return _widgets[name] = new Picture(img, RelativeCordToAbs(relativePos), name, _window);
}

void UI::RemoveWidget(const std::string & key)
{
	delete _widgets[key];
	_widgets.erase(key);
}

Widget* UI::GetWidget(const std::string &key)
{
	auto iter = _widgets.find(key);

	assert(iter != _widgets.end() && "Widget not found");

	Widget* wid = iter->second;

	return wid;
}

UI::~UI()
{
	Dispatcher::getInstance().Disconnect(EventTypes::updateSpaceshipStateEvent, _tokenUI);
	Dispatcher::getInstance().Disconnect(EventTypes::updateSpaceshipWeaponStorageEvent, _tokenWeaponUI);

	for (auto it : _widgets)
	{
		delete it.second;
	}
	_widgets.clear();
}
