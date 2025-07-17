#include <SDL2/SDL_rect.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <random>

#include "game.hpp"
#include "map.hpp"
#include "panic.hpp"
#include "thing.hpp"
#include "vec2.hpp"

const float VEL = 0.25;

Game::Game(const char *name, int width, int height, int w_flags, int r_flags) : window_width(width), window_height(height), rand_generator(rand_device())
{
    window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, w_flags);

    if (window == nullptr)
    {
        std::cout << "Unable to create SDL_Window: " << SDL_GetError() << std::endl;
        panic();
    }

    renderer = SDL_CreateRenderer(window, -1, r_flags);

    if (renderer == nullptr)
    {
        std::cout << "Unable to create SDL_Window: " << SDL_GetError() << std::endl;
        panic();
    }

    camera_origin = {0, 0};
    camera_size = {16 * 2, 9 * 2};

    tile_size = width / camera_size.x;

    map.init(renderer, tile_size);
    map.load_scheme(scheme_1);

    thing.init(renderer, tile_size);
}

Game::~Game()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void Game::events()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                _running = false;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_a:
                        thing.move_input(-1.0f);
                        break;

                    case SDLK_d:
                        thing.move_input(1.0f);
                        break;

                    case SDLK_SPACE:
                        thing.jump();
                        break;

                    case SDLK_UP:
                        camera_vertical(-1);
                        break;

                    case SDLK_LEFT:
                        camera_horizontal(-1);
                        break;

                    case SDLK_DOWN:
                        camera_vertical(1);
                        break;

                    case SDLK_RIGHT:
                        camera_horizontal(1);
                        break;

                    default:
                        break;
                }
                break;

            case SDL_KEYUP:
                switch (event.key.keysym.sym)
                {
                    case SDLK_a:
                    case SDLK_d:
                        thing.stop_input();
                        break;

                    default:
                        break;
                }
                break;

            default:
                break;
        }
    }
}

void Game::update(float delta)
{
    thing.update(delta);

    for (auto colliding : map.colliding(thing.collider))
    {
        //std::cout << "cx " << colliding->collider.rect.x << " cy " << colliding->collider.rect.y << " on " << colliding->collider.active << std::endl;
        //std::cout << "vel " << thing.vel << ", pos " << thing.pos << std::endl;

        SDL_Rect a = thing.collider.rect, b = colliding->collider.rect;
        SDL_Rect inter;
        SDL_IntersectRect(&a, &b, &inter);

        if (inter.w < inter.h) {
            // Horizontal penetration
            if (thing.pos.x < b.x)
                thing.pos.x -= inter.w;
            else
                thing.pos.x += inter.w;

            thing.vel.x = 0.0f;
        } else {
            // Vertical penetration
            if (thing.pos.y < b.y) {
                thing.pos.y -= inter.h;
                thing.land();
            } else
                thing.pos.y += inter.h;

            thing.vel.y = 0.0f;
        }

        thing.collider.rect.x = thing.pos.x;
        thing.collider.rect.y = thing.pos.y;
    }

    if (thing.pos.x < 0) {
        thing.pos.x = 0;
        thing.vel = {0, 0};
    }

    if (thing.pos.y < 0) {
        thing.pos.y = 0;
        thing.vel = {0, 0};
    }

    if ((thing.pos.x + thing.size) > MAP_WIDTH * tile_size) {
        thing.pos.x = MAP_WIDTH * tile_size - thing.size;
        thing.vel = {0, 0};
    }

    if ((thing.pos.y + thing.size) > MAP_HEIGHT * tile_size) {
        thing.pos.y = MAP_HEIGHT * tile_size - thing.size;
        thing.vel = {0, 0};
    }


    //SDL_Rect camera_rect = {
    //  .x = camera_origin.x,
    //  .y = camera_origin.y,
    //  .w = camera_size.x,
    //  .h = camera_size.y,
    //};

    //if (!Collider::aabb(camera_rect, thing.collider.rect))
    //{
    //  if (camera_origin.y > thing.pos.y)
    //  {
    //      camera_vertical(-1);
    //  }
    //  else if (camera_origin.y < thing.pos.y)
    //  {
    //      camera_vertical(1);
    //  }

    //  if (camera_origin.x > thing.pos.x)
    //  {
    //      camera_horizontal(-1);
    //  }
    //  else if (camera_origin.x < thing.pos.x)
    //  {
    //      camera_horizontal(1);
    //  }
    //}
}

void Game::render()
{
    map.render(renderer, camera_origin, camera_origin + camera_size);
    thing.render(renderer, camera_origin * tile_size);
    SDL_RenderPresent(renderer);
}

void Game::camera_vertical(int tiles)
{
    int new_origin = camera_origin.y + tiles;

//  std::cout << "vertical(tiles: " << tiles << ", origin: " << camera_origin << ", new_origin_y: " << new_origin << ")" << std::endl;

    if (new_origin >= 0 && (new_origin + camera_size.y) < MAP_HEIGHT)
    {
        camera_origin.y = new_origin;
    }
}

void Game::camera_horizontal(int tiles)
{
    int new_origin = camera_origin.x + tiles;

//  std::cout << "horizontal(tiles: " << tiles << ", origin: " << camera_origin << ", new_origin_x: " << new_origin << ")" << std::endl;

    if (new_origin >= 0 && (new_origin + camera_size.x) < MAP_WIDTH)
    {
        camera_origin.x = new_origin;
    }
}
