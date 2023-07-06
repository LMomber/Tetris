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

			StartPosition();

			gameState = Playing;
		break;
		}
		case Playing:
		{
			blockTimer.tick();
			elapsedSeconds = static_cast<int>(blockTimer.totalSeconds());

			if (!timeStampTaken)
			{
				//Move blocks downwards
				if (elapsedSeconds > lastSecond)
				{
					blocks[iterator].SetPosition({ blocks[iterator].GetPosition().x, blocks[iterator].GetPosition().y + static_cast<int>(blocks[iterator].GetSize() / 3) });
					lastSecond = elapsedSeconds;
#ifdef _DEBUG
					ShowTestGrid();
#endif
				}
			}

			Update();
#ifdef _DEBUG
			std::cout << xPosCore << " , " << yPosCore << std::endl;
#endif
			blocks[iterator].WallCollision(leftBorder, rightBorder, bottomBorder);
			GridCollision();

			screen->Clear(0); 
			DrawPlayingField(); 

			//Draw sprites
			DrawBlock(); 
			DrawOldBlock(); 

		break;
		}
		case NextBlock:
		{
			AddToGrid();

			iterator++;
			gameState = Initialize;
		break;
		}
		}
	}

	void Game::StartPosition()
	{
		int startY = 0;

		leftBorder = (ScreenWidth / 2) - (blocks[iterator].GetSizeOne() * 5);
		rightBorder = (ScreenWidth / 2) + (blocks[iterator].GetSizeOne() * 5);
		startPosition = { leftBorder + (blocks[iterator].GetSizeOne() * 3), startY };
		blocks[iterator].SetPosition(startPosition);
	}

	void Game::Update()
	{
		core_x1 = blocks[iterator].GetPosition().x + blocks[iterator].GetCorePos().x;
		core_y1 = blocks[iterator].GetPosition().y + blocks[iterator].GetCorePos().y;

		xPosCore = (core_x1 - leftBorder) / blocks[iterator].GetSizeOne();

		extra_x1 = blocks[iterator].GetPosition().x + blocks[iterator].GetExtraPos().x;
		extra_y1 = blocks[iterator].GetPosition().y + blocks[iterator].GetExtraPos().y;

		xPosExtra = (extra_x1 - leftBorder) / blocks[iterator].GetSizeOne();
	}

	void Game::DrawBlock()
	{
#ifdef _DEBUG
#else
		std::cout << blocks[iterator].GetPosition().y << std::endl;
#endif

		if (blocks[iterator].GetPosition().y % 10 != 0)
		{
			blocks[iterator].SetPosition({ blocks[iterator].GetPosition().x, blocks[iterator].GetPosition().y + blocks[iterator].GetSizeOne() - 1});
		}

		switch (blocks[iterator].GetColor()) {
		case Block::Blue:
			blue->DrawScaled(blocks[iterator].GetPosition().x, blocks[iterator].GetPosition().y, blocks[iterator].GetSize(), blocks[iterator].GetSize(), screen, blocks[iterator].GetFrame());
			break;
		case Block::Green:
			green->DrawScaled(blocks[iterator].GetPosition().x, blocks[iterator].GetPosition().y, blocks[iterator].GetSize(), blocks[iterator].GetSize(), screen, blocks[iterator].GetFrame());
			break;
		case Block::Red:
			red->DrawScaled(blocks[iterator].GetPosition().x, blocks[iterator].GetPosition().y, blocks[iterator].GetSizeOfRed(), blocks[iterator].GetSizeOfRed(), screen, blocks[iterator].GetFrame());
			break;
		case Block::Orange:
			orange->DrawScaled(blocks[iterator].GetPosition().x, blocks[iterator].GetPosition().y, blocks[iterator].GetSize(), blocks[iterator].GetSize(), screen, blocks[iterator].GetFrame());
			break;
		case Block::Yellow:
			yellow->DrawScaled(blocks[iterator].GetPosition().x, blocks[iterator].GetPosition().y, blocks[iterator].GetSize(), blocks[iterator].GetSize(), screen, blocks[iterator].GetFrame());
			break;
		case Block::Purple:
			purple->DrawScaled(blocks[iterator].GetPosition().x, blocks[iterator].GetPosition().y, blocks[iterator].GetSizeOfPurple(), blocks[iterator].GetSizeOfPurple(), screen, blocks[iterator].GetFrame());
			break;
		case Block::LightBlue:
			lightBlue->DrawScaled(blocks[iterator].GetPosition().x, blocks[iterator].GetPosition().y, blocks[iterator].GetSize(), blocks[iterator].GetSize(), screen, blocks[iterator].GetFrame());
			break;
		case Block::All:
			colors->DrawScaled(blocks[iterator].GetPosition().x, blocks[iterator].GetPosition().y, blocks[iterator].GetSizeOne(), blocks[iterator].GetSizeOne(), screen, blocks[iterator].GetFrame());
			break;
		}
		/*switch (blocks[iterator].GetColor()) {
		case Block::Blue:
			blue->DrawScaled(blocks[iterator].GetPosition().x, blocks[iterator].GetPosition().y, blocks[iterator].GetSize(), blocks[iterator].GetSize(), screen, blocks[iterator].GetFrame());
			break;
		case Block::Green:
			green->DrawScaled(blocks[iterator].GetPosition().x, blocks[iterator].GetPosition().y, blocks[iterator].GetSize(), blocks[iterator].GetSize(), screen, blocks[iterator].GetFrame());
			break;
		case Block::Red:
			red->DrawScaled(blocks[iterator].GetPosition().x, blocks[iterator].GetPosition().y, blocks[iterator].GetSizeOfRed(), blocks[iterator].GetSizeOfRed(), screen, blocks[iterator].GetFrame());
			break;
		case Block::Orange:
			orange->DrawScaled(blocks[iterator].GetPosition().x, blocks[iterator].GetPosition().y, blocks[iterator].GetSize(), blocks[iterator].GetSize(), screen, blocks[iterator].GetFrame());
			break;
		case Block::Yellow:
			yellow->DrawScaled(blocks[iterator].GetPosition().x, blocks[iterator].GetPosition().y, blocks[iterator].GetSize(), blocks[iterator].GetSize(), screen, blocks[iterator].GetFrame());
			break;
		case Block::Purple:
			purple->DrawScaled(blocks[iterator].GetPosition().x, blocks[iterator].GetPosition().y, blocks[iterator].GetSizeOfPurple(), blocks[iterator].GetSizeOfPurple(), screen, blocks[iterator].GetFrame());
			break;
		case Block::LightBlue:
			lightBlue->DrawScaled(blocks[iterator].GetPosition().x, blocks[iterator].GetPosition().y, blocks[iterator].GetSize(), blocks[iterator].GetSize(), screen, blocks[iterator].GetFrame());
			break;
		case Block::All:
			colors->DrawScaled(blocks[iterator].GetPosition().x, blocks[iterator].GetPosition().y, blocks[iterator].GetSizeOne(), blocks[iterator].GetSizeOne(), screen, blocks[iterator].GetFrame());
			break;
		}*/

#ifdef _DEBUG
		/***************************************

			Code for drawing box-collisions

		****************************************/

		//Core x1
		x11 = blocks[iterator].GetPosition().x + blocks[iterator].GetCorePos().x;

		//Core x2
		x22 = blocks[iterator].GetPosition().x + blocks[iterator].GetCorePos().x + blocks[iterator].GetCoreWidth();

		//Core y1
		y11 = blocks[iterator].GetPosition().y + blocks[iterator].GetCorePos().y;

		//Core y2
		y22 = blocks[iterator].GetPosition().y + blocks[iterator].GetCorePos().y - blocks[iterator].GetCoreHeight();

		//Core Box
		screen->Box(x11, y11, x22, y22, 0x00ff00);


		//Extra x1
		x33 = blocks[iterator].GetPosition().x + blocks[iterator].GetExtraPos().x;

		//Extra x2
		x44 = blocks[iterator].GetPosition().x + blocks[iterator].GetExtraPos().x + blocks[iterator].GetExtraWidth();

		//Extra y1
		y33 = blocks[iterator].GetPosition().y + blocks[iterator].GetExtraPos().y;

		//Extra y2
		y44 = blocks[iterator].GetPosition().y + blocks[iterator].GetExtraPos().y - blocks[iterator].GetExtraHeight();

		//Extra Box
		screen->Box(x33, y33, x44, y44, 0xff0000);
#endif
	}

	void Game::DrawOldBlock()
	{
		for (int j = 0; j < 20; j++)
		{
			for (int i = 0; i < 10; i++)
			{
				switch (gridColors[i][j]) {
				case 1: 
					colors->DrawScaled(i * blocks[iterator].GetSizeOne() + leftBorder, bottomBorder - j * blocks[iterator].GetSizeOne(), blocks[iterator].GetSizeOne(), blocks[iterator].GetSizeOne(), screen, 0); 
					break;
				case 2:
					colors->DrawScaled(i * blocks[iterator].GetSizeOne() + leftBorder, bottomBorder - j * blocks[iterator].GetSizeOne(), blocks[iterator].GetSizeOne(), blocks[iterator].GetSizeOne(), screen, 1);
					break;
				case 3:
					colors->DrawScaled(i * blocks[iterator].GetSizeOne() + leftBorder, bottomBorder - j * blocks[iterator].GetSizeOne(), blocks[iterator].GetSizeOne(), blocks[iterator].GetSizeOne(), screen, 2);
					break;
				case 4:
					colors->DrawScaled(i * blocks[iterator].GetSizeOne() + leftBorder, bottomBorder - j * blocks[iterator].GetSizeOne(), blocks[iterator].GetSizeOne(), blocks[iterator].GetSizeOne(), screen, 3);
					break;
				case 5:
					colors->DrawScaled(i * blocks[iterator].GetSizeOne() + leftBorder, bottomBorder - j * blocks[iterator].GetSizeOne(), blocks[iterator].GetSizeOne(), blocks[iterator].GetSizeOne(), screen, 4);
					break;
				case 6:
					colors->DrawScaled(i * blocks[iterator].GetSizeOne() + leftBorder, bottomBorder - j * blocks[iterator].GetSizeOne(), blocks[iterator].GetSizeOne(), blocks[iterator].GetSizeOne(), screen, 5);
					break;
				case 7:
					colors->DrawScaled(i * blocks[iterator].GetSizeOne() + leftBorder, bottomBorder - j * blocks[iterator].GetSizeOne(), blocks[iterator].GetSizeOne(), blocks[iterator].GetSizeOne(), screen, 6);
					break;
				}
			}
		}
	}

	void Game::DrawPlayingField()
	{
		for (int i = 1; i < 10; i++)
		{
			int x = leftBorder + blocks[0].GetSizeOne() * i;
			screen->Line(x, 0, x, bottomBorder, 0x505050);
		}

		for (int j = 1; j < 20; j++)
		{
			int y = blocks[0].GetSizeOne() * j;
			screen->Line(leftBorder, y, rightBorder, y, 0x505050);
		}
		//Draw borders
		screen->Line(static_cast<float>(leftBorder), static_cast<float>(bottomBorder), static_cast<float>(rightBorder), static_cast<float>(bottomBorder), 0xffffff);
		screen->Line(static_cast<float>(leftBorder), 0, static_cast<float>(leftBorder), static_cast<float>(bottomBorder), 0xffffff);
		screen->Line(static_cast<float>(rightBorder), 0, static_cast<float>(rightBorder), static_cast<float>(bottomBorder), 0xffffff);
	}

	void Game::GridCollision()
	{
		//Calculate the Y-position in the grid
		yPosCore = abs(core_y1 - bottomBorder) / blocks[iterator].GetSizeOne();
		yPosExtra = abs(extra_y1 - bottomBorder) / blocks[iterator].GetSizeOne();

		//Check if the grid left of the xPos is true
		if (InBounds(xPosCore, yPosCore, 1) && grid[xPosCore - 1][yPosCore + 1] == true) allowLeft = false;
		else if (InBounds(xPosExtra, yPosExtra, 1) && grid[xPosExtra - 1][yPosExtra + 1] == true) allowLeft = false;
		else allowLeft = true;

		//Check if the grid right of the xPos is true
		if (InBounds(xPosCore, yPosCore, 2) && grid[xPosCore + 1][yPosCore + 1] == true) allowRight = false;
		else if (InBounds(xPosExtra, yPosExtra, 2) && grid[xPosExtra + 1][yPosExtra + 1] == true) allowRight = false;
		else allowRight = true;

		//Check if the grid under Core is true. If so, prevent movement downwards, nextBlock == true;
		if (InBounds(xPosCore, yPosCore, 3) && grid[xPosCore][yPosCore] == true)
		{
			FixedPosition(xPosCore, yPosCore);
		}
		//Check if the grid under Extra is true. If so, prevent movement downwards, nextBlock == true;
		else if (InBounds(xPosExtra, yPosExtra, 3) && grid[xPosExtra][yPosExtra] == true)
		{
			FixedPosition(xPosExtra, yPosExtra);
		}
		else if (yPosCore == 0 || yPosExtra == 0)
		{
			if (!timeStampTaken)
			{
				onGroundTimeStamp = static_cast<float>(blockTimer.totalSeconds());
				timeStampTaken = true;
			}

			if ((static_cast<float>(blockTimer.totalSeconds()) - onGroundTimeStamp) >= waitTime)
			{
				timeStampTaken = false;
				gameState = NextBlock;
			}
		}
		else timeStampTaken = false;
	}

	void Game::AddToGrid()
	{
		//Sets occupation bool for the core-blocks
		for (long long x = 0; x < (blocks[iterator].GetCoreWidth() / blocks[iterator].GetSizeOne()); x++)
		{
			for (long long y = 0; y < (blocks[iterator].GetCoreHeight() / blocks[iterator].GetSizeOne()); y++)
			{
				//If inside the borders of the playing field
				if ((xPosCore + x < grid.size()) && (yPosCore + y < grid[x].size())) 
				{
					grid[xPosCore + x][yPosCore + y + 1] = true;

					switch (blockColor) {
					case Block::Blue:
						gridColors[xPosCore + x][yPosCore + y + 1] = 1;
						break;
					case Block::Green:
						gridColors[xPosCore + x][yPosCore + y + 1] = 2;
						break;
					case Block::LightBlue:
						gridColors[xPosCore + x][yPosCore + y + 1] = 3;
						break;
					case Block::Orange:
						gridColors[xPosCore + x][yPosCore + y + 1] = 4;
						break;
					case Block::Purple:
						gridColors[xPosCore + x][yPosCore + y + 1] = 5;
						break;
					case Block::Red:
						gridColors[xPosCore + x][yPosCore + y + 1] = 6;
						break;
					case Block::Yellow:
						gridColors[xPosCore + x][yPosCore + y + 1] = 7;
						break;
					}
				}
			}
		}

		//Sets occupation bool for the extra-blocks
		for (long long x = 0; x < (blocks[iterator].GetExtraWidth() / blocks[iterator].GetSizeOne()); x++) 
		{
			for (long long y = 0; y < (blocks[iterator].GetExtraHeight() / blocks[iterator].GetSizeOne()); y++) 
			{
				//If inside the borders of the playing field
				if ((xPosExtra + x < grid.size()) && (yPosExtra + y < grid[x].size())) 
				{
					grid[xPosExtra + x][yPosExtra + y + 1] = true;

					switch (blockColor) {
					case Block::Blue:
						gridColors[xPosExtra + x][yPosExtra + y + 1] = 1;
						break;
					case Block::Green:
						gridColors[xPosExtra + x][yPosExtra + y + 1] = 2;
						break;
					case Block::LightBlue:
						gridColors[xPosExtra + x][yPosExtra + y + 1] = 3;
						break;
					case Block::Orange:
						gridColors[xPosExtra + x][yPosExtra + y + 1] = 4;
						break;
					case Block::Purple:
						gridColors[xPosExtra + x][yPosExtra + y + 1] = 5;
						break;
					case Block::Red:
						gridColors[xPosExtra + x][yPosExtra + y + 1] = 6;
						break;
					case Block::Yellow:
						gridColors[xPosExtra + x][yPosExtra + y + 1] = 7;
						break;
					}

				}
			}
		}
	}

	bool Game::InBounds(int xPos, int yPos, int lever)
	{
		switch (lever) {
		case 1:
			if (xPos > 0 && xPos < 10 && yPos > 0 && yPos < 19) return true;
			else return false;
			break;
		case 2:
			if (xPos >= 0 && xPos < 9 && yPos > 0 && yPos < 19) return true;
			else return false;
			break;
		case 3:
			if (xPos >= 0 && xPos < 10 && yPos > 0 && yPos < 19) return true;
			else return false;
			break;
		}
	}

	void Game::FixedPosition(int xPos, int yPos)
	{
		if (!timeStampTaken)
		{
			onGroundTimeStamp = static_cast<float>(blockTimer.totalSeconds());
			timeStampTaken = true;
		}

		if ((static_cast<float>(blockTimer.totalSeconds()) - onGroundTimeStamp) >= waitTime)
		{
			timeStampTaken = false;
			gameState = NextBlock;
		}

		blocks[iterator].WallCollision(leftBorder, rightBorder, bottomBorder - (yPos * blocks[iterator].GetSizeOne() - 1));
	}

	void Game::ChangeColor()
	{
		//blockColor = Block::Orange;
		
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
			if (!timeStampTaken) blocks[iterator].SetPosition({ blocks[iterator].GetPosition().x, blocks[iterator].GetPosition().y + static_cast<int>(blocks[iterator].GetSize() / 3) });
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

	void Game::ShowTestGrid()
	{
		for (int e = 0; e < 20; e++)
		{
			std::cout << " " << std::endl;
		}

		for (int j = 0; j < 19; j++)
		{
			for (int i = 0; i < 10; i++)
			{

				if (19 - j == yPosCore && i == xPosCore)
				{
					std::cout << std::format(" TRUE  |");
				}
				else std::cout << std::format("{:^7}|", grid[i][19 - j]);
			}

			std::cout << std::endl;
		}
	}
};