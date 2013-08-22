#pragma once

#include "MapComponent.h"
#include "Target.h"
#include "Node.h"

#include <set>

class Player : public MapComponent
{
public:
	Player(Game& game, const Position& position, const Target& target);

	void update(float elapsed);
	void draw();
private:
	typedef std::pair<Position, Node> mapElement;
	typedef std::map<Position, Node> nodeMap;

	void generatePath(const Position& target);
	void calculateStep(const Position& target);
	int getHeuristic(const Position& from, const Position& to) const;
	
	nodeMap todo;
	nodeMap finalized;
	const Node* current;

	const Target& target;
};

