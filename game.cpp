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
			CheckRows();

			screen->Clear(0); 
			DrawPlayingField(); 
			DrawScore();

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

	void Game::DrawScore()
	{
		//screen->Print(score, 50, 100, 0xffffff, 5);
	}

	void Game::GridCollision()
	{
		//Calculate the Y-position in the grid
		yPosCore = abs(core_y1 - bottomBorder) / blocks[iterator].GetSizeOne();
		yPosExtra = abs(extra_y1 - bottomBorder) / blocks[iterator].GetSizeOne();

		BlockCollision(blocks[iterator].GetColor());
	}

	bool Game::IsLeft(int xPos, int yPos)
	{
		//Check if the grid left of the xPos is true
		if (InBounds(xPos, yPos, 1) && grid[xPos - 1][yPos + 1] == true)
		{
			return true;
		}
		else return false;
	}

	bool Game::IsRight(int xPos, int yPos)
	{
		//Check if the grid right of the xPos is true
		if (InBounds(xPos, yPos, 2) && grid[xPos + 1][yPos + 1] == true)
		{
			return true;
		}
		else return false;
	}

	bool Game::IsUnder(int xPos, int yPos)
	{
		if (InBounds(xPosCore, yPosCore, 3) && grid[xPosCore][yPosCore] == true)
		{
			FixedPosition(xPosCore, yPosCore);
			return true;
		}
		else return false;
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

	void Game::CheckRows()
	{
		//For all Y:
		for (int j = 0; j < 20; j++)
		{
			int tempVar = 0;

			//For all X:
			for (int i = 0; i < 10; i++)
			{
				//If the grid on the x,y is true, increase tempVar
				if (grid[i][j] == true) tempVar++;
			}

			//If the whole row is true:
			if (tempVar == 10)
			{
				score += 1000;
				//Make the row false again
				for (int i = 0; i < 10; i++)
				{
					grid[i][j] = false;
					gridColors[i][j] = 0;
				}

				//Make all Y's above the deleted row Y - 1
				for (int k = j + 1; k < 20; k++)
				{
					for (int i = 0; i < 10; i++)
					{
						grid[i][k - 1] = grid[i][k];
						gridColors[i][k - 1] = gridColors[i][k];
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

		return false;
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
		//blockColor = Block::LightBlue;
		
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

	void Game::BlockCollision(Block::Color color)
	{
		switch (color) {
		case Block::Blue:
			switch (blocks[iterator].GetFrame()) {
			case 0:
			{
				if (IsLeft(xPosCore, yPosCore) || IsLeft(xPosExtra, yPosExtra)) allowLeft = false;
				else allowLeft = true;

				if (IsRight(xPosCore + 1, yPosCore) || IsRight(xPosExtra + 1, yPosExtra)) allowRight = false;
				else allowRight = true;

				if		(IsUnder(xPosCore, yPosCore));
				else if (IsUnder(xPosCore + 1, yPosCore));
				else if (IsUnder(xPosExtra + 1, yPosExtra));
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
				break;
			}
			case 1:
			{
				if (IsLeft(xPosCore, yPosCore) || IsLeft(xPosCore, yPosCore + 1) || IsLeft(xPosExtra, yPosExtra)) allowLeft = false;
				else allowLeft = true;

				if (IsRight(xPosCore, yPosCore + 1) || IsRight(xPosExtra, yPosExtra) || IsRight(xPosExtra, yPosExtra + 1)) allowRight = false;
				else allowRight = true;

				if		(IsUnder(xPosCore, yPosCore));
				else if (IsUnder(xPosExtra, yPosExtra));
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
				break;
			}
			case 2:
			{
				if (IsLeft(xPosCore, yPosCore) || IsLeft(xPosExtra, yPosExtra)) allowLeft = false;
				else allowLeft = true;

				if (IsRight(xPosCore + 1, yPosCore) || IsRight(xPosExtra + 1, yPosExtra)) allowRight = false;
				else allowRight = true;

				if		(IsUnder(xPosCore, yPosCore));
				else if (IsUnder(xPosCore + 1, yPosCore));
				else if (IsUnder(xPosExtra + 1, yPosExtra));
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
				break;
			}
			case 3:
			{
				if (IsLeft(xPosCore, yPosCore) || IsLeft(xPosCore, yPosCore + 1) || IsLeft(xPosExtra, yPosExtra)) allowLeft = false;
				else allowLeft = true;

				if (IsRight(xPosCore, yPosCore + 1) || IsRight(xPosExtra, yPosExtra) || IsRight(xPosExtra, yPosExtra + 1)) allowRight = false;
				else allowRight = true;

				if		(IsUnder(xPosCore, yPosCore));
				else if (IsUnder(xPosExtra, yPosExtra));
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
				break;
			}
			}
			break;
		case Block::Green:
			switch (blocks[iterator].GetFrame()) {
			case 0:
			{
				if (IsLeft(xPosCore, yPosCore) || IsLeft(xPosExtra, yPosExtra)) allowLeft = false;
				else allowLeft = true;

				if (IsRight(xPosCore + 2, yPosCore) || IsRight(xPosExtra, yPosExtra)) allowRight = false;
				else allowRight = true;

				if		(IsUnder(xPosCore, yPosCore));
				else if (IsUnder(xPosCore + 1, yPosCore));
				else if (IsUnder(xPosCore + 2, yPosCore));
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
				break;
			}
			case 1:
			{
				if (IsLeft(xPosCore, yPosCore) || IsLeft(xPosCore, yPosCore + 1) || IsLeft(xPosCore, yPosCore + 2)) allowLeft = false;
				else allowLeft = true;

				if (IsRight(xPosCore, yPosCore) || IsRight(xPosCore, yPosCore + 1) || IsRight(xPosExtra, yPosExtra)) allowRight = false;
				else allowRight = true;

				if		(IsUnder(xPosCore, yPosCore));
				else if (IsUnder(xPosExtra, yPosExtra));
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
				break;
			}
			case 2:
			{
				if (IsLeft(xPosCore, yPosCore) || IsLeft(xPosExtra, yPosExtra)) allowLeft = false;
				else allowLeft = true;

				if (IsRight(xPosCore + 2, yPosCore) || IsRight(xPosExtra, yPosExtra)) allowRight = false;
				else allowRight = true;

				if		(IsUnder(xPosCore, yPosCore));
				else if (IsUnder(xPosCore + 1, yPosCore));
				else if (IsUnder(xPosExtra, yPosExtra));
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
				break;
			}
			case 3:
			{
				if (IsLeft(xPosCore, yPosCore) || IsLeft(xPosExtra, yPosExtra + 1) || IsLeft(xPosExtra, yPosExtra + 2)) allowLeft = false;
				else allowLeft = true;

				if (IsRight(xPosExtra, yPosExtra) || IsRight(xPosExtra, yPosExtra + 1) || IsRight(xPosExtra, yPosExtra + 2)) allowRight = false;
				else allowRight = true;

				if		(IsUnder(xPosCore, yPosCore));
				else if (IsUnder(xPosExtra, yPosExtra));
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
				break;
			}
			}
			break;
		case Block::Red:
		{
			if (IsLeft(xPosCore, yPosCore) || IsLeft(xPosCore, yPosCore + 1)) allowLeft = false;
			else allowLeft = true;

			if (IsRight(xPosCore + 1, yPosCore) || IsRight(xPosCore + 1, yPosCore + 1)) allowRight = false;
			else allowRight = true;

			if		(IsUnder(xPosCore, yPosCore));
			else if (IsUnder(xPosCore + 1, yPosCore));
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
			break;
		}
		case Block::Orange:
			switch (blocks[iterator].GetFrame()) {
			case 0:
			{
				if (IsLeft(xPosCore, yPosCore) || IsLeft(xPosExtra, yPosExtra)) allowLeft = false;
				else allowLeft = true;

				if (IsRight(xPosCore + 2, yPosCore) || IsRight(xPosExtra, yPosExtra)) allowRight = false;
				else allowRight = true;

				if		(IsUnder(xPosCore, yPosCore));
				else if (IsUnder(xPosCore + 1, yPosCore));
				else if (IsUnder(xPosCore + 2, yPosCore));
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
				break;
			}
			case 1:
			{
				if (IsLeft(xPosCore, yPosCore) || IsLeft(xPosCore, yPosCore + 1) || IsLeft(xPosCore, yPosCore + 2)) allowLeft = false;
				else allowLeft = true;

				if (IsRight(xPosCore, yPosCore) || IsRight(xPosCore, yPosCore + 2) || IsRight(xPosExtra, yPosExtra)) allowRight = false;
				else allowRight = true;

				if		(IsUnder(xPosCore, yPosCore));
				else if (IsUnder(xPosExtra, yPosExtra));
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
				break;
			case 2:
			{
				if (IsLeft(xPosCore, yPosCore) || IsLeft(xPosExtra, yPosExtra)) allowLeft = false;
				else allowLeft = true;

				if (IsRight(xPosCore + 2, yPosCore) || IsRight(xPosExtra, yPosExtra)) allowRight = false;
				else allowRight = true;

				if		(IsUnder(xPosCore, yPosCore));
				else if (IsUnder(xPosCore + 2, yPosCore));
				else if (IsUnder(xPosExtra, yPosExtra));
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
				break;
			}
			case 3:
			{
				if (IsLeft(xPosCore, yPosCore) || IsLeft(xPosExtra, yPosExtra) || IsLeft(xPosExtra, yPosExtra + 2)) allowLeft = false;
				else allowLeft = true;

				if (IsRight(xPosExtra, yPosExtra) || IsRight(xPosExtra, yPosExtra + 1) || IsRight(xPosExtra, yPosExtra + 2)) allowRight = false;
				else allowRight = true;

				if		(IsUnder(xPosCore, yPosCore));
				else if (IsUnder(xPosExtra, yPosExtra));
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
				break;
			}
			}
			break;
		case Block::Yellow:
			switch (blocks[iterator].GetFrame()) {
			case 0:
			{
				if (IsLeft(xPosCore, yPosCore) || IsLeft(xPosExtra, yPosExtra)) allowLeft = false;
				else allowLeft = true;

				if (IsRight(xPosCore + 1, yPosCore) || IsRight(xPosExtra + 1, yPosExtra)) allowRight = false;
				else allowRight = true;

				if		(IsUnder(xPosCore, yPosCore));
				else if (IsUnder(xPosExtra, yPosExtra));
				else if (IsUnder(xPosExtra + 1, yPosExtra));
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
				break;
			}
			case 1:
			{
				if (IsLeft(xPosCore, yPosCore) || IsLeft(xPosCore, yPosCore + 1) || IsLeft(xPosExtra, yPosExtra + 1)) allowLeft = false;
				else allowLeft = true;

				if (IsRight(xPosCore, yPosCore) || IsRight(xPosExtra, yPosExtra) || IsRight(xPosExtra, yPosExtra + 1)) allowRight = false;
				else allowRight = true;

				if		(IsUnder(xPosCore, yPosCore));
				else if (IsUnder(xPosExtra, yPosExtra));
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
				break;
			}
			case 2:
			{
				if (IsLeft(xPosCore, yPosCore) || IsLeft(xPosExtra, yPosExtra)) allowLeft = false;
				else allowLeft = true;

				if (IsRight(xPosCore + 1, yPosCore) || IsRight(xPosExtra + 1, yPosExtra)) allowRight = false;
				else allowRight = true;

				if		(IsUnder(xPosCore, yPosCore));
				else if (IsUnder(xPosExtra, yPosExtra));
				else if (IsUnder(xPosExtra + 1, yPosExtra));
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
				break;
			}
			case 3:
			{
				if (IsLeft(xPosCore, yPosCore) || IsLeft(xPosCore, yPosCore + 1) || IsLeft(xPosExtra, yPosExtra + 1)) allowLeft = false;
				else allowLeft = true;

				if (IsRight(xPosCore, yPosCore) || IsRight(xPosExtra, yPosExtra) || IsRight(xPosExtra, yPosExtra + 1)) allowRight = false;
				else allowRight = true;

				if		(IsUnder(xPosCore, yPosCore));
				else if (IsUnder(xPosExtra, yPosExtra));
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
				break;
			}
			}
			break;
		case Block::Purple:
			switch (blocks[iterator].GetFrame()) {
			case 0:
			{
				if (IsLeft(xPosCore, yPosCore)) allowLeft = false;
				else allowLeft = true;

				if (IsRight(xPosCore + 3, yPosCore)) allowRight = false;
				else allowRight = true;

				if (IsUnder(xPosCore, yPosCore));
				else if (IsUnder(xPosCore + 1, yPosCore));
				else if (IsUnder(xPosCore + 2, yPosCore));
				else if (IsUnder(xPosCore + 3, yPosCore));
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
				break;
			}
			case 1:
			{
				if (IsLeft(xPosCore, yPosCore) || IsLeft(xPosCore, yPosCore + 1) || IsLeft(xPosCore, yPosCore + 2) || IsLeft(xPosCore, yPosCore + 3)) allowLeft = false;
				else allowLeft = true;

				if (IsRight(xPosCore, yPosCore) || IsRight(xPosCore, yPosCore + 1) || IsRight(xPosCore, yPosCore + 2) || IsRight(xPosCore, yPosCore + 3)) allowRight = false;
				else allowRight = true;

				if (IsUnder(xPosCore, yPosCore));
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
				break;
			}
			case 2:
			{
				if (IsLeft(xPosCore, yPosCore)) allowLeft = false;
				else allowLeft = true;

				if (IsRight(xPosCore + 3, yPosCore)) allowRight = false;
				else allowRight = true;

				if (IsUnder(xPosCore, yPosCore));
				else if (IsUnder(xPosCore + 1, yPosCore));
				else if (IsUnder(xPosCore + 2, yPosCore));
				else if (IsUnder(xPosCore + 3, yPosCore));
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
				break;
			}
			case 3:
			{
				if (IsLeft(xPosCore, yPosCore) || IsLeft(xPosCore, yPosCore + 1) || IsLeft(xPosCore, yPosCore + 2) || IsLeft(xPosCore, yPosCore + 3)) allowLeft = false;
				else allowLeft = true;

				if (IsRight(xPosCore, yPosCore) || IsRight(xPosCore, yPosCore + 1) || IsRight(xPosCore, yPosCore + 2) || IsRight(xPosCore, yPosCore + 3)) allowRight = false;
				else allowRight = true;

				if (IsUnder(xPosCore, yPosCore));
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
				break;
			}
			}
			break;
		case Block::LightBlue:
			switch (blocks[iterator].GetFrame()) {
			case 0:
			{
				if (IsLeft(xPosCore, yPosCore) || IsLeft(xPosExtra, yPosExtra)) allowLeft = false;
				else allowLeft = true;

				if (IsRight(xPosCore + 2, yPosCore) || IsRight(xPosExtra, yPosExtra)) allowRight = false;
				else allowRight = true;

				if (IsUnder(xPosCore, yPosCore));
				else if (IsUnder(xPosCore + 1, yPosCore));
				else if (IsUnder(xPosCore + 2, yPosCore));
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
				break;
			}
			case 1:
			{
				if (IsLeft(xPosCore, yPosCore) || IsLeft(xPosCore, yPosCore + 1) || IsLeft(xPosCore, yPosCore + 1)) allowLeft = false;
				else allowLeft = true;

				if (IsRight(xPosCore, yPosCore + 1) || IsRight(xPosCore, yPosCore + 2) || IsRight(xPosExtra, yPosExtra)) allowRight = false;
				else allowRight = true;

				if (IsUnder(xPosCore, yPosCore));
				else if (IsUnder(xPosExtra, yPosExtra));
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
				break;
			}
			case 2:
			{
				if (IsLeft(xPosCore, yPosCore) || IsLeft(xPosExtra, yPosExtra)) allowLeft = false;
				else allowLeft = true;

				if (IsRight(xPosCore, yPosCore) || IsRight(xPosExtra + 2, yPosExtra)) allowRight = false;
				else allowRight = true;

				if (IsUnder(xPosCore, yPosCore));
				else if (IsUnder(xPosExtra + 1, yPosExtra));
				else if (IsUnder(xPosExtra + 2, yPosExtra));
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
				break;
			}
			case 3:
			{
				if (IsLeft(xPosCore, yPosCore) || IsLeft(xPosExtra, yPosExtra) || IsLeft(xPosExtra, yPosExtra + 1)) allowLeft = false;
				else allowLeft = true;

				if (IsRight(xPosExtra, yPosExtra) || IsRight(xPosExtra, yPosExtra + 1) || IsRight(xPosExtra, yPosExtra + 2)) allowRight = false;
				else allowRight = true;

				if (IsUnder(xPosCore, yPosCore));
				else if (IsUnder(xPosExtra, yPosExtra));
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
				break;
			}
			}
			break;
		}
	}
};