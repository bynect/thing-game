#include <algorithm>

#include "thing.hpp"
#include "texture.hpp"

void Thing::init(SDL_Renderer *renderer, float size)
{
    texture = load_texture(renderer, "assets/slime.png");
    pos = {0, 0};
    vel = {0, 0};
    this->size = size;

    collider.rect.x = pos.x;
    collider.rect.y = pos.y;
    collider.rect.w = collider.rect.h = size;
}

void Thing::update(float delta)
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

void Thing::render(SDL_Renderer *renderer, const SDL_FRect &camera)
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
