#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <random>
#include <utility>

#include "map.hpp"
#include "thing.hpp"

class Game {
public:
    Game(const char *name, int width, int height, int w_flags = SDL_WINDOW_SHOWN, int r_flags = SDL_RENDERER_ACCELERATED);
    ~Game();

    void events();

    void update(float delta);

    void render();

    bool running() { return _running; }

    void camera_vertical(int tiles);

    void camera_horizontal(int tiles);

private:
    int window_width;
    int window_height;
    int tile_size;
    bool _running = true;

    SDL_Window *window;
    SDL_Renderer *renderer;

    std::random_device rand_device;
    std::mt19937 rand_generator;

    Map map;
    Thing thing;
    SDL_FRect camera;
};
