#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "game.hpp"
#include "panic.hpp"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0)
    {
        std::cout << "Unable to initialize SDL2: " << SDL_GetError() << std::endl;
        panic();
    }

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        std::cout << "Unable to initialize SDL2_image: " << IMG_GetError() << std::endl;
        panic();
    }

    int width = 1600, height = 900;

    auto window = SDL_CreateWindow("thing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cout << "Unable to create SDL_Window: " << SDL_GetError() << std::endl;
        panic();
    }

    auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cout << "Unable to create SDL_Window: " << SDL_GetError() << std::endl;
        panic();
    }

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    Game game(width, height, renderer);

    const float time_freq = SDL_GetPerformanceFrequency();
    auto time_last = SDL_GetPerformanceCounter();

    const float frame_delta = 1000.0f / 60.0f;
    auto frame_time = time_last;
    auto frame_count = 0;


    while (game.running())
    {
        auto time_now = SDL_GetPerformanceCounter();
        float time_delta = (time_now - time_last) / time_freq * 1000.0f;

        game.events();

        if (time_now > frame_time + time_freq)
        {
            // Display FPS (1s passed)
            frame_count = 0;
            frame_time = time_now;
        }

        if (time_delta > frame_delta)
        {
            game.update(time_delta);
            game.render();

            time_last = time_now;
            ++frame_count;
        }
    }
}
