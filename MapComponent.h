#pragma once

#include "GameComponent.h"
#include "Map.h"
#include "Position.h"

class MapComponent : public GameComponent
{
public:
	MapComponent(Game& game, const Position& position);
	virtual ~MapComponent() = 0;

	virtual void draw();

	Position position;
protected:
	Map& map;

	float delta;
};

