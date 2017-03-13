#pragma once

#include "globals.h"
#include "hero.h"
#include "wall.h"
#include "glob.h"
#include "keyboardInput.h"
#include "drawing_functions.h"
#include "soundManager.h"

using namespace std;


class Game
{
public:
	Mix_Music* song = nullptr;

	AnimationSet* heroAnimationSet;
	AnimationSet* globAnimationSet;
	AnimationSet* wallAnimationSet;

	SDL_Texture* backgroundImage;
	SDL_Texture* splashImage;
	SDL_Texture* overlayImage;

	SDL_Texture* scoreTexture = nullptr;

	Hero* hero;
	KeyboardInput heroInput;

	list<Entity*> enemies;
	list<Entity*> walls;

	bool splashShowing;
	float overlayTimer;

	Game();
	~Game();

	void update();
	void draw();
};

