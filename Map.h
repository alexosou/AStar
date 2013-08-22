#pragma once

#include "GameComponent.h"
#include "Position.h"
#include "Node.h"
#include "Include.h"

#include <list>

class Map : public GameComponent
{
public:
	enum FieldType { Water, Grass, Street, Forest };

	Map(Game& game, const CL_Size& size);

	FieldType& operator()(int row, int col);

	void init();
	void draw();
	
	CL_Pointf getCenter(const Position& position) const;
	CL_Colorf getColor(const Position& position) const;
	CL_Sizef getFieldSize() const;
	float getCosts(const Position& position) const;
	std::list<Node> getNeighbors(const Position& position) const;

	bool showCosts;
private:
	int getIndex(const Position& position) const;
	void draw_field(CL_GraphicContext& gc, const CL_Pointf& center, const CL_Sizef& size, const CL_Colorf& color) const;
	Node createNode(const Position& position) const;
	bool checkPosition(int row, int col) const;

	std::vector<FieldType> fields;

	CL_Size size;
	CL_Sizef fieldSize;
	CL_Sizef offset;

	CL_Pointf hexPoints[6];
};

