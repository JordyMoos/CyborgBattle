#pragma once

#include <string>
#include "globals.h"
#include "livingEntity.h"

using namespace std;


class Hero : public LivingEntity
{
public:

	// Reference variables
	static const string HERO_ANIM_IDLE_UP;
	static const string HERO_ANIM_IDLE_RIGHT;
	static const string HERO_ANIM_IDLE_DOWN;
	static const string HERO_ANIM_IDLE_LEFT;

	static const string HERO_ANIM_MOVE_UP;
	static const string HERO_ANIM_MOVE_RIGHT;
	static const string HERO_ANIM_MOVE_DOWN;
	static const string HERO_ANIM_MOVE_LEFT;

	static const string HERO_ANIM_SLASH_UP;
	static const string HERO_ANIM_SLASH_RIGHT;
	static const string HERO_ANIM_SLASH_DOWN;
	static const string HERO_ANIM_SLASH_LEFT;

	static const string HERO_ANIM_DASH_UP;
	static const string HERO_ANIM_DASH_RIGHT;
	static const string HERO_ANIM_DASH_DOWN;
	static const string HERO_ANIM_DASH_LEFT;

	static const string HERO_ANIM_DIE;

	static const int HERO_STATE_IDLE = 0;
	static const int HERO_STATE_MOVE = 1;
	static const int HERO_STATE_SLASH = 2;
	static const int HERO_STATE_DASH = 3;
	static const int HERO_STATE_DEAD = 4;


	Hero(AnimationSet *animSet);
	void update();
	void slash();
	void dash();
	void die();
	void revive();
	void changeAnimation(int newState, bool resetFrameToBeginning);
	void updateAnimation();
	void updateDamages();
};
