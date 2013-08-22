#pragma once

#include "Position.h"

struct Node
{
	Node(const Position& position, float costs) : position(position), costs(costs), next(0) {}

	bool operator<(const Node& other) const { return position < other.position; }

	Position position;
	float costs;
	const Node* next;
};

