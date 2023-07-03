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

	void StartPosition();
	void Update();

	void DrawBlock(int i);
	void DrawOldBlock(int i);
	void DrawPlayingField();

	void GridCollision();
	void AddToGrid();

	bool InBounds(int xPos, int yPos);

	void FixedPosition(int xPos, int yPos);
	void ChangeColor();
	void KeyDown(int key = 0);

	void ShowTestGrid();

private:
	Surface* screen;
	Surface* blue_S = new Surface("assets/blue.png");
	Surface* green_S = new Surface("assets/green.png");
	Surface* lightBlue_S = new Surface("assets/lightBlue.png");
	Surface* orange_S = new Surface("assets/orange.png");
	Surface* yellow_S = new Surface("assets/yellow.png");
	Surface* purple_S = new Surface("assets/purple.png");
	Surface* red_S = new Surface("assets/red.png");
	Surface* colors_S = new Surface("assets/colors.png");

	SDL_Event event;

	Block::Color blockColor = Block::Purple;
	Block::Color testColor = Block::All;

	std::array<std::array<bool, 20>, 10> grid = { false };

	std::vector<Block> blocks;
	std::vector<Sprite> sprites;
	int iterator{ 0 };

	GameState gameState{ Initialize };

	Sprite* blue = new Sprite(blue_S, 4);
	Sprite* green = new Sprite(green_S, 4);
	Sprite* lightBlue = new Sprite(lightBlue_S, 4); 
	Sprite* orange = new Sprite(orange_S, 4); 
	Sprite* yellow = new Sprite(yellow_S, 4);
	Sprite* purple = new Sprite(purple_S, 4);
	Sprite* red = new Sprite(red_S, 1);
	Sprite* colors = new Sprite(colors_S, 7);

	Graphics::Timer blockTimer;

	int elapsedSeconds{ 0 };
	int lastSecond{ 0 };

	float waitTime = 0.5f;

	//Variables for registering mouse clicks
	int mouseX, mouseY;
	int mousePressed;
	int mouseReleased;

	bool up = false, down = false, left = false, right = false;
	bool allowUp = true, allowDown = true, allowLeft = true, allowRight = true; 

	int bottomBorder = ScreenHeight - 50;
	int leftBorder;
	int rightBorder;

	int core_x1, core_x2, core_y1, core_y2; //Coords Core
	int extra_x1, extra_x2, extra_y1, extra_y2; //Coords Extra

	int x11, x22, y11, y22; //Coords Core
	int x33, x44, y33, y44; //Coords Extra

	int xPosCore = 1;
	int yPosCore = 1;

	int xPosExtra = 1;
	int yPosExtra = 1;

	float onGroundTimeStamp{ 0 };
	bool timeStampTaken{ false };
	bool nextBlock{ false };

	vec startPosition;
};

}; // namespace Tmpl8