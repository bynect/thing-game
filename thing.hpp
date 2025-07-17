#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <cstddef>
#include <algorithm>

#include "vec2.hpp"
#include "texture.hpp"
#include "collider.hpp"

enum Facing {
    F_LEFT,
    F_RIGHT,
};

const float GRAVITY = 0.001f;
const float AIR_FRICTION = 0.0002f;
const float DIRT_FRICTION = 0.008f;
const float MAX_FALL_SPEED = 1.0f;
const float MOVE_ACCEL = 0.01f;
const float MAX_MOVE_SPEED = 0.4f;
const float JUMP_SPEED = 0.5f;

class Thing {
public:
    void init(SDL_Renderer *renderer, float size)
    {
        texture = load_texture(renderer, "assets/slime.png");
        pos = {0, 0};
        vel = {0, 0};
        this->size = size;

        collider.rect.x = pos.x;
        collider.rect.y = pos.y;
        collider.rect.w = collider.rect.h = size;
    }

    void update(float delta)
    {
        vel.y += GRAVITY * delta;
        if (vel.y > MAX_FALL_SPEED) vel.y = MAX_FALL_SPEED;

        vel.x += accel.x * delta;
        vel.x = std::clamp(vel.x, -MAX_MOVE_SPEED, MAX_MOVE_SPEED);

        float coeff = on_ground ? DIRT_FRICTION : AIR_FRICTION;
        apply_friction(vel.x, coeff, delta);

        pos.x += vel.x * delta;
        pos.y += vel.y * delta;

        collider.rect.x = pos.x;
        collider.rect.y = pos.y;
    }

    void render(SDL_Renderer *renderer, const SDL_FRect &camera)
    {
        SDL_FRect dst = {
			.x = collider.rect.x - camera.x,
			.y = collider.rect.y - camera.y,
			.w = float(collider.rect.w),
			.h = float(collider.rect.h),
		};

        SDL_RendererFlip flip = facing == F_RIGHT ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
        render_texture(renderer, texture, NULL, &dst, 0, NULL, flip);

        //std::cout << "displacement: " << displacement << std::endl;
        //std::cout << "x: " << dst.x << " + y: " << dst.y << std::endl;
    }

    void move_input(float dir)
    {
        accel.x = dir * MOVE_ACCEL;
        if (dir > 0) facing = F_RIGHT;
        else if (dir < 0) facing = F_LEFT;
    }

    void stop_input()
    {
        accel.x = 0;
    }

    void jump()
    {
        if (on_ground) {
            vel.y = -JUMP_SPEED;
            on_ground = false;
        }
    }

    void land()
    {
        on_ground = true;
        vel.y = 0.f;
    }

    Vec2<float> pos{};
    Vec2<float> vel{};
    float size;
    Facing facing = F_RIGHT;
    Collider collider;

private:
    SDL_Texture *texture;
    bool on_ground = false;
    Vec2<float> accel{0, 0};

    void apply_friction(float& v, float coeff, float delta)
    {
        if (v == 0.0f) return;
        float sign = (v > 0.0f) ? 1.0f : -1.0f;
        v -= sign * coeff * delta;
        if (v * sign < 0.0f) v = 0.0f;
    }
};
