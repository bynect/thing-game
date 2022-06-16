#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <cstddef>

#include "vec2.hpp"
#include "texture.hpp"

enum Facing {
	F_LEFT,
	F_RIGHT,
};

class Thing {
public:
	void init(SDL_Renderer *renderer, int size)
	{
		texture = load_texture(renderer, "assets/slime.png");
		pos = {(float)size, (float)size};
		vel = {0, 0};
		this->size = size;
	}

	void update(float delta)
	{
		if (pos.y < 600)
		{
			const float G = 0.0008;
			vel.y += G * delta;
		}
		pos += vel * delta;
	}

	void render(SDL_Renderer *renderer, Vec2<int> displacement)
	{
		SDL_Rect dst = {
			.x = (int)pos.x - displacement.x,
			.y = (int)pos.y - displacement.y,
			.w = size,
			.h = size,
		};
		SDL_RendererFlip flip = facing == F_RIGHT ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
		render_texture(renderer, texture, NULL, &dst, 0, NULL, flip);
	}

	void increase_vel(Vec2<float> v)
	{
		vel += v;
	}

	void jump()
	{
		vel += {0.5 * (facing == F_RIGHT ? 1 : -1), -0.25};
	}

	Vec2<float> pos{};

	Vec2<float> vel{};

	int size;

	Facing facing = F_RIGHT;

private:
	SDL_Texture *texture;
};
