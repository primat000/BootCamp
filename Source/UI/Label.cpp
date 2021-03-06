#include "Label.h"



Label::Label(const std::string & content, const sf::Font & font, const sf::Vector2f position, const std::string & name,
	sf::RenderWindow & owner) :
	Widget(name, position, owner),
	_content(content, font, 32),
	_isVisible(true)
{
	_content.setOrigin(_content.getLocalBounds().width / 2, _content.getLocalBounds().height / 2);
}
//
//Setter
//
void Label::SetFillColor(const sf::Color & color)
{
	_content.setFillColor(color);
}

void Label::SetOutlineColor(const sf::Color & color)
{
	_content.setOutlineColor(color);
}

void Label::SetFont(const sf::Font & font)
{
	_content.setFont(font);
}

void Label::SetCharSize(const unsigned int size)
{
	_content.setCharacterSize(size);
}

void Label::SetStyle(sf::Text::Style style)
{
	_content.setStyle(style);
}

void Label::SetString(const std::string content)
{
  	_content.setString(content);
}

//
//Getter
//

sf::Color Label::GetFillColor() const
{
	return _content.getFillColor();
}

sf::Color Label::GetOutlineColor() const
{
	return _content.getOutlineColor();
}

const sf::Font * Label::GetFont() const
{
	return _content.getFont();
}

unsigned int Label::GetCharSize() const
{
	return _content.getCharacterSize();
}

sf::Text::Style Label::GetStyle() const
{
	return sf::Text::Style(_content.getStyle());
}

const std::string & Label::GetContent() const
{
	return _content.getString();
}
//
//Resize
//
void Label::OnResize()
{
	_content.setScale(GetScale());
}

void Label::Draw()
{
	if (_isVisible)
	{
		_content.setPosition(GetPosition());
		_window.draw(_content);
	}
}


Label::~Label()
{
}
