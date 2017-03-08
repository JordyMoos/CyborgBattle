#include "globals.h"

const float Globals::PI = 3.14159;

bool Globals::debugging = true;

int Globals::ScreenWidth = 640;
int Globals::ScreenHeight = 352;
int Globals::ScreenScale = 1;

static SDL_Renderer* renderer = nullptr;
