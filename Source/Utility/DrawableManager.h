#pragma once
#include "Drawable.h"
#include <vector>
#include <algorithm>

class DrawableManager
{
public:
	static DrawableManager& getInstance();

	void SortDrawableVector();
	void DrawScene(sf::RenderWindow& window);
	void AddDrawableObject(Drawable* object);
	void RemoveDrawableObject(Drawable* object);

private:
	DrawableManager(DrawableManager const&) = delete;
	DrawableManager& operator= (DrawableManager const&) = delete;

	DrawableManager() {}
	~DrawableManager() {}

	std::vector<Drawable*> _drawableObjects;
};