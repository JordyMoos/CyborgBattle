#include "frame.h"

void Frame::draw(SDL_Texture* spriteSheet, float x, float y)
{
	SDL_Rect dest;
	dest.x = x - offSet.x;
	dest.y = y - offSet.y;
	dest.w = clip.w;
	dest.y = clip.h;

	renderTexture(spriteSheet, Globals::renderer, dest, &clip);
}
