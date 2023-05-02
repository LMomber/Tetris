#pragma once

#include "surface.h"
#include "template.h"
#include "SDL.h"
#include "Timer.hpp"

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
	void drawBlock(Sprite& color);
	int FrameMin();
	int FramePlus();
private:
	Surface* screen;

	SDL_Event event;

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

	int blockFrame{ 0 };
	int blockWidth = 100;
	int blockHeight = 100;
	int blockX{ ScreenWidth / 2 - (blockWidth / 2) };
	int blockY{ ScreenHeight / 2 - (blockHeight / 2) };

	Graphics::Timer blockTimer;

	int elapsedSeconds{ 0 };
	int lastSecond{ 0 };
};

}; // namespace Tmpl8