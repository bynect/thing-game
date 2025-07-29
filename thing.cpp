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

static constexpr float RESTITUTION   = 0.4f;
static constexpr float BOUNCE_CUTOFF = 0.08f;

void Thing::collisions(const Slice<Tile *> &colliding)
{
    for (auto hit : colliding)
    {
        const SDL_FRect& b = hit->collider.rect;
        SDL_FRect inter;
        SDL_IntersectFRect(&collider.rect, &b, &inter);

        // Only handle X‐penetrations here:
        if (inter.w > 0.0f && inter.w < inter.h)
        {
            // push slime out of the wall
            if (vel.x > 0) pos.x -= inter.w;
            else           pos.x += inter.w;

            // reflect & damp horizontal velocity
            vel.x = -vel.x * RESTITUTION;

            // update collider for next passes
            collider.rect.x = pos.x;
        }
    }

    for (auto hit : colliding)
    {
        const SDL_FRect& b = hit->collider.rect;
        SDL_FRect inter;
        SDL_IntersectFRect(&collider.rect, &b, &inter);

        // Only handle Y‐penetrations here:
        if (inter.h > 0.0f && inter.h <= inter.w)
        {
            if (vel.y > 0.0f)
            {
                // hitting the floor
                pos.y -= inter.h;
                vel.y  = -vel.y * RESTITUTION;
            }
            else
            {
                // hitting the ceiling
                pos.y += inter.h;
                vel.y  = -vel.y * RESTITUTION;
            }

            if (std::abs(vel.y) < BOUNCE_CUTOFF)
                land();

            // update collider
            collider.rect.y = pos.y;
        }
    }
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
