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
	void KeyUp(int key = 0) {/*Up for implementation*/ }
	void KeyDown(int key = 0);
	void drawBlock();
private:
	Surface* screen;

	SDL_Event event;

	Block::Color blockColor = Block::Purple;
	Block block{ blockColor };

	Sprite blue{ new Surface("assets/blue.png"), 4 };
	Sprite green{ new Surface("assets/green.png"), 4 };
	Sprite lightBlue{ new Surface("assets/lightblue.png"), 4 };
	Sprite orange{ new Surface("assets/orange.png"), 4 };
	Sprite yellow{ new Surface("assets/yellow.png"), 4 };
	Sprite purple{ new Surface("assets/purple.png"), 4 };
	Sprite red{ new Surface("assets/red.png"), 1 };

	Graphics::Timer blockTimer;

	int elapsedSeconds{ 0 };
	int lastSecond{ 0 };

	//Variables for registering mouse clicks
	int mouseX, mouseY;
	int mousePressed;
	int mouseReleased;

	bool up, down, left, right{ false };

	int bottomBorder = ScreenHeight - 60;
	int leftBorder = (ScreenWidth / 2) - (block.GetSizeOne() * 5);
	int rightBorder = (ScreenWidth / 2) + (block.GetSizeOne() * 5);

	float onGroundTimeStamp{ 0 };
	bool timeStampTaken{ false };
	bool moveLock{ false };
};

}; // namespace Tmpl8