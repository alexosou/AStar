#include "Map.h"
#include "Game.h"

Map::Map(Game& game, const CL_Size& size) : GameComponent(game), size(size), offset(CL_Sizef(10.0f, 10.0f)), showCosts(false)
{
	// Felder initialisieren
	fields = std::vector<FieldType>(size.width * size.height, Grass);

	// Punkte für die Darstellung eines Hex-Feldes initialisieren
	hexPoints[0] = CL_Pointf(0.0f, 2.0f);
	hexPoints[1] = CL_Pointf(1.75f, 1.0f);
	hexPoints[2] = CL_Pointf(1.75f, -1.0f);
	hexPoints[3] = CL_Pointf(0.0f, -2.0f);
	hexPoints[4] = CL_Pointf(-1.75f,	-1.0f);
	hexPoints[5] = CL_Pointf(-1.75f,	1.0f);

	for (int i = 0; i < 6; ++i)
		hexPoints[i] = CL_Pointf(hexPoints[i].x / 3.5f, hexPoints[i].y / 4.0f);
}


void Map::init()
{
	// Feldgröße berechnen
	CL_Rect viewport = window.get_viewport();

	fieldSize.width = (viewport.get_width() - 2.0f * offset.width) / (size.width + 0.5f);
	fieldSize.height = (viewport.get_height() - 2.0f * offset.height) / (size.height * 0.75f + 0.25f);
}


void Map::draw()
{
	// Spielfeld zeichnen
	for (int row = 0; row < size.height; ++row)
		for (int col = 0; col < size.width; ++col)
			draw_field(window.get_gc(), getCenter(Position(row, col)), fieldSize, getColor(Position(row, col)));
}


void Map::draw_field(CL_GraphicContext& gc, const CL_Pointf& center, const CL_Sizef& size, const CL_Colorf& color) const
{
	// für jeden Punkt am Hex-Feld
	for (int i = 0; i < 6; ++i)
	{
		// Positionen von aktuellem und nächstem Punkt berechnen
		CL_Pointf start = CL_Pointf(hexPoints[i].x * size.width, hexPoints[i].y * size.height);
		CL_Pointf end = CL_Pointf(hexPoints[(i+1)%6].x * size.width, hexPoints[(i+1)%6].y * size.height);

		// Dreieck zwischen Zentrum, aktuellem und nächstem Punkt zeichnen
		CL_Draw::triangle(gc, center + start, center + end, center, color);

		// Linie zwischen aktuellem und nächstem Punkt zeichnen
		CL_Draw::line(gc, center + start, center + end, CL_Colorf::white);
	}
}


// Zugriffs-Operator
Map::FieldType& Map::operator()(int row, int col)
{
	return fields[getIndex(Position(row, col))];
}


// Berechnung des Indexes im Vektor
int Map::getIndex(const Position& position) const
{
	return position.row * size.width + position.col;
}


// liefert die Koordinaten eines Feldes
CL_Pointf Map::getCenter(const Position& position) const
{
	CL_Pointf center;
	int col = position.col;
	int row = position.row;

	center.x = offset.width + col * fieldSize.width + fieldSize.width / 2.0f + (row % 2) * fieldSize.width / 2.0f;
	center.y = offset.height + row * fieldSize.height * 0.75f + fieldSize.height / 2.0f;

	return center;
}


// liefert die Farbe eines Feldes
CL_Colorf Map::getColor(const Position& position) const
{
	// wenn Kosten anzeigt werden sollen
	if (showCosts)
	{
		// Kosten vom Spiel abrufen
		float costs = game.getCosts(position);

		// Rotwert abhängig von Kosten zurückliefern
		return (costs >= 0) ? CL_Colorf(costs / 20.0f, 0, 0) : CL_Colorf::red;
	}

	// je nach Feldtyp Farbe zurückliefern
	switch (fields[getIndex(position)])
	{
		case Water: return CL_Colorf::blue;
		case Street: return CL_Colorf::gray;
		case Grass: return CL_Colorf::green;
		case Forest: return CL_Colorf(0.0f, 0.3f, 0.0f);
		default: return CL_Colorf::black;
	}
}


// liefert die Feldgröße
CL_Sizef Map::getFieldSize() const
{
	return fieldSize;
}


// liefert die Kosten je nach Feldtyp
float Map::getCosts(const Position& position) const
{
	switch (fields[getIndex(position)])
	{
		case Water: return -1;
		case Street: return 1;
		case Grass: return 3;
		case Forest: return 10;
		default: return 0;
	}
}


// liefert die Nachbarfelder
std::list<Node> Map::getNeighbors(const Position& position) const
{
	int row = position.row;
	int col = position.col;

	int offset = row % 2;

	std::list<Node> neighbors;

	// links-oben
	if (checkPosition(row - 1, col - 1 + offset))
		neighbors.push_back(createNode(Position(row - 1, col - 1 + offset)));

	// rechts-oben
	if (checkPosition(row - 1, col + offset))
		neighbors.push_back(createNode(Position(row - 1, col + offset)));

	// rechts
	if (checkPosition(row, col + 1))
		neighbors.push_back(createNode(Position(row, col + 1)));
	
	// rechts-unten
	if (checkPosition(row + 1, col + offset))
		neighbors.push_back(createNode(Position(row + 1, col + offset)));

	// links-unten
	if (checkPosition(row + 1, col - 1 + offset))
		neighbors.push_back(createNode(Position(row + 1, col - 1 + offset)));
	
	// links
	if (checkPosition(row, col - 1))
		neighbors.push_back(createNode(Position(row, col - 1)));

	return neighbors;
}


// überprüft, ob Position am Spielfeld ist
bool Map::checkPosition(int row, int col) const
{
	return row >= 0 && row < size.height && col >= 0 && col < size.width;
}


// erzeugt einen Knoten (Position + Kosten)
Node Map::createNode(const Position& position) const
{
	return Node(position, game.getCosts(position));
}