#include <SDL2/SDL_rect.h>
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
    auto colliding = map.colliding(thing.collider, tiles);

    if (show_colliders) {
        hits.clear();
        hits.insert(hits.end(), colliding.begin(), colliding.end());
    }

    thing.collisions(colliding);

    // Clamp to world width
    if ((thing.pos.x + thing.size) > MAP_WIDTH * tile_size) {
        thing.pos.x = MAP_WIDTH * tile_size - thing.size;
        thing.vel.x = 0;
    } else if (thing.pos.x < 0) {
        thing.pos.x = 0;
        thing.vel.x = 0;
    }

    // Clamp to world height
    if ((thing.pos.y + thing.size) > MAP_HEIGHT * tile_size) {
        thing.pos.y = MAP_HEIGHT * tile_size - thing.size;
        thing.vel.y = 0.0f;
    } else if (thing.pos.y < 0) {
        thing.pos.y = 0;
        thing.vel.y = 0;
    }

    // Follow player with camera
    Vec2<float> target = {
        std::clamp(
            (thing.pos.x + thing.size * 0.5f) - (camera.w * 0.5f),
            0.0f,
            MAP_WIDTH * tile_size - camera.w
        ),
        std::clamp(
            (thing.pos.y + thing.size * 0.5f) - (camera.h * 0.5f),
            0.0f,
            MAP_HEIGHT * tile_size - camera.h
        ),
    };

    constexpr float SMOOTH_SPEED = 0.1f;
    float alpha = 1.0f - std::exp(-SMOOTH_SPEED * delta);
    camera.x += (target.x - camera.x) * alpha;
    camera.y += (target.y - camera.y) * alpha;
}

void Game::render()
{
    map.render(renderer, camera);
    thing.render(renderer, camera);

    if (show_colliders) {
        for (auto hit : hits) {
            hit->collider.render(renderer, camera);
        }
        thing.collider.render(renderer, camera);
    }

    render_menu();
    SDL_RenderPresent(renderer);
}

void Game::render_menu()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    int fps = ImGui::GetIO().Framerate;
    ImGui::Begin("Debug");
    ImGui::Text("FPS: %d", fps);
    ImGui::Text("Thing X: %f", thing.collider.rect.x);
    ImGui::Text("Thing Y: %f", thing.collider.rect.y);
    ImGui::Text("Camera X: %f", camera.x);
    ImGui::Text("Camera Y: %f", camera.y);
    ImGui::Checkbox("Show Colliders", &show_colliders);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
}
