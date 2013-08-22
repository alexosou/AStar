#include "MapComponent.h"
#include "Game.h"

MapComponent::MapComponent(Game& game, const Position& position) 
	: GameComponent(game), map(game.getMap()), position(position), delta(0)
{
}


MapComponent::~MapComponent()
{
}


void MapComponent::draw()
{
}