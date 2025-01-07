
#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <iostream>
int main(int argc, char* args[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    }

    SDL_Window* window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 800, SDL_WINDOW_SHOWN);

	Mix_Music* music = NULL;


	Mix_Chunk* sound1 = NULL;
	Mix_Chunk* sound2 = NULL;
	Mix_Chunk* sound3 = NULL;


    music = Mix_LoadMUS("beat.wav");
    if (music == NULL)
    {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
    }


    sound1 = Mix_LoadWAV("high.wav");
    if (sound1 == NULL)
    {
        printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
    }

    sound2 = Mix_LoadWAV("low.wav");
    if (sound2 == NULL)
    {
        printf("Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError());
    }

    sound3 = Mix_LoadWAV("medium.wav");
    if (sound3 == NULL)
    {
        printf("Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError());
    }

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
                if (event.key.keysym.sym == SDLK_1)
                {
                    Mix_PlayChannel(-1, sound1, 0);
                }
                else if (event.key.keysym.sym == SDLK_2)
                {
                    Mix_PlayChannel(-1, sound2, 0);
                }
                else if (event.key.keysym.sym == SDLK_3)
                {
                    Mix_PlayChannel(-1, sound3, 0);
                }
                else if (event.key.keysym.sym == SDLK_0)
                {
                    if (Mix_PlayingMusic() == 0)
                    {
                        Mix_PlayMusic(music, -1);
                    }
                    else
                    {
                        if (Mix_PausedMusic() == 1)
                        {
                            Mix_ResumeMusic();
                        }
                        else
                        {
                            Mix_PauseMusic();
                        }
                    }
                }
            }
        }
    }







    SDL_Quit();
    SDL_DestroyWindow(window);
	return 0;

}
