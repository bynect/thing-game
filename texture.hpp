#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>

#include "panic.hpp"

static inline void render_texture(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dst)
{
	if (SDL_RenderCopy(renderer, texture, src, dst) < 0)
	{
		panic(SDL_GetError());
	}
}

static inline void render_texture(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dst, double angle, SDL_Point *centre, SDL_RendererFlip flip)
{
	if (SDL_RenderCopyEx(renderer, texture, src, dst, angle, centre, flip) < 0)
	{
		panic(SDL_GetError());
	}
}

static inline SDL_Texture *load_texture(SDL_Renderer *renderer, const char *file)
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
