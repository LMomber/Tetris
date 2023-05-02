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
        if (elapsedSeconds > lastSecond)
        {
            blockY += static_cast<int>(blockHeight / 3);
            lastSecond = elapsedSeconds;
        }

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                /* Look for a keypress */
            case SDL_KEYDOWN:
                /* Check the SDLKey values and move change the coords */
                switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                    blockX -= static_cast<int>(blockWidth / 3); 
                    break;
                case SDLK_RIGHT:
                    blockX += static_cast<int>(blockWidth / 3); 
                    break;
                case SDLK_UP:
                    blue.SetFrame(FrameMin()); 
                    break;
                case SDLK_DOWN:
                    blue.SetFrame(FramePlus());
                    break;
                default:
                    break;
                }
            }
        }

        screen->Clear(0); 
        drawBlock(blue); 
	}

    void Game::drawBlock(Sprite& color)
    {
        color.DrawScaled(blockX, blockY, blockWidth, blockHeight, screen);
    }

    int Game::FrameMin()
    {
        if (blockFrame > 0) blockFrame--;
        else blockFrame = 3;

        return blockFrame;
    }

    int Game::FramePlus()
    {
        if (blockFrame < 3) blockFrame++;
        else blockFrame = 0;

        return blockFrame;
    }
};