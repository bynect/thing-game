#include "map.hpp"
#include "game.hpp"
#include "panic.hpp"
#include "texture.hpp"

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
				.x = column * tile_size,
				.y = row * tile_size,
				.w = tile_size,
				.h = tile_size,
			};
		}
	}
}

void Map::render(SDL_Renderer *renderer, Vec2<int> from, Vec2<int> to)
{
	SDL_SetRenderDrawColor(renderer, 212, 241, 249, 255);
	SDL_RenderClear(renderer);

//	std::cout << "map(from: " << from << ", to: " << to << ")" << std::endl;
//	std::cout << "map_scheme.size() = " << map_scheme.size() << " map_scheme[0].size() = " << map_scheme[0].size() << std::endl;

	for (int row = from.y; row < to.y; row++)
	{
		for (int column = from.x; column < to.x; column++)
		{
			//SDL_Rect dst = {
			//	.x = (column - from.x) * tile_size,
			//	.y = (row - from.y) * tile_size,
			//	.w = tile_size,
			//	.h = tile_size,
			//};

			auto &tile = tiles[row][column];
			if (tile.material == M_VOID) continue;

			tile.collider.rect.x = (column - from.x) * tile_size;
			tile.collider.rect.y = (row - from.y) * tile_size;
			render_texture(renderer, materials[tile.material], NULL, &tile.collider.rect);
		}
	}
}

Tile *Map::colliding(const Collider &other)
{
	for (int row = 0; row < MAP_HEIGHT; row++)
	{
		for (int column = 0; column < MAP_WIDTH; column++)
		{
			auto &tile = tiles[row][column];
			if (tile.collider.colliding(other))
			{
				return &tiles[row][column];
			}
		}
	}
	return nullptr;
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
	{M_FLOWER,  M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_VOID,   M_GRASS,   M_GRASS,  M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,    M_DIRT,   M_DIRT,   M_GRASS,  M_GRASS,   M_VOID,   M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS,  M_GRASS},
	{M_GRASS,   M_WATER,  M_WATER,  M_WATER,  M_WATER,  M_WATER,  M_GRASS,  M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_GRASS,   M_GRASS,  M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT},
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
