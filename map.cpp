#include "map.hpp"
#include "game.hpp"
#include "panic.hpp"
#include "texture.hpp"
#include <vector>

const char *material_texture_path[M_COUNT] = {
	/* M_VOID */ "",
	/* M_DIRT */ "assets/dirt.png",
	/* M_LAPIS */ "assets/lapis.png",
	/* M_COAL */ "assets/coal.png",
	/* M_GRASS */ "assets/grass.png",
	/* M_WATER */ "assets/water.png",
	/* M_FLOWER */ "assets/flower.png",
};

void Map::init(SDL_Renderer *renderer, int tile_size)
{
	this->tile_size = tile_size;

	for (int i = 0; i < M_COUNT; i++)
	{
		if (*material_texture_path[i] == '\0') continue;
		materials[i] = load_texture(renderer, material_texture_path[i]);
	}
}

void Map::load_scheme(const MapScheme &scheme)
{
	for (int row = 0; row < MAP_HEIGHT; row++)
	{
		for (int column = 0; column < MAP_WIDTH; column++)
		{
			auto &material = scheme[row][column];
			tiles[row][column].material = material;
			tiles[row][column].collider.active = material != M_VOID && material != M_FLOWER;
			tiles[row][column].collider.rect = {
				.x = float(column * tile_size),
				.y = float(row * tile_size),
				.w = float(tile_size),
				.h = float(tile_size),
			};
		}
	}
}

void Map::render(SDL_Renderer *renderer, const SDL_FRect &camera)
{
	SDL_SetRenderDrawColor(renderer, 212, 241, 249, 255);
	SDL_RenderClear(renderer);

//	std::cout << "map(from: " << from << ", to: " << to << ")" << std::endl;
//	std::cout << "map_scheme.size() = " << map_scheme.size() << " map_scheme[0].size() = " << map_scheme[0].size() << std::endl;

	int start_row = std::max(0, int(camera.y / tile_size));
    int end_row   = std::min(MAP_HEIGHT, int(camera.y + camera.h) / tile_size + 1);

    int start_col = std::max(0, int(camera.x / tile_size));
    int end_col   = std::min(MAP_WIDTH, int(camera.x + camera.w) / tile_size + 1);

    for (int row = start_row; row < end_row; row++) {
        for (int col = start_col; col < end_col; col++) {
            auto &tile = tiles[row][col];
            if (tile.material == M_VOID) continue;

            SDL_FRect dst = {
				.x = float(col * tile_size - camera.x),
				.y = float(row * tile_size - camera.y),
				.w = float(tile_size),
				.h = float(tile_size),
			};
            SDL_RenderCopyF(renderer, materials[tile.material], nullptr, &dst);
        }
    }
}

std::vector<Tile*> Map::colliding(const Collider &other)
{
    int approx_row = other.rect.y / tile_size;
    int min_row = std::max(0, approx_row - 1);
    int max_row = std::min(MAP_HEIGHT - 1, approx_row + 1);

    int approx_column = other.rect.x / tile_size;
    int min_column = std::max(0, approx_column - 1);
    int max_column = std::min(MAP_WIDTH - 1, approx_column + 1);

    std::vector<Tile*> colliding{};
	for (int row = min_row; row <= max_row; row++) {
		for (int column = min_column; column <= max_column; column++) {
			auto &tile = tiles[row][column];
			if (tile.collider.colliding(other))
                colliding.push_back(&tile);
		}
	}

	return colliding;
}

const MapScheme scheme_1 = {{
	{M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_VOID,    M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID},
	{M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_VOID,    M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID},
	{M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_VOID,    M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID},
	{M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_VOID,    M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID},
	{M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_VOID,    M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID},
	{M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_VOID,    M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID},
	{M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_VOID,    M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID},
	{M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_VOID,    M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID},
	{M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_VOID,    M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID},
	{M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_FLOWER,  M_FLOWER,  M_VOID,   M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID},
	{M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_FLOWER,  M_VOID,   M_GRASS,  M_GRASS,   M_GRASS,   M_VOID,   M_VOID,   M_VOID,   M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID},
	{M_VOID,    M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_FLOWER,  M_GRASS,  M_GRASS,  M_GRASS,   M_GRASS,  M_DIRT,   M_DIRT,    M_GRASS,   M_GRASS,  M_GRASS,  M_VOID,   M_FLOWER,  M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID},
	{M_FLOWER,  M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_GRASS,   M_GRASS,  M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,    M_DIRT,   M_DIRT,   M_GRASS,  M_GRASS,   M_VOID,   M_VOID,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS},
	{M_GRASS,   M_WATER,  M_WATER,  M_WATER,  M_WATER,  M_WATER,  M_GRASS,  M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_GRASS,   M_GRASS,  M_GRASS,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT},
	{M_DIRT,    M_WATER,  M_WATER,  M_WATER,  M_WATER,  M_WATER,  M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT},
	{M_DIRT,    M_DIRT,   M_WATER,  M_WATER,  M_WATER,  M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT},
	{M_DIRT,    M_DIRT,   M_WATER,  M_WATER,  M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT},
	{M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_LAPIS,  M_LAPIS,  M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT},
	{M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_LAPIS,  M_LAPIS,  M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT},
	{M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,    M_LAPIS,  M_DIRT,   M_LAPIS,  M_DIRT,    M_LAPIS,  M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_LAPIS,  M_DIRT,   M_DIRT,   M_DIRT,   M_COAL,   M_COAL,   M_DIRT,   M_DIRT,   M_DIRT},
	{M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_LAPIS,   M_LAPIS,  M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_COAL,   M_COAL,   M_DIRT,   M_DIRT,   M_DIRT},
	{M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,    M_COAL,   M_COAL,   M_COAL,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_COAL,   M_COAL,   M_DIRT,   M_DIRT},
	{M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_COAL,   M_DIRT,    M_COAL,   M_COAL,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_COAL,   M_DIRT,   M_DIRT},
	{M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_COAL,   M_COAL,    M_COAL,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT},
}};
