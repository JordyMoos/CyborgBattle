#pragma once

#include "SDL_mixer.h"
#include <string>
#include <list>

using namespace std;


struct soundListing
{
	Mix_Chunk* sound;
	string name;
};


class SoundManager
{
public:

	list<soundListing> sounds;

	static SoundManager soundmanager;

	SoundManager();
	~SoundManager();
	void loadSound(string name, string file);
	void playSound(string name);
};
