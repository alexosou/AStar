#include "Player.h"
#include "Game.h"


Player::Player(Game& game, const Position& position, const Target& target) : MapComponent(game, position), target(target), current(0)
{
}


void Player::update(float elapsed)
{
	// Pfad wird je nach Spielgeschwindigkeit aktualisiert
	delta += elapsed * game.getGameSpeed();

	if (delta > 0)
		generatePath(target.position);

	delta -= (int)delta;
}


void Player::draw()
{
	CL_Sizef fieldSize = map.getFieldSize();

	//for (nodeMap::iterator it = todo.begin(); it != todo.end(); ++it)
	//	CL_Draw::circle(window.get_gc(), map.getCenter(it->second.position), 10.0f, CL_Colorf::blue);

	//for (nodeMap::iterator it = finalized.begin(); it != finalized.end(); ++it)
	//	CL_Draw::circle(window.get_gc(), map.getCenter(it->second.position), 10.0f, CL_Colorf::black);

	// wenn Pfad berechnet wurde
	if (current != 0)
	{
		const Node* node = current->next;

		// solange aktuelles Feld ungleich Zielfeld
		while (node->position != target.position)
		{
			// orangen Kreis am aktuellen Feld zeichnen
			CL_Draw::circle(window.get_gc(), map.getCenter(node->position), 10.0f, CL_Colorf::orange);

			// zum nächsten Feld am Pfad gehen
			node = node->next;
		}
	}

	// gelben Kreis an der Position des Spielers zeichnen
	CL_Draw::circle(window.get_gc(), map.getCenter(position), min(fieldSize.width, fieldSize.height) / 3.0f, CL_Colorf::yellow);

	MapComponent::draw();
}


void Player::generatePath(const Position& target)
{
	// todo und finalized liste leeren
	todo.clear();
	finalized.clear();

	// Ziel-Knoten in finalized Liste einfügen
	finalized.insert(mapElement(target, Node(target, 0)));

	// aktuellen Knoten auf Ziel-Knoten setzen
	current = &finalized.at(target);

	// rekursive Funktion aufrufen
	calculateStep(target);
}


void Player::calculateStep(const Position& target)
{
	// Abbruchbedienung (Spieler-Position erreicht)
	if (current != 0 && current->position == position)
		return;

	// Nachbarn des aktuellen Knoten beziehen
	std::list<Node> neighbors = map.getNeighbors(current->position);

	for (std::list<Node>::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
	{
		Node& neighbor = *it;

		// wenn Kosten größer 0 und noch nicht finalized
		if (neighbor.costs > 0 && finalized.find(neighbor.position) == finalized.end())
		{
			// in todo Liste nach Nachbarn suchen
			nodeMap::iterator todoIt = todo.find(neighbor.position);

			// wenn noch nicht in todo Liste oder aktuelle Kosten kleiner als Kosten in todo Liste
			if (todoIt == todo.end() || todoIt->second.costs > neighbor.costs + current->costs)
			{
				// wenn in todo Liste, Nachbarn aus todo Liste löschen
				if (todoIt != todo.end())
					todo.erase(todoIt);

				// Nachfolger auf aktuellen Knoten setzen
				neighbor.next = current;

				// Kosten um bisherige Kosten erhöhen
				neighbor.costs += current->costs;

				// in todo Liste einfügen
				todo.insert(mapElement(neighbor.position, neighbor));
			}
		}
	}

	current = &(todo.begin()->second);
	int currentHeuristic = getHeuristic(current->position, position);

	// in todo Liste nach billigstem Knoten suchen
	for (nodeMap::iterator it = todo.begin(); it != todo.end(); ++it)
	{
		Node& node = it->second;

		// Heuristik berechnen
		int heuristic = getHeuristic(node.position, position);

		// Gesamtkosten vergleichen
		if (node.costs + heuristic < current->costs + currentHeuristic)
		{
			currentHeuristic = heuristic;
			current = &node;
		}
	}

	// Knoten in die finalized List einfügen
	finalized.insert(mapElement(current->position, *current));
	current = &finalized.at(current->position);

	// Knoten aus der todo Liste löschen
	todo.erase(current->position);

	calculateStep(target);
}


// berechnet die Distanz zwischen zwei Feldern (Manhatten Methode)
int Player::getHeuristic(const Position& from, const Position& to) const
{
	return std::abs(from.row - to.row) + std::abs(from.col - to.col);
}