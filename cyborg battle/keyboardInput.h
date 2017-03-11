#pragma once

#include "hero.h"


class KeyboardInput
{
public:

	Hero* hero;

	static const SDL_Scancode UP = SDL_SCANCODE_UP;
	static const SDL_Scancode RIGHT = SDL_SCANCODE_RIGHT;
	static const SDL_Scancode DOWN = SDL_SCANCODE_DOWN;
	static const SDL_Scancode LEFT = SDL_SCANCODE_LEFT;
	static const SDL_Scancode SLASH = SDL_SCANCODE_Z;
	static const SDL_Scancode DASH = SDL_SCANCODE_X;

	KeyboardInput();
	void update(SDL_Event* e);
};

