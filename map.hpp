#pragma once

#include <SDL2/SDL.h>
#include <array>

enum Material {
	M_DIRT,
	M_GRASS,
	M_AIR,
	M_WATER,
	M_FLOWER_BLANK,
	M_FLOWER,
	M_COUNT
};

const int MAP_HEIGHT = 20;
const int MAP_WIDTH = 20;

using MapScheme = std::array<std::array<Material, MAP_HEIGHT>, MAP_WIDTH>;

extern const MapScheme scheme_1;

class Map {
public:
	void init(SDL_Renderer *renderer);

	void load_scheme(const MapScheme &scheme);

	void render(SDL_Renderer *renderer);

private:
	std::array<SDL_Texture *, M_COUNT> materials;
	MapScheme map_scheme{};
};
