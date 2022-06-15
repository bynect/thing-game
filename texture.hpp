#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>

#include "panic.hpp"

inline SDL_Texture *load_texture(SDL_Renderer *renderer, const char *file)
{
	SDL_Surface *surface = IMG_Load(file);
	if (surface == nullptr)
	{
		panic(IMG_GetError());
	}


	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}
