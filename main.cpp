#include <SDL2/SDL_image.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <ostream>

#include "game.hpp"
#include "panic.hpp"

int main()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0)
	{
		std::cout << "Unable to initialize SDL2: " << SDL_GetError() << std::endl;
		panic();
	}

	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		std::cout << "Unable to initialize SDL2_image: " << IMG_GetError() << std::endl;
		panic();
	}

	Game game("test", 1600, 900);

	const float time_freq = SDL_GetPerformanceFrequency();
	auto time_last = SDL_GetPerformanceCounter();

	const float frame_delta = 1000.0f / 60.0f;
	auto frame_time = time_last;
	auto frame_count = 0;

	while (game.running())
	{
		auto time_now = SDL_GetPerformanceCounter();
		float time_delta = (time_now - time_last) / time_freq * 1000.0f;

		game.events();

		if (time_now > frame_time + time_freq)
		{
			// Display FPS (1s passed)
			frame_count = 0;
			frame_time = time_now;
		}

		if (time_delta > frame_delta)
		{
			game.update(time_delta);
			game.render();

			time_last = time_now;
			++frame_count;
		}
	}
}
