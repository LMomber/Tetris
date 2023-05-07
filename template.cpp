// Template, BUAS version https://www.buas.nl/games
// IGAD/BUAS(NHTV)/UU - Jacco Bikker - 2006-2020

// Note:
// this version of the template uses SDL2 for all frame buffer interaction
// see: https://www.libsdl.org

#ifdef _MSC_VER
#pragma warning (disable : 4530) // complaint about exception handler
#pragma warning (disable : 4311) // pointer truncation from HANDLE to long
#endif

//#define FULLSCREEN
//#define ADVANCEDGL

#include "game.h"

#include <fcntl.h>
#include <io.h>
#include "template.h"
#include <corecrt_math.h>
#include "SDL.h"
#include "surface.h"
#include <cstdio>
#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifdef ADVANCEDGL
#define GLEW_BUILD
extern "C"
{
#include "glew.h" 
}
#include "gl.h"
#include "wglext.h"
#endif

namespace Tmpl8 { 

double timer::inv_freq = 1;

timer::timer(): start(get())
{
	init();
}

float timer::elapsed() const
{
	return (float)((get() - start) * inv_freq);
}

timer::value_type timer::get()
{
	LARGE_INTEGER c;
	QueryPerformanceCounter(&c);
	return c.QuadPart;
}

double timer::to_time(const value_type vt)
{
	return double(vt) * inv_freq;
}

void timer::reset()
{
	start = get();
}

void timer::init()
{
	LARGE_INTEGER f;
	QueryPerformanceFrequency(&f);
	inv_freq = 1000. / double(f.QuadPart);
}

void NotifyUser( char* s )
{
	HWND hApp = FindWindow(nullptr, TemplateVersion);
	MessageBox( hApp, s, "ERROR", MB_OK );
	exit( 0 );
}

}

using namespace Tmpl8;
using namespace std;

static bool firstframe = true;

Surface* surface = 0;
Game* game = 0;
SDL_Window* window = 0;

#ifdef _MSC_VER
bool redirectIO()
{
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	AllocConsole();
	GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &coninfo );
	coninfo.dwSize.Y = 500;
	SetConsoleScreenBufferSize( GetStdHandle( STD_OUTPUT_HANDLE ), coninfo.dwSize );
	HANDLE h1 = GetStdHandle( STD_OUTPUT_HANDLE );
	int h2 = _open_osfhandle( (intptr_t)h1, _O_TEXT );
	FILE* fp = _fdopen( h2, "w" );
	*stdout = *fp;
	setvbuf( stdout, NULL, _IONBF, 0 );
	h1 = GetStdHandle( STD_INPUT_HANDLE ), h2 = _open_osfhandle( (intptr_t)h1, _O_TEXT );
	fp = _fdopen( h2, "r" ), *stdin = *fp;
	setvbuf( stdin, NULL, _IONBF, 0 );
	h1 = GetStdHandle( STD_ERROR_HANDLE ), h2 = _open_osfhandle( (intptr_t)h1, _O_TEXT );
	fp = _fdopen( h2, "w" ), *stderr = *fp;
	setvbuf( stderr, NULL, _IONBF, 0 );
	ios::sync_with_stdio();
    FILE* stream;
    if ((stream = freopen("CON", "w", stdout)) == NULL)
        return false;
    if ((stream = freopen("CON", "w", stderr)) == NULL)
        return false;
    return true;
}
#endif

int main( int argc, char **argv ) 
{  
#ifdef _MSC_VER
    if (!redirectIO())
        return 1;
#endif
	SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	window = SDL_CreateWindow(TemplateVersion, 100, 100, ScreenWidth, ScreenHeight, SDL_WINDOW_SHOWN );
	surface = new Surface( ScreenWidth, ScreenHeight );
	surface->Clear( 0 );
	SDL_Renderer* renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	SDL_Texture* frameBuffer = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, ScreenWidth, ScreenHeight );

	int exitapp = 0;
	game = new Game();
	game->SetTarget( surface );
	timer t;
	t.reset();

	while (!exitapp) 
	{
		void* target = 0;
		int pitch;
		SDL_LockTexture( frameBuffer, NULL, &target, &pitch );
		if (pitch == (surface->GetWidth() * 4))
		{
			memcpy( target, surface->GetBuffer(), ScreenWidth * ScreenHeight * 4 );
		}
		else
		{
			unsigned char* t = (unsigned char*)target;
			for( int i = 0; i < ScreenHeight; i++ )
			{
				memcpy( t, surface->GetBuffer() + i * ScreenWidth, ScreenWidth * 4 );
				t += pitch;
			}
		}
		SDL_UnlockTexture( frameBuffer );
		SDL_RenderCopy( renderer, frameBuffer, NULL, NULL );
		SDL_RenderPresent( renderer );

		if (firstframe)
		{
			game->Init();
			firstframe = false;
		}
		// calculate frame time and pass it to game->Tick
		float elapsedTime = t.elapsed();
		t.reset();

		game->Tick( elapsedTime );
		// event loop
		SDL_Event event;
		while (SDL_PollEvent( &event )) 
		{
			switch (event.type)
			{
			case SDL_QUIT:
				exitapp = 1;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) 
				{
					exitapp = 1;
					// find other keys here: http://sdl.beuc.net/sdl.wiki/SDLKey
				}
				game->KeyDown( event.key.keysym.scancode );
				break;
			case SDL_KEYUP:
				game->KeyUp( event.key.keysym.scancode );
				break;
			case SDL_MOUSEMOTION:
				game->MouseMove( event.motion.x, event.motion.y );
				break;
			case SDL_MOUSEBUTTONUP:
				game->MouseUp( event.button.button );
				break;
			case SDL_MOUSEBUTTONDOWN:
				game->MouseDown( event.button.button );
				break;
			default:
				break;
			}
		}
	}
	game->Shutdown();
	SDL_Quit();
	return 0;
}