#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <random>
#include <iostream>



int SCREENX = 1200;
int SCREENY = 800;

struct Shape : SDL_Rect
{
    int r;
};

class Entity
{
public:
    SDL_Surface* sprite;
    SDL_Surface* hitsprite;

    void move();

    Shape hitbox;

    int w = 100;
    int h = 100;

    double posX = 0;
    double posY = 0;

    double velX = 0;
    double velY = 0;
};


bool checkCollisionCircle(Shape a, Shape b);

bool checkCollisionBox(Shape a, Shape b);

void Entity::move()
{
    posX += velX;
    posY += velY;

    hitbox.x = posX;
    hitbox.y = posY;

    hitbox.w = w;
    hitbox.h = h;

    if (posX > SCREENX)
    {
        posX = 0;
    }


    if (posY > SCREENY)
    {
        posY = 0;
    }

}

double distanceSquared(int x1, int y1, int x2, int y2);



bool checkCollisionCircle(Shape a, Shape b)
{
    //Calculate total radius squared
    int totalRadiusSquared = a.r + b.r;
    totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

    //If the distance between the centers of the circles is less than the sum of their radii
    if (distanceSquared(a.x, a.y, b.x, b.y) < (totalRadiusSquared))
    {
        //The circles have collided
        return true;
    }

    //If not
    return false;
}

double distanceSquared(int x1, int y1, int x2, int y2)
{
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    return deltaX * deltaX + deltaY * deltaY;
}

bool checkCollisionBox(Shape a, Shape b)
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if (bottomA <= topB)
    {
        return false;
    }

    if (topA >= bottomB)
    {
        return false;
    }

    if (rightA <= leftB)
    {
        return false;
    }

    if (leftA >= rightB)
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}













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



	SDL_Window* window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREENX, SCREENY, SDL_WINDOW_SHOWN);

	SDL_Surface* screenSurface = SDL_GetWindowSurface(window);

    SDL_Surface* box = IMG_Load("box.png");
    SDL_Surface* boxhit = IMG_Load("boxhit.png");

    SDL_Surface* circle = IMG_Load("circle.png");
    SDL_Surface* circlehit = IMG_Load("circlehit.png");


    Shape hitbox1;
    hitbox1.x = 0;
    hitbox1.y = 0;
    hitbox1.r = 50;
    hitbox1.w = 0;
    hitbox1.h = 0;
    Shape hitbox2 = hitbox1;

    Entity* entity = new Entity();
    entity->sprite = box;
    entity->hitbox = hitbox1;
    entity->posX = SCREENX / 2;
    entity->posY = SCREENY / 2;


    Entity* entityMoving = new Entity();
    entityMoving->sprite = box;
    entityMoving->hitsprite = boxhit;
    entityMoving->hitbox = hitbox2;
    entityMoving->posX = SCREENX / 2 - 150;
    entityMoving->posY = SCREENY / 2 + 75;
    entityMoving->velX = 0.05;


    SDL_Event event;
    bool running = true;
    auto collisionCheck = checkCollisionBox;

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
                    entity->sprite = box;

                    entityMoving->sprite = box;
                    entityMoving->hitsprite = boxhit;

                    collisionCheck = checkCollisionBox;
                }
                else if (event.key.keysym.sym == SDLK_2)
                {
                    entity->sprite = circle;

                    entityMoving->sprite = circle;
                    entityMoving->hitsprite = circlehit;

                    collisionCheck = checkCollisionCircle;
                }
                else if (event.key.keysym.sym == SDLK_3)
                {
                    entityMoving->velX = 0.05;
                    entityMoving->velY = 0;
                }
                else if (event.key.keysym.sym == SDLK_4)
                {
                    entityMoving->velX = 0;
                    entityMoving->velY = 0.05;
                }

            }
        }

        entity->move();
        entityMoving->move();

        
     
        SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 0));

        SDL_BlitSurface(entity->sprite, NULL, screenSurface, &entity->hitbox);


        if (collisionCheck(entity->hitbox, entityMoving->hitbox))
        {
            SDL_BlitSurface(entityMoving->hitsprite, NULL, screenSurface, &entityMoving->hitbox);
        }
        else
        {
            SDL_BlitSurface(entityMoving->sprite, NULL, screenSurface, &entityMoving->hitbox);
        }

        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
	

    return 0;

}