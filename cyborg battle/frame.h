#pragma once

#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include "SDL.h"
#include "drawing_functions.h"
#include "globals.h"
#include "groupBuilder.h"

using namespace std;


class Frame
{
	int frameNumber;
	SDL_Rect clip;
	float duration;
	SDL_Point offSet; // Pivot point to help align frames in an animation

	list<Group*> frameData;

	void draw(SDL_Texture* spriteSheet, float x, float y);


};

