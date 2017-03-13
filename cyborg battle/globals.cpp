#include "globals.h"

const float Globals::PI = 3.14159;

bool Globals::debugging = false;

int Globals::ScreenWidth = 640;
int Globals::ScreenHeight = 352;
int Globals::ScreenScale = 2;

SDL_Renderer* Globals::renderer = nullptr;


string Globals::clipOffDataHeader(string data)
{
	int pos = data.find(":", 0);
	if (pos == -1)
	{
		return data;
	}
	
	return data.substr(pos + 1, data.length() - pos + 2);
}
