#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <random>
#include <utility>

#include "map.hpp"
#include "thing.hpp"

class Game {
public:
    Game(int width, int height, SDL_Renderer *renderer);

    void events();

    void update(float delta);

    void render(int fps);

    bool running() { return _running; }

    void camera_vertical(int tiles);

    void camera_horizontal(int tiles);

private:
    int window_width;
    int window_height;
    int tile_size;
    bool _running = true;

    std::random_device rand_device;
    std::mt19937 rand_generator;

    Map map;
    Thing thing;
    SDL_FRect camera;
    SDL_Renderer *renderer;
};
