#include <iostream>
#include <SDL2/SDL.h>
#include <random>

#include "game.hpp"
#include "panic.hpp"

Game::Game(const char *name, int width, int height, int w_flags, int r_flags) : window_width(width), window_height(height), rand_generator(rand_device())
{
	window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, w_flags);

	if (window == nullptr)
	{
		std::cout << "Unable to create SDL_Window: " << SDL_GetError() << std::endl;
		panic();
	}

	renderer = SDL_CreateRenderer(window, -1, r_flags);

	if (renderer == nullptr)
	{
		std::cout << "Unable to create SDL_Window: " << SDL_GetError() << std::endl;
		panic();
	}

	map.init(renderer);
	map.load_scheme(scheme_1);
}

Game::~Game()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void Game::events()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
    	switch (event.type)
    	{
    		case SDL_QUIT:
    			_running = false;
    			break;

			case SDL_KEYUP:
				if (event.key.keysym.sym == ' ')
				{
				}
				break;

    		default:
    			break;
    	}
    }
}

void Game::update(float delta)
{
}

void Game::render()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	map.render(renderer);

	SDL_RenderPresent(renderer);
}
