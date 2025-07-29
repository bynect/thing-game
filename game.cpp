#include <SDL2/SDL_rect.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <random>
#include <algorithm>

#include "game.hpp"
#include "map.hpp"
#include "thing.hpp"
#include "vec2.hpp"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

Game::Game(int width, int height, SDL_Renderer *renderer) :  window_width(width), window_height(height), rand_generator(rand_device()), renderer(renderer)
{
    constexpr int SCALE = 16 * 2;
    tile_size = width / SCALE;

    camera = {
        .x = 0,
        .y = 0,
        .w = float(16 * 2 * tile_size),
        .h = float(9 * 2 * tile_size),
    };

    thing.init(renderer, tile_size);
    map.init(renderer, tile_size);
    map.load_scheme(scheme_1);
}

void Game::events()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:
                is_running = false;
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
                }
                break;

            case SDL_KEYUP:
                switch (event.key.keysym.sym)
                {
                    case SDLK_a:
                    case SDLK_d:
                        thing.stop_input();
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

    Tile *tiles[8];
    for (auto colliding : map.colliding(thing.collider, tiles))
    {
        //std::cout << "cx " << colliding->collider.rect.x << " cy " << colliding->collider.rect.y << " on " << colliding->collider.active << std::endl;
        //std::cout << "vel " << thing.vel << ", pos " << thing.pos << std::endl;

        SDL_FRect a = thing.collider.rect, b = colliding->collider.rect;
        SDL_FRect inter;
        SDL_IntersectFRect(&a, &b, &inter);

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

    if ((thing.pos.x + thing.size) > MAP_WIDTH * tile_size) {
        thing.pos.x = MAP_WIDTH * tile_size - thing.size;
        thing.vel.x = 0;
    } else if (thing.pos.x < 0) {
        thing.pos.x = 0;
        thing.vel.x = 0;
    }

    if ((thing.pos.y + thing.size) > MAP_HEIGHT * tile_size) {
        thing.pos.y = MAP_HEIGHT * tile_size - thing.size;
        thing.vel.y = 0.0f;
    } else if (thing.pos.y < 0) {
        thing.pos.y = 0;
        thing.vel.y = 0;
    }

    Vec2<float> target = {
        (thing.pos.x + thing.size * 0.5f) - (camera.w * 0.5f),
        (thing.pos.y + thing.size * 0.5f) - (camera.h * 0.5f),
    };

    target.x = std::clamp(target.x, 0.0f, MAP_WIDTH * tile_size - camera.w);
    target.y = std::clamp(target.y, 0.0f, MAP_HEIGHT * tile_size - camera.h);

    constexpr float SMOOTH_SPEED = 0.01f;
    float alpha = 1.0f - std::exp(-SMOOTH_SPEED * delta);

    static Vec2<float> camera_pos = {camera.x, camera.y};
    camera_pos += (target - camera_pos) * alpha;

    camera.x = camera_pos.x;
    camera.y = camera_pos.y;

    //std::cout << "camerax " << camera.x << " cameray " << camera.y << std::endl;
}

void Game::render(int fps)
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    map.render(renderer, camera);
    thing.render(renderer, camera);

    ImGui::Begin("Debug");
    ImGui::Text("FPS: %d", fps);
    ImGui::Checkbox("Show Colliders", &show_colliders);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);

    SDL_RenderPresent(renderer);
}
