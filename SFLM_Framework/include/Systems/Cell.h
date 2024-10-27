#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include "Core/ECB/GridEntity.h"
#include "Systems/GridCoordinate.h"

constexpr int LEFT_NEIGHBOR   = 0;
constexpr int RIGHT_NEIGHBOR  = 1;
constexpr int TOP_NEIGHBOR    = 2;
constexpr int BOTTOM_NEIGHBOR = 3;

struct Cell
{
	GridCoordinate GridCoordinate;
	Cell* Neighbors[4];
	sf::RectangleShape Tile;
	Core::ECB::Entity* CurrentEntity = nullptr;
};
