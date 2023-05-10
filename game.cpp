#include "game.h"
#include <iostream>


namespace Tmpl8
{
	void Game::Init()
	{
	}

	void Game::Shutdown()
	{
	}

	void Game::Tick(float deltaTime)
	{
		switch (gameState) {
		case Initialize:
		{
			blockTimer.reset();
			block.Collider();
			block.SetPosition(startPosition);
			gameState = Playing;
		}
			break;
		case Playing:
		{
			blockTimer.tick();
			elapsedSeconds = static_cast<int>(blockTimer.totalSeconds());

			//Move blocks downwards
			if (elapsedSeconds > lastSecond)
			{
				block.SetPosition({ block.GetPosition().x, block.GetPosition().y + static_cast<int>(block.GetSize() / 3) });
				lastSecond = elapsedSeconds;
			}

			block.WallCollision(leftBorder, rightBorder, bottomBorder);
			GridCollision();

			screen->Clear(0);
			drawBlock();

			//screen->Line(leftBorder, bottomBorder - (block.GetSize() / 3), rightBorder, bottomBorder - (block.GetSize()/3), 0xffffff);
			screen->Line(leftBorder, bottomBorder, rightBorder, bottomBorder, 0xffffff);
			screen->Line(leftBorder, 0, leftBorder, bottomBorder, 0xffffff);
			screen->Line(rightBorder, 0, rightBorder, bottomBorder, 0xffffff);
		}
			break;
		case NextBlock:
		{
			for (int i = 0; i < (block.GetCoreWidth() / block.GetSizeOne()); i++)
			{
				for (int j = 0; j < (block.GetCoreHeight() / block.GetSize()); j++)
				{
					grid[i][j] = true;
				}
			}
		}
			break;
		}
	}
	
	void Game::GridCollision()
	{
		int xPos = (x1 - leftBorder) / block.GetSizeOne(); 
		int yPos; 

		//Calculate the Y-position in the grid
		if (y1 - bottomBorder == 0) yPos = 0;
		else yPos = abs(y1 - bottomBorder) / block.GetSizeOne(); 

		//Check if the grid left of the xPos is true
		if (xPos > 0 && grid[xPos - 1][yPos] == true) allowLeft = false; 
		else allowLeft = true;

		//Check if the grid right of the xPos is true
		if ((xPos < 9 && xPos >= 0) && grid[xPos + 1][yPos] == true) allowRight = false;
		else allowRight = true;

		//Check if the grid under the yPos is true. If so, prevent movement downwards, nextBlock == true;
		if (yPos > 0 && yPos < 19)
		{
			if (grid[xPos][yPos - 1] == true || block.onGround(bottomBorder))
			{
				if (!timeStampTaken)
				{
					onGroundTimeStamp = static_cast<float>(blockTimer.totalSeconds());
					timeStampTaken = true;
				}

				if ((static_cast<float>(blockTimer.totalSeconds()) - onGroundTimeStamp) >= 0.4f)
					gameState = NextBlock;

				block.WallCollision(leftBorder, rightBorder, bottomBorder - (yPos * block.GetSizeOne()));
			}
		}
	}

	void Game::KeyDown(int key)
	{
		if (nextBlock)
		{

		}
		else
		{
			switch (key) {
			case SDL_SCANCODE_W:
			case SDL_SCANCODE_UP:
				if (allowUp)
				{
					block.FramePlus();

					switch (blockColor) {
					case Block::Blue:
						blue.SetFrame(block.GetFrame());
						break;
					case Block::Green:
						green.SetFrame(block.GetFrame());
						break;
					case Block::Red:
						red.SetFrame(block.GetFrame());
						break;
					case Block::Orange:
						orange.SetFrame(block.GetFrame());
						break;
					case Block::Yellow:
						yellow.SetFrame(block.GetFrame());
						break;
					case Block::Purple:
						purple.SetFrame(block.GetFrame());
						break;
					case Block::LightBlue:
						lightBlue.SetFrame(block.GetFrame());
						break;
					}

					block.Collider();
				}
				break;
			case SDL_SCANCODE_S:
			case SDL_SCANCODE_DOWN:
				if (allowDown) block.SetPosition({ block.GetPosition().x, block.GetPosition().y + static_cast<int>(block.GetSize() / 3) });
				break;
			case SDL_SCANCODE_A:
			case SDL_SCANCODE_LEFT:
				if (allowLeft) block.SetPosition({ block.GetPosition().x - static_cast<int>(block.GetSize() / 3), block.GetPosition().y });
				break;
			case SDL_SCANCODE_D:
			case SDL_SCANCODE_RIGHT:
				if (allowRight) block.SetPosition({ block.GetPosition().x + static_cast<int>(block.GetSize() / 3), block.GetPosition().y });
				break;
			}
		}
	}

#ifdef _DEBUG
	void Game::drawBlock()
	{
		switch (blockColor) {
		case Block::Blue:
			blue.DrawScaled(block.GetPosition().x, block.GetPosition().y, block.GetSize(), block.GetSize(), screen);
			break;
		case Block::Green:
			green.DrawScaled(block.GetPosition().x, block.GetPosition().y, block.GetSize(), block.GetSize(), screen);
			break;
		case Block::Red:
			red.DrawScaled(block.GetPosition().x, block.GetPosition().y, block.GetSizeOfRed(), block.GetSizeOfRed(), screen);
			break;
		case Block::Orange:
			orange.DrawScaled(block.GetPosition().x, block.GetPosition().y, block.GetSize(), block.GetSize(), screen);
			break;
		case Block::Yellow:
			yellow.DrawScaled(block.GetPosition().x, block.GetPosition().y, block.GetSize(), block.GetSize(), screen);
			break;
		case Block::Purple:
			purple.DrawScaled(block.GetPosition().x, block.GetPosition().y, block.GetSizeOfPurple(), block.GetSizeOfPurple(), screen);
			break;
		case Block::LightBlue:
			lightBlue.DrawScaled(block.GetPosition().x, block.GetPosition().y, block.GetSize(), block.GetSize(), screen);
			break;
		}

		//Core x1
		x1 = block.GetPosition().x + block.GetCorePos().x;
		//screen->Line(x1, 0, x1, ScreenHeight, 0x00ff00);
		//Core x2
		x2 = block.GetPosition().x + block.GetCorePos().x + block.GetCoreWidth();
		//screen->Line(x2, 0, x2, ScreenHeight, 0x00ff00);
		//Core y1
		y1 = block.GetPosition().y + block.GetCorePos().y;
		//screen->Line(0, y1, ScreenWidth, y1, 0x00ff00);
		//Core y2
		y2 = block.GetPosition().y + block.GetCorePos().y - block.GetCoreHeight();
		//screen->Line(0, y2, ScreenWidth, y2, 0x00ff00);

		//Core Box
		screen->Box(x1, y1, x2, y2, 0x00ff00);

		//Extra x1
		x3 = block.GetPosition().x + block.GetExtraPos().x;
		//screen->Line(x3, 0, x3, ScreenHeight, 0x00ff00);
		//Extra x2
		x4 = block.GetPosition().x + block.GetExtraPos().x + block.GetExtraWidth();
		//screen->Line(x4, 0, x4, ScreenHeight, 0x00ff00);
		//Extra y1
		y3 = block.GetPosition().y + block.GetExtraPos().y;
		//screen->Line(0, y3, ScreenWidth, y3, 0x00ff00);
		//Extra y2
		y4 = block.GetPosition().y + block.GetExtraPos().y - block.GetExtraHeight();
		//screen->Line(0, y4, ScreenWidth, y4, 0x00ff00);

		//Extra Box
		screen->Box(x3, y3, x4, y4, 0xff0000);
	}

#else
	void Game::drawBlock()
	{
		switch (blockColor) {
		case Block::Blue:
			blue.DrawScaled(block.GetPosition().x, block.GetPosition().y, block.GetSize(), block.GetSize(), screen);
			break;
		case Block::Green:
			green.DrawScaled(block.GetPosition().x, block.GetPosition().y, block.GetSize(), block.GetSize(), screen);
			break;
		case Block::Red:
			red.DrawScaled(block.GetPosition().x, block.GetPosition().y, block.GetSizeOfRed(), block.GetSizeOfRed(), screen);
			break;
		case Block::Orange:
			orange.DrawScaled(block.GetPosition().x, block.GetPosition().y, block.GetSize(), block.GetSize(), screen);
			break;
		case Block::Yellow:
			yellow.DrawScaled(block.GetPosition().x, block.GetPosition().y, block.GetSize(), block.GetSize(), screen);
			break;
		case Block::Purple:
			purple.DrawScaled(block.GetPosition().x, block.GetPosition().y, block.GetSizeOfPurple(), block.GetSizeOfPurple(), screen);
			break;
		case Block::LightBlue:
			lightBlue.DrawScaled(block.GetPosition().x, block.GetPosition().y, block.GetSize(), block.GetSize(), screen);
			break;
		}
	}
#endif
};