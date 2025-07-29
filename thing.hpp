#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

#include "vec2.hpp"
#include "collider.hpp"

enum Facing {
    F_LEFT,
    F_RIGHT,
};

constexpr float GRAVITY = 0.001f;
constexpr float AIR_FRICTION = 0.0002f;
constexpr float DIRT_FRICTION = 0.008f;
constexpr float MAX_FALL_SPEED = 1.0f;
constexpr float MOVE_ACCEL = 0.02f;
constexpr float MAX_MOVE_SPEED = 0.4f;
constexpr float JUMP_SPEED = 0.5f;

class Thing {
public:
    void init(SDL_Renderer *renderer, float size);

    void update(float delta);

    void render(SDL_Renderer *renderer, const SDL_FRect &camera);

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
