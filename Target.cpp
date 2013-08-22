#include "Target.h"
#include "Game.h"


Target::Target(Game& game, const Position& position) : MapComponent(game, position)
{
}


void Target::draw()
{
	CL_Sizef fieldSize = map.getFieldSize();

	CL_Draw::circle(window.get_gc(), map.getCenter(position), min(fieldSize.width, fieldSize.height) / 3.0f, CL_Colorf::yellow);
}