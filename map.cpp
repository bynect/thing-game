#include "map.hpp"
#include "game.hpp"
#include "panic.hpp"
#include "texture.hpp"

const char *material_texture_path[M_COUNT] = {
	/* DIRT */ "assets/dirt.png",
	/* GRASS */ "assets/grass.png",
	/* AIR */ "assets/air.png",
	/* WATER */ "assets/water.png",
	/* FLOWER_BLANK */ "assets/flower.png",
	/* FLOWER */ "",
};

void Map::init(SDL_Renderer *renderer)
{
	for (int i = 0; i < M_COUNT; i++)
	{
		if (*material_texture_path[i] == '\0') continue;
		materials[i] = load_texture(renderer, material_texture_path[i]);
	}

	auto texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 8, 8);
	if (texture == nullptr)
	{
		panic(SDL_GetError());
	}

	if (SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND) < 0)
	{
		panic(SDL_GetError());
	}

	SDL_SetRenderTarget(renderer, texture);

	SDL_RenderCopy(renderer, materials[M_AIR], NULL, NULL);
	SDL_RenderCopy(renderer, materials[M_FLOWER_BLANK], NULL, NULL);

	SDL_SetRenderTarget(renderer, NULL);

	materials[M_FLOWER] = texture;
}

void Map::load_scheme(const MapScheme &scheme)
{
	map_scheme = scheme;
}

void Map::render(SDL_Renderer *renderer)
{
	for (int row = 0; row < MAP_WIDTH; row++)
	{
		for (int column = 0; column < MAP_HEIGHT; column++)
		{
			int scale = 48;
			SDL_Rect dst = {
				.x = column * scale,
				.y = row * scale,
				.w = scale,
				.h = scale,
			};

			Game::render_texture(renderer, materials[map_scheme[row][column]], NULL, &dst);
		}
	}
}

const MapScheme scheme_1 = {{
	{M_AIR,     M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,     M_AIR,    M_AIR,    M_AIR,     M_AIR,    M_AIR,    M_AIR,     M_AIR,     M_AIR,    M_AIR,    M_AIR,    M_AIR,     M_AIR},
	{M_AIR,     M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,     M_AIR,    M_AIR,    M_AIR,     M_AIR,    M_AIR,    M_AIR,     M_AIR,     M_AIR,    M_AIR,    M_AIR,    M_AIR,     M_AIR},
	{M_AIR,     M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,     M_AIR,    M_AIR,    M_AIR,     M_AIR,    M_AIR,    M_AIR,     M_AIR,     M_AIR,    M_AIR,    M_AIR,    M_AIR,     M_AIR},
	{M_AIR,     M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,     M_AIR,    M_AIR,    M_AIR,     M_AIR,    M_AIR,    M_AIR,     M_AIR,     M_AIR,    M_AIR,    M_AIR,    M_AIR,     M_AIR},
	{M_AIR,     M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,     M_AIR,    M_AIR,    M_AIR,     M_AIR,    M_AIR,    M_AIR,     M_AIR,     M_AIR,    M_AIR,    M_AIR,    M_AIR,     M_AIR},
	{M_AIR,     M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,     M_AIR,    M_AIR,    M_AIR,     M_AIR,    M_AIR,    M_AIR,     M_AIR,     M_AIR,    M_AIR,    M_AIR,    M_AIR,     M_AIR},
	{M_AIR,     M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,     M_AIR,    M_AIR,    M_AIR,     M_AIR,    M_AIR,    M_AIR,     M_AIR,     M_AIR,    M_AIR,    M_AIR,    M_AIR,     M_AIR},
	{M_AIR,     M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,     M_AIR,    M_AIR,    M_AIR,     M_AIR,    M_AIR,    M_AIR,     M_AIR,     M_AIR,    M_AIR,    M_AIR,    M_AIR,     M_AIR},
	{M_AIR,     M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,     M_AIR,    M_AIR,    M_AIR,     M_AIR,    M_AIR,    M_AIR,     M_AIR,     M_AIR,    M_AIR,    M_AIR,    M_AIR,     M_AIR},
	{M_AIR,     M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,     M_AIR,    M_AIR,    M_AIR,     M_AIR,    M_AIR,    M_FLOWER,  M_FLOWER,  M_AIR,    M_AIR,    M_AIR,    M_AIR,     M_AIR},
	{M_AIR,     M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,     M_AIR,    M_AIR,    M_FLOWER,  M_AIR,    M_GRASS,  M_GRASS,   M_GRASS,   M_AIR,    M_AIR,    M_AIR,    M_AIR,     M_AIR},
	{M_AIR,     M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_FLOWER,  M_GRASS,  M_GRASS,  M_GRASS,   M_GRASS,  M_DIRT,   M_DIRT,    M_GRASS,   M_GRASS,  M_GRASS,  M_AIR,    M_FLOWER,  M_AIR},
	{M_FLOWER,  M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_AIR,    M_GRASS,   M_GRASS,  M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,    M_DIRT,   M_DIRT,   M_GRASS,  M_GRASS,   M_AIR},
	{M_GRASS,   M_WATER,  M_WATER,  M_WATER,  M_WATER,  M_WATER,  M_GRASS,  M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_GRASS,   M_GRASS},
	{M_DIRT,    M_WATER,  M_WATER,  M_WATER,  M_WATER,  M_WATER,  M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT},
	{M_DIRT,    M_DIRT,   M_WATER,  M_WATER,  M_WATER,  M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT},
	{M_DIRT,    M_DIRT,   M_WATER,  M_WATER,  M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT},
	{M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT,    M_DIRT,   M_DIRT,   M_DIRT,   M_DIRT,    M_DIRT},
}};
