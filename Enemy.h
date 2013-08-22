#pragma once

#include "MapComponent.h"

class Enemy : public MapComponent
{
public:
	Enemy(Game& game, const Position& position, const Position& target, float costs);

	void update(float elapsed);
	void draw();

	float getCosts(const Position& position) const;
	Position getNextPosition();
private:
	Position start;
	Position target;
	float costs;

	int getDistance(const Position& from, const Position& to) const;
};

