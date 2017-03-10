#pragma once

#include <string>
#include <iostream>
#include <SDL.h>
#include "randomNumber.h"

using namespace std;


class Globals
{
public:
	
	static const float PI;

	static bool debugging;

	static int ScreenWidth, ScreenHeight, ScreenScale;
	static SDL_Renderer* renderer;

	static string clipOffDataHeader(string data);
};

