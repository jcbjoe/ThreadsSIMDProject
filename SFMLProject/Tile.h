#pragma once
#include <SFML/Graphics.hpp>
#include "Config.h"

class Tile : public sf::Drawable
{
private:
	int _x;
	int _y;

	sf::RectangleShape _rect;

	sf::Color _colour;

	// Help for debugging - the bounds of the tile
	int _xMax;
	int _yMax;

public:

	sf::Vector2i GetPosition() { return sf::Vector2i(_x, _y); };

	Tile(int x, int y, sf::Color colour);

	void Update(float dTime);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Color GetColour() { return _colour; };

	void SetColour(sf::Color col) { _colour = col;	_rect.setFillColor(_colour); };
};