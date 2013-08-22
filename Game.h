#pragma once

#include "Include.h"
#include "GameComponent.h"
#include "Map.h"
#include "Player.h"
#include "Target.h"
#include "Enemy.h"

#include <list>

#define GAME_SPEED 2

class Game
{
public:
	Game(CL_DisplayWindow& display_window);
	void run();

	Map& getMap();

	CL_DisplayWindow& getDisplayWindow() const;
	float getCosts(const Position& position) const;
	float getGameSpeed() const;
private:
	void init();
	void draw();
	void update();

	void onKeyDown(const CL_InputEvent &key, const CL_InputState &state);
	void onWindowClose();

	float getElapsedTime() const;

	CL_DisplayWindow& window;

	bool quit;
	bool pause;

	CL_Slot slotQuit;
	CL_Slot slotKeyDown;

	std::list<GameComponent*> components;

	Map* map;
	Player* player;
	Target* target;
	std::vector<Enemy*> enemies;
};

