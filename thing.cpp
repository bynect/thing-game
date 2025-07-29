#include <algorithm>

#include "thing.hpp"
#include "texture.hpp"


constexpr float GRAVITY = 0.001f;
constexpr float AIR_FRICTION = 0.0002f;
constexpr float DIRT_FRICTION = 0.008f;
constexpr float MAX_FALL_SPEED = 1.0f;
constexpr float MOVE_ACCEL = 0.02f;
constexpr float MAX_MOVE_SPEED = 0.4f;
constexpr float JUMP_SPEED = 0.5f;

void Thing::init(SDL_Renderer *renderer, float size)
{
    this->size = size;
    texture = load_texture(renderer, "assets/slime.png");
    pos = {0, 0};
    vel = {0, 0};

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

void Thing::move_input(float dir)
{
    accel.x = dir * MOVE_ACCEL;
    if (dir > 0) facing = F_RIGHT;
    else if (dir < 0) facing = F_LEFT;
}

void Thing::stop_input()
{
    accel.x = 0;
}

void Thing::jump()
{
    if (on_ground) {
        vel.y = -JUMP_SPEED;
        on_ground = false;
    }
}

void Thing::land()
{
    on_ground = true;
    vel.y = 0.f;
}

void Thing::render(SDL_Renderer *renderer, const SDL_FRect &camera)
{
    SDL_FRect dst = {
        .x = collider.rect.x - camera.x,
        .y = collider.rect.y - camera.y,
        .w = collider.rect.w,
        .h = collider.rect.h,
    };

    SDL_RendererFlip flip = facing == F_RIGHT ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    render_texture(renderer, texture, NULL, &dst, 0, NULL, flip);

    //std::cout << "displacement: " << displacement << std::endl;
    //std::cout << "x: " << dst.x << " + y: " << dst.y << std::endl;
}
