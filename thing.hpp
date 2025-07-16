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

		//std::cout << "displacement: " << displacement << std::endl;
		//std::cout << "x: " << dst.x << " + y: " << dst.y << std::endl;
	}

	void increase_vel(Vec2<float> v)
	{
		vel += v;
	}

	void decrease_vel(Vec2<float> v)
	{
		vel -= v;
	}

	void jump()
	{
		if (on_ground)
		{
			jump_vel = {0.65f * (facing == F_RIGHT ? 1 : -1), -0.40};
			vel += jump_vel;
			on_ground = false;
		}
	}

	void land()
	{
		if (!on_ground)
		{
			on_ground = true;
			vel -= jump_vel;
		}
	}

	Vec2<float> pos{};

	Vec2<float> vel{};

	int size;

	Facing facing = F_RIGHT;

	Collider collider;

private:
	SDL_Texture *texture;

	bool on_ground = false;
	Vec2<float> jump_vel{};
};
