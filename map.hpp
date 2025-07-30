#pragma once

#include <SDL2/SDL.h>
#include <array>

#include "collider.hpp"
#include "util.hpp"
#include "vec2.hpp"

enum Material {
    M_VOID,
    M_DIRT,
    M_LAPIS,
    M_COAL,
    M_GRASS,
    M_WATER,
    M_FLOWER,
    M_COUNT
};

struct Tile {
    Material material;
    Collider collider;
};

class Map {
public:
    void init(SDL_Renderer *renderer, int tile_size);

    bool load_file(std::string path);

    void render(SDL_Renderer *renderer, const SDL_FRect &camera);

    Slice<Tile*> colliding(const Collider &other, Tile *(&scratch)[8]);

    size_t width() const { return tiles.columns; }

    size_t height() const { return tiles.rows; }

    Vec2<float> spawn() const { return spawn_pos; }

    const std::string& file_path() const { return path; }

private:
    int tile_size;
    std::string path;
    std::array<SDL_Texture *, M_COUNT> materials;
    Matrix<Tile> tiles{0, 0};
    Vec2<float> spawn_pos{0, 0};
};
