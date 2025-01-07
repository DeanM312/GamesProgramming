
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <random>
#include <iostream>

int main(int argc, char* args[])
{
	int imgFlags = IMG_INIT_PNG;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}

	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
	}



	int SCREENX = 1200;
	int SCREENY = 800;

	


	

	SDL_Window* window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREENX, SCREENY, SDL_WINDOW_SHOWN);

	SDL_Surface* screenSurface = SDL_GetWindowSurface(window);

	

	
	SDL_Surface* surface = IMG_Load("image.png");
	SDL_Surface*  optimizedSurface = SDL_ConvertSurface(surface, screenSurface->format, 0);
	
	SDL_Surface* surface2 = IMG_Load("image2.png");





	SDL_UpdateWindowSurface(window);

	SDL_Rect stretchRect;
	stretchRect.x = 0;
	stretchRect.y = 0;
	stretchRect.w = SCREENX;
	stretchRect.h = SCREENY;

	SDL_Rect rect1;
	rect1.x = 0;
	rect1.y = 0;
	rect1.w = 400;
	rect1.h = 400;

	SDL_Rect pos;
	rect1.x = 0;
	rect1.y = 0;
	rect1.w = 400;
	rect1.h = 400;

	


	SDL_Event event;
	bool running = true;
	bool background = true;

	while (running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
				break;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				pos.x = (std::rand() % (SCREENX - 1));
				pos.y = (std::rand() % (SCREENY - 1));

				if (event.key.keysym.sym == SDLK_0)
				{
					if (background)
					{
						SDL_BlitScaled(optimizedSurface, NULL, screenSurface, &stretchRect);
						SDL_UpdateWindowSurface(window);
						background = false;
					}
					else
					{
						SDL_FillRect(screenSurface, 0, SDL_MapRGB(screenSurface->format, 0, 0, 0));
						SDL_UpdateWindowSurface(window);
						background = true;
					}
				}
				else if (event.key.keysym.sym == SDLK_1)
				{
					rect1.x = 0;
					SDL_BlitSurface(surface2, &rect1, screenSurface, &pos);
					SDL_UpdateWindowSurface(window);
				}
				else if (event.key.keysym.sym == SDLK_2)
				{
					rect1.x = 400;
					SDL_BlitSurface(surface2, &rect1, screenSurface, &pos);
					SDL_UpdateWindowSurface(window);
				}
				else if (event.key.keysym.sym == SDLK_3)
				{
					rect1.x = 800;
					SDL_BlitSurface(surface2, &rect1, screenSurface, &pos);
					SDL_UpdateWindowSurface(window);
				}
			}
		}
	}


	return 0;
}