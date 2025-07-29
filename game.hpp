#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <random>
#include <vector>

#include "map.hpp"
#include "thing.hpp"

class Game {
public:
    Game(int width, int height, SDL_Renderer *renderer);

    void events();

    void update(float delta);

    void render();

    bool running() { return is_running; }

    void camera_vertical(int tiles);

    void camera_horizontal(int tiles);

private:
    void render_menu();

    int window_width;
    int window_height;
    int tile_size;

    bool is_running = true;
    bool show_colliders = false;
    std::vector<Tile *> hits;

    std::random_device rand_device;
    std::mt19937 rand_generator;

    Map map;
    Thing thing;
    SDL_FRect camera;
    SDL_Renderer *renderer;
};
