#include "game.h"


namespace Tmpl8
{
	void Game::Init()
	{
        blockTimer.reset();
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

        //Rotating & moving blocks on keypress
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                /* Look for a keypress */
            case SDL_KEYDOWN:
                /* Check the SDLKey values and move change the coords */
                switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                    block.SetPosition({ block.GetPosition().x - static_cast<int>(block.GetSize() / 3), block.GetPosition().y});
                    break;
                case SDLK_RIGHT:
                    block.SetPosition({ block.GetPosition().x + static_cast<int>(block.GetSize() / 3), block.GetPosition().y });
                    break;
                case SDLK_UP:
                    blue.SetFrame(block.FramePlus());
                    break;
                case SDLK_DOWN:
                    block.SetPosition({ block.GetPosition().x, block.GetPosition().y + static_cast<int>(block.GetSize() / 3) });
                    break;
                default:
                    break;
                }
            }
        }

        screen->Clear(0); 
        drawBlock(); 
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
            red.DrawScaled(block.GetPosition().x, block.GetPosition().y, block.GetSize(), block.GetSize(), screen); 
            break;
        case Block::Orange:
            orange.DrawScaled(block.GetPosition().x, block.GetPosition().y, block.GetSize(), block.GetSize(), screen); 
            break;
        case Block::Yellow:
            yellow.DrawScaled(block.GetPosition().x, block.GetPosition().y, block.GetSize(), block.GetSize(), screen); 
            break;
        case Block::Purple:
            purple.DrawScaled(block.GetPosition().x, block.GetPosition().y, block.GetSize(), block.GetSize(), screen); 
            break;
        case Block::LightBlue:
            lightBlue.DrawScaled(block.GetPosition().x, block.GetPosition().y, block.GetSize(), block.GetSize(), screen); 
            break;
        }
    }
};