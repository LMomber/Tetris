#pragma once

#include "surface.h"
#include "template.h"
#include "SDL.h"
#include "Timer.hpp"
#include "Block.h"
#include <vector>
#include <array>

namespace Tmpl8 {

class Surface;
class Game
{
public:
	enum GameState {
		Initialize,
		Playing,
		NextBlock
	};

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
	void GridCollision();
private:
	Surface* screen;

	SDL_Event event;

	Block::Color blockColor = Block::Purple;
	Block block{ blockColor };

	GameState gameState{ Initialize };

	std::array<std::array<bool, 20>, 10> grid = { false };

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

	bool up = false, down = false, left = false, right = false;
	bool allowUp = true, allowDown = true, allowLeft = true, allowRight = true; 

	int bottomBorder = ScreenHeight - 50;
	int leftBorder = (ScreenWidth / 2) - (block.GetSizeOne() * 5);
	int rightBorder = (ScreenWidth / 2) + (block.GetSizeOne() * 5);

	int x1, x2, y1, y2; //Coords Core
	int x3, x4, y3, y4; //Coords Extra

	float onGroundTimeStamp{ 0 };
	bool timeStampTaken{ false };
	bool nextBlock{ false };

	vec startPosition = { leftBorder + (block.GetSizeOne() * 3), 
		(block.GetCoreHeight() > block.GetExtraHeight() ? block.GetCoreHeight() : block.GetExtraHeight())};
};

}; // namespace Tmpl8