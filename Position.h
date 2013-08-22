#pragma once

struct Position
{
	Position(int row, int col) : row(row), col(col) {}

	bool operator==(const Position& other) const { return row == other.row && col == other.col; }
	bool operator!=(const Position& other) const { return !(*this == other); }
	bool operator<(const Position& other) const { return row < other.row || (row == other.row && col < other.col); }

	int row;
	int col;
};

