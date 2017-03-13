#include "soundManager.h"


SoundManager SoundManager::soundmanager;


SoundManager::SoundManager()
{

}


SoundManager::~SoundManager()
{
	for (auto it = sounds.begin(); it != sounds.end(); it++)
	{
		Mix_FreeChunk(it->sound);
	}
}


void SoundManager::loadSound(string name, string file)
{
	soundListing listing;
	listing.name = name;
	listing.sound = Mix_LoadWAV(file.c_str());

	sounds.push_back(listing);
}


void SoundManager::playSound(string name)
{
	for (auto it = sounds.begin(); it != sounds.end(); it++)
	{
		if (it->name == name)
		{
			Mix_PlayChannel(-1, it->sound, 0);
			break;
		}
	}
}

