#include "game.h"
#include <memory>
#include <random>
#include <format>
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
			elapsedSeconds = lastSecond = 0;

			ChangeColor();
			Block newBlock(blockColor);
			blocks.push_back(newBlock);
			blocks[iterator].Collider();

			leftBorder = (ScreenWidth / 2) - (blocks[iterator].GetSizeOne() * 5);
			rightBorder = (ScreenWidth / 2) + (blocks[iterator].GetSizeOne() * 5);
			startPosition = { leftBorder + (blocks[iterator].GetSizeOne() * 3),
					(blocks[iterator].GetCoreHeight() > blocks[iterator].GetExtraHeight() ? blocks[iterator].GetCoreHeight() : blocks[iterator].GetExtraHeight()) };
			blocks[iterator].SetPosition(startPosition);

			gameState = Playing;
		break;
		}
		case Playing:
		{
			blockTimer.tick();
			elapsedSeconds = static_cast<int>(blockTimer.totalSeconds());

			//Move blocks downwards
			if (elapsedSeconds > lastSecond)
			{
				blocks[iterator].SetPosition({ blocks[iterator].GetPosition().x, blocks[iterator].GetPosition().y + static_cast<int>(blocks[iterator].GetSize() / 3) });
				lastSecond = elapsedSeconds;
			}

			Update();

			blocks[iterator].WallCollision(leftBorder, rightBorder, bottomBorder);
			GridCollision();

			screen->Clear(0);
			//Draw sprites
			for (int i = 0; i <= iterator; i++)
			{
				DrawBlock(i);
			}

			//Draw borders
			screen->Line(static_cast<float>(leftBorder), static_cast<float>(bottomBorder), static_cast<float>(rightBorder), static_cast<float>(bottomBorder), 0xffffff);
			screen->Line(static_cast<float>(leftBorder), 0, static_cast<float>(leftBorder), static_cast<float>(bottomBorder), 0xffffff);
			screen->Line(static_cast<float>(rightBorder), 0, static_cast<float>(rightBorder), static_cast<float>(bottomBorder), 0xffffff);
		break;
		}
		case NextBlock:
		{
			//Sets occupation bool for the core-blocks
			for (int x = 0; x < (blocks[iterator].GetCoreWidth() / blocks[iterator].GetSizeOne()); x++)
			{
				for (int y = 0; y < (blocks[iterator].GetCoreHeight() / blocks[iterator].GetSizeOne()); y++)
				{
					//If inside the borders of the playing field
					if ((xPosCore + x < grid.size()) && (yPosCore + y < grid[x].size())) grid[xPosCore + x][yPosCore + y] = true;
				}
			}

			//Sets occupation bool for the extra-blocks
			for (int x = 0; x < (blocks[iterator].GetExtraWidth() / blocks[iterator].GetSizeOne()); x++)
			{
				for (int y = 0; y < (blocks[iterator].GetExtraHeight() / blocks[iterator].GetSizeOne()); y++)
				{
					//If inside the borders of the playing field
					if ((xPosExtra + x < grid.size()) && (yPosExtra + y < grid[x].size())) grid[xPosExtra + x][yPosExtra + y] = true;
				}
			}

			//std::cout << xPos << "    " << yPos << std::endl;
			//std::cout << blocks[iterator].GetCoreHeight() / blocks[iterator].GetSize() << std::endl;

			for(int j = 0; j < 5; j++)
			{
				for (int i = 0; i < 10; i++)
				{
					std::cout << std::format("{:^7}|", grid[i][4-j]);
				}

				std::cout << std::endl;
			}

			iterator++;
			gameState = Initialize;
		break;
		}
		}
	}

	void Game::GridCollision()
	{
		//Calculate the Y-position in the grid
		if (y1 - bottomBorder == 0) yPosCore = 0;
		else yPosCore = abs(y1 - bottomBorder) / blocks[iterator].GetSizeOne();

		//Check if the grid left of the xPos is true
		if (yPosCore < 20 && xPosCore > 0 && grid[xPosCore - 1][yPosCore] == true) allowLeft = false;
		else allowLeft = true;

		//Check if the grid right of the xPos is true
		if (yPosCore < 20 && (xPosCore < 9 && xPosCore >= 0) && grid[xPosCore + 1][yPosCore] == true) allowRight = false;
		else allowRight = true;

		//Check if the grid under the yPos is true. If so, prevent movement downwards, nextBlock == true;
		if ((xPosCore >= 0 && yPosCore > 0 && yPosCore < 19) && grid[xPosCore][yPosCore - 1] == true)
		{
			if (!timeStampTaken)
			{
				onGroundTimeStamp = static_cast<float>(blockTimer.totalSeconds());
				timeStampTaken = true;
			}

			if ((static_cast<float>(blockTimer.totalSeconds()) - onGroundTimeStamp) >= 0.4f)
			{
				timeStampTaken = false;
				gameState = NextBlock;
			}

			blocks[iterator].WallCollision(leftBorder, rightBorder, bottomBorder - (yPosCore * blocks[iterator].GetSizeOne()));
		}
		else if (yPosCore == 0)
		{
			if (!timeStampTaken)
			{
				onGroundTimeStamp = static_cast<float>(blockTimer.totalSeconds());
				timeStampTaken = true;
			}

			if ((static_cast<float>(blockTimer.totalSeconds()) - onGroundTimeStamp) >= 0.4f)
			{
				timeStampTaken = false;
				gameState = NextBlock;
			}
		}
	}

	void Game::ChangeColor()
	{
		std::mt19937 rng(std::random_device{}());
		std::uniform_int_distribution<int> dist(0, 6);

		int color = dist(rng);

		switch (color) {
		case 0:
			blockColor = Block::Blue;
			break;
		case 1:
			blockColor = Block::Green;
			break;
		case 2:
			blockColor = Block::LightBlue;
			break;
		case 3:
			blockColor = Block::Orange;
			break;
		case 4:
			blockColor = Block::Purple;
			break;
		case 5:
			blockColor = Block::Red;
			break;
		case 6:
			blockColor = Block::Yellow;
			break;
		}
	}

	void Game::Update()
	{
		x1 = blocks[iterator].GetPosition().x + blocks[iterator].GetCorePos().x;  
		y1 = blocks[iterator].GetPosition().y + blocks[iterator].GetCorePos().y;  

		xPosCore = (x1 - leftBorder) / blocks[iterator].GetSizeOne();  

		x3 = blocks[iterator].GetPosition().x + blocks[iterator].GetExtraPos().x; 
		y3 = blocks[iterator].GetPosition().y + blocks[iterator].GetExtraPos().y;

		xPosExtra = (x3 - leftBorder) / blocks[iterator].GetSizeOne();
	}

	void Game::KeyDown(int key)
	{
		switch (key) {
		case SDL_SCANCODE_W:
		case SDL_SCANCODE_UP:
			if (allowUp)
			{
				blocks[iterator].FramePlus();
				blocks[iterator].Collider();
			}
			break;
		case SDL_SCANCODE_S:
		case SDL_SCANCODE_DOWN:
			if (allowDown) blocks[iterator].SetPosition({ blocks[iterator].GetPosition().x, blocks[iterator].GetPosition().y + static_cast<int>(blocks[iterator].GetSize() / 3) });
			break;
		case SDL_SCANCODE_A:
		case SDL_SCANCODE_LEFT:
			if (allowLeft) blocks[iterator].SetPosition({ blocks[iterator].GetPosition().x - static_cast<int>(blocks[iterator].GetSize() / 3), blocks[iterator].GetPosition().y });
			break;
		case SDL_SCANCODE_D:
		case SDL_SCANCODE_RIGHT:
			if (allowRight) blocks[iterator].SetPosition({ blocks[iterator].GetPosition().x + static_cast<int>(blocks[iterator].GetSize() / 3), blocks[iterator].GetPosition().y });
			break;
		}
	}

#ifdef _DEBUG
	void Game::DrawBlock(int i)
	{
		switch (blocks[i].GetColor()) {
		case Block::Blue:
			blue->DrawScaled(blocks[i].GetPosition().x, blocks[i].GetPosition().y, blocks[i].GetSize(), blocks[i].GetSize(), screen, blocks[i].GetFrame());
			break;
		case Block::Green:
			green->DrawScaled(blocks[i].GetPosition().x, blocks[i].GetPosition().y, blocks[i].GetSize(), blocks[i].GetSize(), screen, blocks[i].GetFrame());
			break;
		case Block::Red:
			red->DrawScaled(blocks[i].GetPosition().x, blocks[i].GetPosition().y, blocks[i].GetSizeOfRed(), blocks[i].GetSizeOfRed(), screen, blocks[i].GetFrame());
			break;
		case Block::Orange:
			orange->DrawScaled(blocks[i].GetPosition().x, blocks[i].GetPosition().y, blocks[i].GetSize(), blocks[i].GetSize(), screen, blocks[i].GetFrame());
			break;
		case Block::Yellow:
			yellow->DrawScaled(blocks[i].GetPosition().x, blocks[i].GetPosition().y, blocks[i].GetSize(), blocks[i].GetSize(), screen, blocks[i].GetFrame());
			break;
		case Block::Purple:
			purple->DrawScaled(blocks[i].GetPosition().x, blocks[i].GetPosition().y, blocks[i].GetSizeOfPurple(), blocks[i].GetSizeOfPurple(), screen, blocks[i].GetFrame());
			break;
		case Block::LightBlue:
			lightBlue->DrawScaled(blocks[i].GetPosition().x, blocks[i].GetPosition().y, blocks[i].GetSize(), blocks[i].GetSize(), screen, blocks[i].GetFrame());
			break;
		}

		/***************************************
			
			Code for drawing box-collisions
		
		****************************************/

		//Core x1
		x11 = blocks[i].GetPosition().x + blocks[i].GetCorePos().x;
		//screen->Line(x11, 0, x11, ScreenHeight, 0x00ff00);
		//Core x2
		x22 = blocks[i].GetPosition().x + blocks[i].GetCorePos().x + blocks[i].GetCoreWidth();
		//screen->Line(x22, 0, x22, ScreenHeight, 0x00ff00);
		//Core y1
		y11 = blocks[i].GetPosition().y + blocks[i].GetCorePos().y;
		//screen->Line(0, y11, ScreenWidth, y11, 0x00ff00);
		//Core y2
		y22 = blocks[i].GetPosition().y + blocks[i].GetCorePos().y - blocks[i].GetCoreHeight();
		//screen->Line(0, y22, ScreenWidth, y22, 0x00ff00);

		//Core Box
		screen->Box(x11, y11, x22, y22, 0x00ff00);

		//Extra x1
		x33 = blocks[i].GetPosition().x + blocks[i].GetExtraPos().x;
		//screen->Line(x33, 0, x33, ScreenHeight, 0x00ff00);
		//Extra x2
		x44 = blocks[i].GetPosition().x + blocks[i].GetExtraPos().x + blocks[i].GetExtraWidth();
		//screen->Line(x4, 0, x44, ScreenHeight, 0x00ff00);
		//Extra y1
		y33 = blocks[i].GetPosition().y + blocks[i].GetExtraPos().y;
		//screen->Line(0, y33, ScreenWidth, y33, 0x00ff00);
		//Extra y2
		y44 = blocks[i].GetPosition().y + blocks[i].GetExtraPos().y - blocks[i].GetExtraHeight();
		//screen->Line(0, y44, ScreenWidth, y44, 0x00ff00);

		//Extra Box
		screen->Box(x33, y33, x44, y44, 0xff0000);
	}

#else
	void Game::drawBlock(int i)
	{
		switch (blocks[i].GetColor()) {
		case Block::Blue:
			blue->DrawScaled(blocks[i].GetPosition().x, blocks[i].GetPosition().y, blocks[i].GetSize(), blocks[i].GetSize(), screen, blocks[i].GetFrame());
			break;
		case Block::Green:
			green->DrawScaled(blocks[i].GetPosition().x, blocks[i].GetPosition().y, blocks[i].GetSize(), blocks[i].GetSize(), screen, blocks[i].GetFrame());
			break;
		case Block::Red:
			red->DrawScaled(blocks[i].GetPosition().x, blocks[i].GetPosition().y, blocks[i].GetSizeOfRed(), blocks[i].GetSizeOfRed(), screen, blocks[i].GetFrame());
			break;
		case Block::Orange:
			orange->DrawScaled(blocks[i].GetPosition().x, blocks[i].GetPosition().y, blocks[i].GetSize(), blocks[i].GetSize(), screen, blocks[i].GetFrame());
			break;
		case Block::Yellow:
			yellow->DrawScaled(blocks[i].GetPosition().x, blocks[i].GetPosition().y, blocks[i].GetSize(), blocks[i].GetSize(), screen, blocks[i].GetFrame());
			break;
		case Block::Purple:
			purple->DrawScaled(blocks[i].GetPosition().x, blocks[i].GetPosition().y, blocks[i].GetSizeOfPurple(), blocks[i].GetSizeOfPurple(), screen, blocks[i].GetFrame());
			break;
		case Block::LightBlue:
			lightBlue->DrawScaled(blocks[i].GetPosition().x, blocks[i].GetPosition().y, blocks[i].GetSize(), blocks[i].GetSize(), screen, blocks[i].GetFrame());
			break;
		}
	}
#endif
};