#include "game.h"
#include <iostream>


namespace Tmpl8
{
	void Game::Init()
	{
		blockTimer.reset();
		block.SetPosition({ (ScreenWidth / 2) - (block.GetSize() / 2), block.GetSize() });
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
};