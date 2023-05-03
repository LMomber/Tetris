#pragma once

#include "surface.h"
#include "template.h"
#include "SDL.h"
#include "Timer.hpp"
#include "Block.h"

namespace Tmpl8 {

class Surface;
class Game
{
public:
	void SetTarget( Surface* surface ) { screen = surface; }
	void Init();
	void Shutdown();
	void Tick( float deltaTime );
	void MouseUp( int button ) { mouseReleased = button; if (mouseReleased) mousePressed = 0; }
	void MouseDown( int button ) { mousePressed = button; if (mousePressed) mouseReleased = 0; }
	void MouseMove(int x, int y) { mouseX = x, mouseY = y; }
	void KeyUp( int key = 0 ) { /* implement if you want to handle keys */ }
	void KeyDown( int key = 0 ) { /* implement if you want to handle keys */ }
	void drawBlock();
private:
	Surface* screen;

	SDL_Event event;

	Block block;

	Block::Color blockColor;

	Sprite blue{ new Surface("assets/blue.png"), 4 };
	Sprite green{ new Surface("assets/blue.png"), 4 };
	Sprite lightBlue{ new Surface("assets/blue.png"), 4 };
	Sprite orange{ new Surface("assets/blue.png"), 4 };
	Sprite yellow{ new Surface("assets/blue.png"), 4 };
	Sprite purple{ new Surface("assets/blue.png"), 4 };
	Sprite red{ new Surface("assets/blue.png"), 1 };

	//Variables for registering mouse clicks
	int mouseX, mouseY;
	int mousePressed;
	int mouseReleased;

	Graphics::Timer blockTimer;

	int elapsedSeconds{ 0 };
	int lastSecond{ 0 };
};

}; // namespace Tmpl8