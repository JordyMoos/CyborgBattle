#include "keyboardInput.h"


KeyboardInput::KeyboardInput()
{

}


void KeyboardInput::update(SDL_Event* e)
{
	if (e->type == SDL_KEYDOWN)
	{
		switch (e->key.keysym.scancode)
		{
		case DASH:
			hero->dash();
			break;

		case SLASH:
			hero->slash();
			break;
		}
	}

	// Check if key is held down
	const Uint8* keyStates = SDL_GetKeyboardState(nullptr);

	// If hero is not able to move or no direction is held then stop moving
	if ((hero->state != Hero::HERO_STATE_MOVE && hero->state != Hero::HERO_STATE_IDLE)
		|| ( ! keyStates[UP] && ! keyStates[RIGHT] && ! keyStates[DOWN] && ! keyStates[LEFT]))
	{
		hero->moving = false;
	}
	else
	{
		if (keyStates[UP])
		{
			if (keyStates[RIGHT])
			{
				hero->move(270 + 45);
			}
			else if (keyStates[LEFT])
			{
				hero->move(270 - 45);
			}
			else
			{
				hero->move(270);
			}
		}
		
		else if (keyStates[DOWN])
		{
			if (keyStates[RIGHT])
			{
				hero->move(90 - 45);
			}
			else if (keyStates[LEFT])
			{
				hero->move(90 + 45);
			}
			else
			{
				hero->move(90);
			}
		}

		else if (keyStates[LEFT])
		{
			hero->move(180);
		}

		else if (keyStates[RIGHT])
		{
			hero->move(0);
		}
	}
}