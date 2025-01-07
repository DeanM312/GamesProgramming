
#include <SDL.h>
#include <stdio.h>
#include <random>
#include <iostream>

int main( int argc, char* args[] )
{
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}
	else
	{
		
		window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 800, SDL_WINDOW_SHOWN );
		if( window == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		}
		else
		{
			screenSurface = SDL_GetWindowSurface( window );


			SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0, 255, 0 ) );
			SDL_UpdateWindowSurface( window );
            
           
			SDL_Event event;
			bool running = true;

			while (running) {
				while (SDL_PollEvent(&event)) {
					if (event.type == SDL_QUIT) {
						running = false;
						break;
					}
					else if (event.type == SDL_KEYDOWN)
					{
						if (event.key.keysym.sym == SDLK_r)
						{
							SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, (std::rand() % (255 - 1)), (std::rand() % (255 - 1)), (std::rand() % (255 - 1))));
							SDL_UpdateWindowSurface(window);
						}
					}
				}
			}
		}
	}

	
	SDL_DestroyWindow( window );
	SDL_Quit();

	return 0;
}
