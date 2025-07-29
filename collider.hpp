#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

struct Collider {
    Collider(SDL_FRect rect = {}, bool active = true)
        : rect(rect), active(active) {}

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

    void render(SDL_Renderer *renderer, const SDL_FRect &camera)
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_FRect dst = {
            .x = rect.x - camera.x,
            .y = rect.y - camera.y,
            .w = rect.w,
            .h = rect.h,
        };
        SDL_RenderDrawRectF(renderer, &dst);
    }

    SDL_FRect rect;
    bool active;
};
