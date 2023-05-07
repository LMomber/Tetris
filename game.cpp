#include "game.h"
#include <iostream>


namespace Tmpl8
{
	void Game::Init()
	{
		blockTimer.reset();
		block.SetPosition({ (ScreenWidth / 2) - (block.GetSize() / 2), block.GetSize() });
		block.Collider();
	}

	void Game::Shutdown()
	{
	}

	void Game::Tick(float deltaTime)
	{
		blockTimer.tick();
		elapsedSeconds = static_cast<int>(blockTimer.totalSeconds());

		//Move blocks downwards
		if (elapsedSeconds > lastSecond)
		{
			block.SetPosition({ block.GetPosition().x, block.GetPosition().y + static_cast<int>(block.GetSize() / 3) });
			lastSecond = elapsedSeconds;
		}
		
		block.WallCollision();

		screen->Clear(0);
		drawBlock();
	}

	void Game::KeyDown(int key)
	{
		switch (key) {
		case SDL_SCANCODE_W:
		case SDL_SCANCODE_UP:
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
			break;
		case SDL_SCANCODE_S:
		case SDL_SCANCODE_DOWN:
			block.SetPosition({ block.GetPosition().x, block.GetPosition().y + static_cast<int>(block.GetSize() / 3) });
			break;
		case SDL_SCANCODE_A:
		case SDL_SCANCODE_LEFT:
			block.SetPosition({ block.GetPosition().x - static_cast<int>(block.GetSize() / 3), block.GetPosition().y });
			break;
		case SDL_SCANCODE_D:
		case SDL_SCANCODE_RIGHT:
			block.SetPosition({ block.GetPosition().x + static_cast<int>(block.GetSize() / 3), block.GetPosition().y });
			break;
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
		int x1 = block.GetPosition().x + block.GetCorePos().x;
		//screen->Line(x1, 0, x1, ScreenHeight, 0x00ff00);
		//Core x2
		int x2 = block.GetPosition().x + block.GetCorePos().x + block.GetCoreWidth();
		//screen->Line(x2, 0, x2, ScreenHeight, 0x00ff00);
		//Core y1
		int y1 = block.GetPosition().y + block.GetCorePos().y;
		//screen->Line(0, y1, ScreenWidth, y1, 0x00ff00);
		//Core y2
		int y2 = block.GetPosition().y + block.GetCorePos().y - block.GetCoreHeight();
		//screen->Line(0, y2, ScreenWidth, y2, 0x00ff00);

		//Core Box
		screen->Box(x1, y1, x2, y2, 0x00ff00);

		//Core x1
		int x3 = block.GetPosition().x + block.GetExtraPos().x;
		//screen->Line(x3, 0, x3, ScreenHeight, 0x00ff00);
		//Core x2
		int x4 = block.GetPosition().x + block.GetExtraPos().x + block.GetExtraWidth();
		//screen->Line(x4, 0, x4, ScreenHeight, 0x00ff00);
		//Core y1
		int y3 = block.GetPosition().y + block.GetExtraPos().y;
		//screen->Line(0, y3, ScreenWidth, y3, 0x00ff00);
		//Core y2
		int y4 = block.GetPosition().y + block.GetExtraPos().y - block.GetExtraHeight();
		//screen->Line(0, y4, ScreenWidth, y4, 0x00ff00);

		//Core Box
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