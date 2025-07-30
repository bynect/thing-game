#include <fstream>
#include <regex>

#include "map.hpp"
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

const int MAP_WIDTH = 48;
const int MAP_HEIGHT = 24;

template<typename T, int X, int Y>
using FixMatrix = std::array<std::array<T, X>, Y>;

using MapScheme = FixMatrix<Material, MAP_WIDTH, MAP_HEIGHT>;

extern const MapScheme scheme_1;

bool Map::load_file(std::string path)
{
    std::ifstream infile(path);
    std::string line;
    std::regex pattern("map (\\d+) (\\d+) spawn (\\d+) (\\d+)");

    if (!std::getline(infile, line))
        return false;

    std::smatch match;
    if (std::regex_match(line, match, pattern)) {
        size_t width = std::stoul(match[1].str());
        size_t height = std::stoul(match[2].str());
        size_t spawnx = std::stoul(match[3].str());
        size_t spawny = std::stoul(match[4].str());

        std::cout << "Parsed map:" << std::endl;
        std::cout << "\tWidth: " << width << std::endl;
        std::cout << "\tHeight " << height << std::endl;
        std::cout << "\tSpawn X: " << spawnx << std::endl;
        std::cout << "\tSpawn Y: " << spawny << std::endl;

        tiles.resize(height, width);

        for (size_t row = 0; row < height; row++)
        {
            bool end = false;
            for (size_t column = 0; column < width; column++)
            {
                Material material = M_VOID;
                if (!end) {
                    char c = infile.peek();
                    infile.get();
                    switch (c) {
                        case ' ':
                            break;

                        case 'D':
                            material = M_DIRT;
                            break;

                        case 'G':
                            material = M_GRASS;
                            break;

                        case 'C':
                            material = M_COAL;
                            break;

                        case 'L':
                            material = M_LAPIS;
                            break;

                        case 'W':
                            material = M_WATER;
                            break;

                        case 'F':
                            material = M_FLOWER;
                            break;

                        case '\n':
                            end = true;
                            break;

                        default:
                            std::cout << "Invalid material " << int(c) << std::endl;
                            break;
                    }
                }

                tiles[row][column].material = material;
                tiles[row][column].collider.active = material != M_VOID && material != M_FLOWER;
                tiles[row][column].collider.rect = {
                    .x = float(column * tile_size),
                    .y = float(row * tile_size),
                    .w = float(tile_size),
                    .h = float(tile_size),
                };
            }

            if (!end) infile.get();
        }

        if (spawnx > width || spawny > height) {
            std::cout << "Invalid spawn point" << std::endl;
            return false;
        }

        spawn_pos = {
            float(spawnx * tile_size),
            float(spawny * tile_size),
        };
    } else {
        std::cout << "Invalid map header" << std::endl;
        return false;
    }

    this->path = std::move(path);
    return true;
}

void Map::render(SDL_Renderer *renderer, const SDL_FRect &camera)
{
    SDL_SetRenderDrawColor(renderer, 212, 241, 249, 255);
    SDL_RenderClear(renderer);

//  std::cout << "map(from: " << from << ", to: " << to << ")" << std::endl;
//  std::cout << "map_scheme.size() = " << map_scheme.size() << " map_scheme[0].size() = " << map_scheme[0].size() << std::endl;

    int start_row = std::max(0, int(camera.y / tile_size));
    int end_row   = std::min(int(tiles.rows), int(camera.y + camera.h) / tile_size + 1);

    int start_col = std::max(0, int(camera.x / tile_size));
    int end_col   = std::min(int(tiles.columns), int(camera.x + camera.w) / tile_size + 1);

    for (auto row = start_row; row < end_row; row++) {
        for (auto column = start_col; column < end_col; column++) {
            auto &tile = tiles[row][column];
            if (tile.material == M_VOID) continue;

            SDL_FRect dst = {
                .x = float(column * tile_size - camera.x),
                .y = float(row * tile_size - camera.y),
                .w = float(tile_size),
                .h = float(tile_size),
            };
            SDL_RenderCopyF(renderer, materials[tile.material], nullptr, &dst);
        }
    }
}

Slice<Tile*> Map::colliding(const Collider &other, Tile *(&scratch)[8])
{
    int approx_row = other.rect.y / tile_size;
    int min_row = std::max(0, approx_row - 1);
    int max_row = std::min(int(tiles.rows) - 1, approx_row + 1);

    int approx_column = other.rect.x / tile_size;
    int min_column = std::max(0, approx_column - 1);
    int max_column = std::min(int(tiles.columns) - 1, approx_column + 1);

    size_t idx = 0;
    for (auto row = min_row; row <= max_row; row++) {
        for (auto column = min_column; column <= max_column; column++) {
            auto &tile = tiles[row][column];
            if (tile.collider.colliding(other))
                scratch[idx++] = &tile;
        }
    }

    return Slice(scratch, idx);
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
