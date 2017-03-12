#pragma once

#include "globals.h"
#include "hero.h"
#include "wall.h"
#include "glob.h"
#include "keyboardInput.h"
#include "drawing_functions.h"

using namespace std;


class Game
{
public:
	AnimationSet* heroAnimationSet;
	AnimationSet* globAnimationSet;
	AnimationSet* wallAnimationSet;

	SDL_Texture* backgroundImage;

	Hero* hero;
	KeyboardInput heroInput;

	list<Entity*> enemies;
	list<Entity*> walls;

	Game();
	~Game();

	void update();
	void draw();
};

