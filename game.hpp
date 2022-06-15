#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <memory>
#include <random>

#include "panic.hpp"
#include "map.hpp"

class Game {
public:
	Game(const char *name, int width, int height, int w_flags = SDL_WINDOW_SHOWN, int r_flags = SDL_RENDERER_ACCELERATED);
	~Game();

	void events();

	void update(float delta);

	void render();

	bool running() { return _running; }

	static void render_texture(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dst)
	{
		if (SDL_RenderCopy(renderer, texture, src, dst) < 0)
		{
			panic(SDL_GetError());
		}
	}

private:
	int window_width;
	int window_height;
	bool _running = true;

	SDL_Window *window;
	SDL_Renderer *renderer;

	std::random_device rand_device;
	std::mt19937 rand_generator;

	Map map;

	void render_texture(SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dst)
	{
		render_texture(renderer, texture, src, dst);
	}
};
