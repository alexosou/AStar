#include "Enemy.h"
#include "Game.h"


Enemy::Enemy(Game& game, const Position& position, const Position& target, float costs) 
	: MapComponent(game, position), start(position), target(target), costs(costs)
{
}


void Enemy::update(float elapsed)
{
	// vergangene Zeit erhöhen
	delta += elapsed * game.getGameSpeed();

	// solange vergangene Zeit größer oder gleich eine Sekunde
	for (; delta >= 1.0f; delta -= 1.0f)
	{
		// zum nächsten Feld bewegen
		position = getNextPosition();		

		// wenn Zielfeld erreicht
		if (position == target)
		{
			// Start und Ziel vertauschen
			target = start;
			start = position;
		}
	}
}


// berechnet die nächste Position am Spielfeld
Position Enemy::getNextPosition()
{
	Position nextPosition = position;

	// wenn Zeile ungleich Ziel-Zeile
	if (nextPosition.row != target.row)
		// Zeile in Richtung Ziel verändern
		nextPosition.row += (nextPosition.row < target.row) ? 1 : -1;
	// wenn Spalte ungleich Ziel-Spalte
	else if (nextPosition.col != target.col)
		// Spalte in Richtung Ziel verändern
		nextPosition.col += (nextPosition.col < target.col) ? 1 : -1;

	return nextPosition;
}


void Enemy::draw()
{
	CL_Sizef fieldSize = map.getFieldSize();

	// weißen Kreis an aktueller Position zeichnen
	CL_Draw::circle(window.get_gc(), map.getCenter(position), min(fieldSize.width, fieldSize.height) / 3.0f, CL_Colorf::white);

	MapComponent::draw();
}


// liefert die Kosten, die durch den Gegner verursacht werden
float Enemy::getCosts(const Position& position) const
{
	// costs / ((distanz + 1) ^ 3) -> exponentieller Abfall im Umkreis
	return costs / pow(getDistance(position, this->position) + 1.0f, 3);
}


// berechnet den Abstand zwischen zwei Feldern
int Enemy::getDistance(const Position& from, const Position& to) const
{
	return std::abs(from.row - to.row) + std::abs(from.col - to.col);
}