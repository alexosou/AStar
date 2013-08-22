#include "Game.h"

Game::Game(CL_DisplayWindow& window) : window(window), quit(false), pause(false)
{
	// Eventhandler anlegen
	slotQuit = window.sig_window_close().connect(this, &Game::onWindowClose);
	slotKeyDown = window.get_ic().get_keyboard().sig_key_down().connect(this, &Game::onKeyDown);

	// Spielkomponenten anlegen
	map = new Map(*this, CL_Size(20, 20));
	target = new Target(*this, Position(18, 18));
	player = new Player(*this, Position(1, 1), *target);

	enemies.push_back(new Enemy(*this, Position(1, 6), Position(18, 12), 100));
	enemies.push_back(new Enemy(*this, Position(18, 12), Position(1, 6), 100));
	enemies.push_back(new Enemy(*this, Position(14, 1), Position(4, 18), 100));
	enemies.push_back(new Enemy(*this, Position(4, 18), Position(14, 1), 100));

	// Spielfeld definieren
	char fields[] = {
		'G', 'G', 'G', 'G', 'G', 'G', 'S', 'G', 'W', 'G', 'G', 'G', 'G', 'G', 'W', 'G', 'G', 'G', 'G', 'G',
			'G', 'G', 'G', 'G', 'G', 'S', 'F', 'F', 'W', 'G', 'G', 'G', 'G', 'W', 'G', 'G', 'G', 'G', 'G', 'G',
		'G', 'G', 'G', 'G', 'G', 'G', 'S', 'F', 'F', 'W', 'G', 'G', 'G', 'G', 'W', 'G', 'G', 'G', 'G', 'G',
			'G', 'G', 'G', 'G', 'G', 'S', 'F', 'F', 'G', 'W', 'G', 'G', 'G', 'G', 'W', 'G', 'G', 'G', 'G', 'G',
		'S', 'S', 'G', 'W', 'W', 'G', 'S', 'G', 'G', 'W', 'G', 'G', 'G', 'G', 'W', 'G', 'G', 'G', 'G', 'G',
			'G', 'S', 'W', 'G', 'W', 'G', 'S', 'G', 'W', 'G', 'G', 'G', 'G', 'W', 'G', 'G', 'G', 'G', 'G', 'G',
		'W', 'W', 'S', 'G', 'W', 'G', 'S', 'G', 'G', 'W', 'G', 'G', 'G', 'G', 'W', 'G', 'G', 'G', 'G', 'G',
			'G', 'S', 'G', 'G', 'W', 'S', 'W', 'W', 'W', 'G', 'G', 'S', 'S', 'S', 'G', 'G', 'G', 'G', 'G', 'G',
		'G', 'S', 'G', 'G', 'G', 'G', 'S', 'G', 'G', 'G', 'G', 'S', 'G', 'W', 'S', 'G', 'G', 'G', 'G', 'G',
			'G', 'S', 'G', 'G', 'G', 'G', 'S', 'S', 'S', 'S', 'S', 'G', 'W', 'G', 'S', 'S', 'G', 'G', 'S', 'S',
		'S', 'S', 'G', 'F', 'F', 'F', 'F', 'G', 'G', 'G', 'G', 'G', 'W', 'G', 'G', 'G', 'S', 'S', 'S', 'G',
			'F', 'F', 'G', 'G', 'F', 'F', 'G', 'G', 'G', 'G', 'G', 'G', 'W', 'G', 'G', 'G', 'G', 'G', 'G', 'G',
		'F', 'F', 'F', 'F', 'G', 'F', 'F', 'G', 'G', 'G', 'G', 'G', 'W', 'G', 'G', 'F', 'F', 'F', 'G', 'G',
			'F', 'G', 'G', 'G', 'F', 'F', 'G', 'G', 'G', 'G', 'G', 'G', 'W', 'G', 'F', 'F', 'F', 'F', 'G', 'G',
		'F', 'G', 'F', 'F', 'F', 'F', 'F', 'G', 'G', 'G', 'G', 'G', 'G', 'W', 'G', 'G', 'G', 'F', 'F', 'F',
			'S', 'S', 'F', 'F', 'F', 'S', 'S', 'S', 'G', 'G', 'G', 'G', 'W', 'G', 'F', 'F', 'G', 'F', 'F', 'F',
		'G', 'G', 'S', 'F', 'S', 'S', 'G', 'G', 'S', 'S', 'G', 'G', 'G', 'W', 'G', 'F', 'F', 'G', 'F', 'F',
			'G', 'G', 'S', 'S', 'G', 'G', 'G', 'G', 'G', 'S', 'G', 'G', 'W', 'G', 'G', 'F', 'F', 'G', 'G', 'G',
		'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'S', 'S', 'S', 'S', 'S', 'F', 'F', 'F', 'G', 'G',
			'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'W', 'G', 'S', 'F', 'F', 'G', 'G', 'G'};

	Map::FieldType fieldType;

	for (int i = 0; i < 400; ++i)
	{
		switch (fields[i])
		{
			case 'G': fieldType = Map::Grass; break;
			case 'W': fieldType = Map::Water; break;
			case 'S': fieldType = Map::Street; break;
			case 'F': fieldType = Map::Forest; break;
		}

		(*map)(i / 20, i % 20) = fieldType;
	}

	// Spielkomponenten in Liste einfügen
	components.push_back(map);

	for (std::vector<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); ++it)
		components.push_back(*it);

	components.push_back(target);
	components.push_back(player);
}


// main loop
void Game::run()
{
	// Spielkomponenten initialisieren
	init();

	while (!quit)
	{
		// Spielkomponenten aktualisieren
		update();

		// Spielkomponenten zeichnen
		draw();

		window.flip(1);
		CL_KeepAlive::process();
	}
}


// Initialisieren der Spielkomponenten
void Game::init()
{	
	for (std::list<GameComponent*>::iterator it = components.begin(); it != components.end(); ++it)
		(*it)->init();
}


// Updaten der Spielkomponenten
void Game::update()
{	
	float elapsed = getElapsedTime();

	if (pause)
		return;

	for (std::list<GameComponent*>::iterator it = components.begin(); it != components.end(); ++it)
		(*it)->update(elapsed);
}


// Zeichnen der Spielkomponenten
void Game::draw()
{
	CL_Draw::fill(window.get_gc(), window.get_viewport(), CL_Colorf::black);

	for(std::list<GameComponent*>::iterator it = components.begin(); it != components.end(); ++it)
		(*it)->draw();
}


// Event für Beenden des Windows
void Game::onWindowClose()
{
	quit = true;
}


// Event für Tastendruck
void Game::onKeyDown(const CL_InputEvent &key, const CL_InputState &state)
{
	switch (key.id)
	{
		case CL_KEY_ESCAPE: quit = true; break;
		case CL_KEY_SPACE: pause = !pause; break;
		case CL_KEY_ENTER: map->showCosts = !map->showCosts; break;
	}
}


// liefert das aktuelle DisplayWindow
CL_DisplayWindow& Game::getDisplayWindow() const
{
	return window;
}


// liefert das Spielfeld
Map& Game::getMap()
{
	return *map;
}


// liefert die Kosten eines bestimmten Feldes
float Game::getCosts(const Position& position) const
{
	// Kosten des Feldtypens auf der Karte abrufen
	float costs = map->getCosts(position);

	if (costs < 0)
		return costs;

	// von Gegnern verursachte Kosten addieren
	for (std::vector<Enemy*>::const_iterator it = enemies.begin(); it != enemies.end(); ++it)
		costs += (*it)->getCosts(position);

	return costs;
}


// liefert die vergangene Zeit seit dem letzten Aufruf
float Game::getElapsedTime() const
{
	static unsigned int lastTime = 0;

	unsigned int newTime = CL_System::get_time();

	if (lastTime == 0)
		lastTime = newTime;

	int deltaTime = (newTime - lastTime);

	lastTime = newTime;

	return deltaTime * 0.001f;
}


// liefert die Geschwindigkeit der Anwendung
float Game::getGameSpeed() const
{
	return GAME_SPEED;
}