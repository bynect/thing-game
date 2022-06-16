#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <cstddef>

#include "vec2.hpp"
#include "texture.hpp"
#include "collider.hpp"

enum Facing {
	F_LEFT,
	F_RIGHT,
};

const float GRAVITY = 0.0008;
const float AIR_FRICTION = 0.0008;
const float DIRT_FRICTION = 0.001;

class Thing {
public:
	void init(SDL_Renderer *renderer, int size)
	{
		texture = load_texture(renderer, "assets/slime.png");
		pos = {(float)size, (float)size};
		vel = {0, 0};
		this->size = size;

		collider.rect.x = pos.x;
		collider.rect.y = pos.y;
		collider.rect.w = collider.rect.h = size;
	}

	void update(float delta)
	{
		vel.y += GRAVITY * delta;
		pos += vel * delta;

		collider.rect.x = pos.x;
		collider.rect.y = pos.y;
	}

	void render(SDL_Renderer *renderer, Vec2<int> displacement)
	{
		SDL_Rect dst = collider.rect;
		dst.x -= displacement.x;
		dst.y -= displacement.y;
		SDL_RendererFlip flip = facing == F_RIGHT ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
		render_texture(renderer, texture, NULL, &dst, 0, NULL, flip);
	}

	void increase_vel(Vec2<float> v)
	{
		vel += v;
	}

	void jump()
	{
		if (on_ground)
		{
			vel += {0.65f * (facing == F_RIGHT ? 1 : -1), -0.40};
			on_ground = false;
		}
	}

	Vec2<float> pos{};

	Vec2<float> vel{};

	int size;

	Facing facing = F_RIGHT;

	Collider collider;

	bool on_ground = false;

private:
	SDL_Texture *texture;
};
