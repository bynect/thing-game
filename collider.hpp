#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>

struct Collider {
	Collider(SDL_FRect rect = {}, bool active = true) : rect(rect), active(active) {}

	static bool aabb(const SDL_FRect &a, const SDL_FRect &b)
	{
		return a.x + a.w >= b.x
			&& b.x + b.w >= a.x
			&& a.y + a.h >= b.y
			&& b.y + b.h >= a.y;
	}

	bool colliding(const SDL_FRect &other)
	{
		return active && aabb(rect, other);
	}

	bool colliding(const Collider &other)
	{
		return active && other.active && aabb(rect, other.rect);
	}

	SDL_FRect rect;
	bool active;
};
