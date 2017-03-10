#pragma once

#include "animation.h"

using namespace std;


class AnimationSet
{
public:

	string imageName;
	SDL_Texture* spriteSheet;
	SDL_Texture* whiteSpriteSheet = nullptr;
	list<Animation> animations;

	~AnimationSet();

	Animation* getAnimation(string name);

	void loadAnimationSet(string fileName, list<DataGroupType> &groupTypes, bool setColorKey = false,
		int transparentPixleIndex = 0, bool createWhiteTexture = false);
};
