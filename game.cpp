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

			blocks[iterator].WallCollision(leftBorder, rightBorder, bottomBorder);
			GridCollision();

			screen->Clear(0);

			//Draw sprites
			DrawBlock(iterator);
			for (int i = 0; i < iterator; i++)
			{
				DrawBlock(i);
				//DrawOldBlock(i);
			}

			DrawPlayingField();
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
		leftBorder = (ScreenWidth / 2) - (blocks[iterator].GetSizeOne() * 5);
		rightBorder = (ScreenWidth / 2) + (blocks[iterator].GetSizeOne() * 5);
		startPosition = { leftBorder + (blocks[iterator].GetSizeOne() * 3),
				(blocks[iterator].GetCoreHeight() > blocks[iterator].GetExtraHeight() ? blocks[iterator].GetCoreHeight() : blocks[iterator].GetExtraHeight()) };
		blocks[iterator].SetPosition(startPosition);
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

#ifdef _DEBUG
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
#endif
	}

	void Game::DrawOldBlock(int i)
	{
		int x = leftBorder + blocks[i].GetSizeOne() * blocks[i].GetGridPos().x;
		int y = bottomBorder - blocks[i].GetSizeOne() * blocks[i].GetGridPos().y;
		int size = blocks[i].GetSize();
		int frame = blocks[i].GetFrame();

		switch (blocks[i].GetColor()) {
		case Block::Blue:
			blue->DrawScaled(x, y, size, size, screen, frame);
			break;
		case Block::Green:
			green->DrawScaled(x, y, size, size, screen, frame);
			break;
		case Block::Red:
			red->DrawScaled(x, y, blocks[i].GetSizeOfRed(), blocks[i].GetSizeOfRed(), screen, frame);
			break;
		case Block::Orange:
			orange->DrawScaled(x, y, size, size, screen, frame);
			break;
		case Block::Yellow:
			yellow->DrawScaled(x, y, size, size, screen, frame);
			break;
		case Block::Purple:
			purple->DrawScaled(x, y, blocks[i].GetSizeOfPurple(), blocks[i].GetSizeOfPurple(), screen, frame);
			break;
		case Block::LightBlue:
			lightBlue->DrawScaled(x, y, size, size, screen, frame);
			break;
		}
	}

	void Game::DrawPlayingField()
	{
		//Draw borders
		screen->Line(static_cast<float>(leftBorder), static_cast<float>(bottomBorder), static_cast<float>(rightBorder), static_cast<float>(bottomBorder), 0xffffff);
		screen->Line(static_cast<float>(leftBorder), 0, static_cast<float>(leftBorder), static_cast<float>(bottomBorder), 0xffffff);
		screen->Line(static_cast<float>(rightBorder), 0, static_cast<float>(rightBorder), static_cast<float>(bottomBorder), 0xffffff);
	}

	void Game::GridCollision()
	{
		//Calculate the Y-position in the grid
		yPosCore = abs(y1 - bottomBorder) / blocks[iterator].GetSizeOne();
		yPosExtra = abs(y3 - bottomBorder) / blocks[iterator].GetSizeOne();

		//std::cout << yPosCore << std::endl;

		//Check if the grid left of the xPos is true
		if (yPosCore < 20 && xPosCore > 0 && grid[xPosCore - 1][yPosCore] == true) allowLeft = false;
		else if (yPosExtra < 20 && xPosExtra > 0 && grid[xPosExtra - 1][yPosExtra] == true) allowLeft = false;
		else allowLeft = true;

		//Check if the grid right of the xPos is true
		if (yPosCore < 20 && (xPosCore < 9 && xPosCore >= 0) && grid[xPosCore + 1][yPosCore] == true) allowRight = false;
		else if (yPosExtra < 20 && (xPosExtra < 9 && xPosExtra >= 0) && grid[xPosExtra + 1][yPosExtra] == true) allowRight = false;
		else allowRight = true;

		//Check if the grid under Core is true. If so, prevent movement downwards, nextBlock == true;
		if ((xPosCore >= 0 && xPosCore < 10 && yPosCore > 0 && yPosCore < 19) && grid[xPosCore][yPosCore] == true)
		{
			FixedPosition(xPosCore, yPosCore);
		}
		//Check if the grid under Extra is true. If so, prevent movement downwards, nextBlock == true;
		else if ((xPosExtra >= 0 && xPosExtra < 10 && yPosExtra > 0 && yPosExtra < 19) && grid[xPosExtra][yPosExtra] == true)
		{
			FixedPosition(xPosExtra, yPosExtra);
		}
		else if (yPosCore == 0 || yPosExtra == 0)
		{
			if (!timeStampTaken)
			{
				if (yPosCore <= yPosExtra)
				{
					blocks[iterator].SetGridPos({ yPosCore, yPosCore });
				}
				else blocks[iterator].SetGridPos({ xPosExtra, yPosExtra });

				onGroundTimeStamp = static_cast<float>(blockTimer.totalSeconds());
				timeStampTaken = true;
			}

			if ((static_cast<float>(blockTimer.totalSeconds()) - onGroundTimeStamp) >= waitTime)
			{
				timeStampTaken = false;
				gameState = NextBlock;
			}
		}
	}

	void Game::AddToGrid()
	{
		//Sets occupation bool for the core-blocks
		for (int x = 0; x < (blocks[iterator].GetCoreWidth() / blocks[iterator].GetSizeOne()); x++)
		{
			for (int y = 0; y < (blocks[iterator].GetCoreHeight() / blocks[iterator].GetSizeOne()); y++)
			{
				//If inside the borders of the playing field
				if ((xPosCore + x < grid.size()) && (yPosCore + y < grid[x].size())) grid[xPosCore + x][yPosCore + y + 1] = true;
			}
		}

		//Sets occupation bool for the extra-blocks
		for (int x = 0; x < (blocks[iterator].GetExtraWidth() / blocks[iterator].GetSizeOne()); x++)
		{
			for (int y = 0; y < (blocks[iterator].GetExtraHeight() / blocks[iterator].GetSizeOne()); y++)
			{
				//If inside the borders of the playing field
				if ((xPosExtra + x < grid.size()) && (yPosExtra + y < grid[x].size())) grid[xPosExtra + x][yPosExtra + y + 1] = true;
			}
		}
	}

	void Game::FixedPosition(int xPos, int yPos)
	{
		if (!timeStampTaken)
		{
			blocks[iterator].SetGridPos({ xPos, yPos });

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