#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
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

    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "0");
    auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cout << "Unable to create SDL_Window: " << SDL_GetError() << std::endl;
        panic();
    }

    SDL_GL_SetSwapInterval(0);
    SDL_RendererInfo info;
    if (SDL_GetRendererInfo(renderer, &info)) {
        std::cout << "Unable to fetch RendererInfo: " << SDL_GetError() << std::endl;
    } else {
        std::cout << "RendererInfo:" << std::endl;
        std::cout << "\tName: " << info.name << std::endl;
        std::cout << "\tFlags: " << info.flags << std::endl;
        std::cout << "\tTexture Formats: " << info.num_texture_formats << std::endl;
        std::cout << "\tMax Texture Width: " << info.max_texture_width << std::endl;
        std::cout << "\tMax Texture Height: " << info.max_texture_height << std::endl;
    }

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui::GetIO().IniFilename = nullptr;

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    Game game(width, height, renderer);

    const float time_freq = SDL_GetPerformanceFrequency();
    auto time_last = SDL_GetPerformanceCounter();

    auto frame_time = time_last;
    int frame_count = 0;
    int fps = 0;

    while (game.running())
    {
        auto time_now = SDL_GetPerformanceCounter();
        float elapsed_ms = (time_now - time_last) / time_freq * 1000.0f;
        time_last = time_now;

        frame_count++;
        if (time_now > frame_time + time_freq)
        {
            fps = frame_count;
            frame_count = 0;
            frame_time = time_now;
        }

        game.events();
        game.update(elapsed_ms);
        game.render(ImGui::GetIO().Framerate);

        SDL_Delay(0);
    }

    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}
