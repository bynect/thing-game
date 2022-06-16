#include <iostream>
#include <SDL2/SDL.h>
#include <random>

#include "game.hpp"
#include "map.hpp"
#include "panic.hpp"
#include "thing.hpp"
#include "vec2.hpp"

const float VEL = 0.25;

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

	camera_origin = {0, 0};
	camera_size = {16 * 2, 9 * 2};

	tile_size = width / camera_size.x;

	map.init(renderer, tile_size);
	map.load_scheme(scheme_1);

	thing.init(renderer, tile_size);
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

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case 'w':
						thing.increase_vel({0, -VEL});
						break;

					case 'a':
						thing.facing = F_LEFT;
						thing.increase_vel({-VEL, 0});
						break;

					case 's':
						thing.increase_vel({0, VEL});
						break;

					case 'd':
						thing.facing = F_RIGHT;
						thing.increase_vel({VEL, 0});
						break;

					default:
						break;
				}
				break;

			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
					case ' ':
						thing.jump();
						break;

					case 'w':
						thing.increase_vel({0, VEL});
						break;

					case 'a':
						thing.increase_vel({VEL, 0});
						break;

					case 's':
						thing.increase_vel({0, -VEL});
						break;

					case 'd':
						thing.increase_vel({-VEL, 0});
						break;

					case SDLK_UP:
						camera_vertical(-1);
						break;

					case SDLK_LEFT:
						camera_horizontal(-1);
						break;

					case SDLK_DOWN:
						camera_vertical(1);
						break;

					case SDLK_RIGHT:
						camera_horizontal(1);
						break;

					default:
						break;
				}
				break;

    		default:
    			break;
    	}
    }
}

void Game::update(float delta)
{
	thing.update(delta);

	if (thing.pos.x < 0)
	{
		thing.pos.x = 0;
		thing.vel = {0, 0};
	}

	if (thing.pos.y < 0)
	{
		thing.pos.y = 0;
		thing.vel = {0, 0};
	}

	if ((thing.pos.x + thing.size) >= MAP_WIDTH * tile_size)
	{
		thing.pos.x = (MAP_WIDTH - 1) * tile_size - thing.size;
		thing.vel = {0, 0};
	}

	if ((thing.pos.y + thing.size) >= MAP_HEIGHT * tile_size)
	//if ((thing.pos.y + thing.size) >= window_height)//MAP_WIDTH * tile_size)
	{
		//thing.pos.y = window_height - thing.size;
		thing.pos.y = (MAP_HEIGHT - 1) * tile_size - thing.size;
		thing.vel = {0, 0};
	}
}

void Game::render()
{
	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	//SDL_RenderClear(renderer);

	Vec2<int> camera_from = {camera_origin.x, camera_origin.y};
	Vec2<int> camera_to = {camera_from.x + camera_size.x, camera_from.y + camera_size.y};
	map.render(renderer, camera_from, camera_to);

	thing.render(renderer, camera_origin * tile_size);

	SDL_RenderPresent(renderer);
}

void Game::camera_vertical(int tiles)
{
	int new_origin = camera_origin.y + tiles;

//	std::cout << "vertical(tiles: " << tiles << ", origin: " << camera_origin << ", new_origin_y: " << new_origin << ")" << std::endl;

	if (new_origin >= 0 && (new_origin + camera_size.y) < MAP_HEIGHT)
	{
		camera_origin.y = new_origin;
	}
}

void Game::camera_horizontal(int tiles)
{
	int new_origin = camera_origin.x + tiles;

//	std::cout << "horizontal(tiles: " << tiles << ", origin: " << camera_origin << ", new_origin_x: " << new_origin << ")" << std::endl;

	if (new_origin >= 0 && (new_origin + camera_size.x) < MAP_WIDTH)
	{
		camera_origin.x = new_origin;
	}
}
