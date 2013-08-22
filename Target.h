#pragma once

#include "MapComponent.h"

class Target : public MapComponent
{
public:
	Target(Game& game, const Position& position);

	void draw();
};

