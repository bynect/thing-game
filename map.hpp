#pragma once

#include <SDL2/SDL.h>
#include <array>

#include "vec2.hpp"
#include "collider.hpp"

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

const int MAP_WIDTH = 48;
const int MAP_HEIGHT = 24;

template<typename T, int X, int Y>
using Matrix = std::array<std::array<T, X>, Y>;

using MapScheme = Matrix<Material, MAP_WIDTH, MAP_HEIGHT>;

extern const MapScheme scheme_1;

struct Tile {
	Material material;
	Collider collider;
};

class Map {
public:
	void init(SDL_Renderer *renderer, int tile_size);

	void load_scheme(const MapScheme &scheme);

	void render(SDL_Renderer *renderer, Vec2<int> from = {0, 0}, Vec2<int> to = {MAP_WIDTH, MAP_HEIGHT});

	Tile *colliding(const Collider &other);

private:
	int tile_size;
	std::array<SDL_Texture *, M_COUNT> materials;
	Matrix<Tile, MAP_WIDTH, MAP_HEIGHT> tiles;
};
