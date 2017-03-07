#include "cleanup.h"
#include "res_path.h"
#include "drawing_functions.h"
#include "SDL_mixer.h"

int main(int argc, char **argv)
{
	// Setup SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "error initialising SDL" << endl;
		return 1;
	}

	// Setup window
	SDL_Window *window = SDL_CreateWindow(
		"Cyborg Battle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		640, 352, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		SDL_Quit();
		cout << "window error" << endl;
		return 2;
	}

	// Setup renderer
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr)
	{
		cleanup(window);
		SDL_Quit();
		cout << "renderer error" << endl;
		return 3;
	}

	SDL_RenderSetLogicalSize(renderer, 640, 352);

	// Initialise sdl image
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		cleanup(renderer);
		cleanup(window);
		SDL_Quit();
		cout << "sdl image did not initialise" << endl;
		return 4;
	}

	// Initialise text to font
	if (TTF_Init() != 0)
	{
		cleanup(renderer);
		cleanup(window);
		SDL_Quit();
		cout << "sdl_ttf did not initialise" << endl;
		return 5;
	}

	// Initialise sdl mixer
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		cleanup(renderer);
		cleanup(window);
		SDL_Quit();
		cout << "sdl_mixer did not initialise" << endl;
		return 6;
	}

	// Load up a texture to draw
	string resPath = getResourcePath();
	SDL_Texture *texture = loadTexture(resPath + "map.png", renderer);

	// Run game
	while (SDL_GetTicks() < 5000)
	{
		SDL_RenderClear(renderer);
		renderTexture(texture, renderer, 0, 0);
		SDL_RenderPresent(renderer);
	}

	cleanup(texture);
	cleanup(renderer);
	cleanup(window);
	SDL_Quit();

	return 0;
}
