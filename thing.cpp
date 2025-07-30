#include <algorithm>
#include <cfloat>
#include <cmath>

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

static constexpr float RESTITUTION   = 0.5f;
static constexpr float BOUNCE_CUTOFF = 0.08f;

void Thing::collisions(const Slice<Tile *> &colliding)
{
    for (auto hit : colliding)
    {
        SDL_FRect b = hit->collider.rect;
        SDL_FRect inter;
        SDL_IntersectFRect(&collider.rect, &b, &inter);

        if (inter.w < inter.h) {
            // Horizontal penetration
            if (pos.x < b.x)
                pos.x -= inter.w;
            else
                pos.x += inter.w;

            vel.x = 0.0f;
        } else {
            // Vertical penetration
            if (pos.y < b.y) {
                pos.y -= inter.h;
                land();
            } else
                pos.y += inter.h;

            vel.y = 0.0f;
        }
    }

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
    Vec2<float> threshold = {1, 1};
    auto diff = (pos - landing).abs();
    if (on_ground || diff < threshold) {
        vel.y -= JUMP_SPEED;
        on_ground = false;
    }
}

void Thing::land()
{
    on_ground = true;
    landing = pos;
    vel.y = 0.f;
}

void Thing::set_position(Vec2<float> pos)
{
    this->pos = pos;
    collider.rect.x = pos.x;
    collider.rect.y = pos.y;
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
}
