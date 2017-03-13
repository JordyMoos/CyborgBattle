#pragma once

#include <cstdlib>
#include "livingEntity.h"
#include "soundManager.h"


class Grob : public LivingEntity
{
public:

	static const string GROB_ANIM_IDLE_UP;
	static const string GROB_ANIM_IDLE_RIGHT;
	static const string GROB_ANIM_IDLE_DOWN;
	static const string GROB_ANIM_IDLE_LEFT;

	static const string GROB_ANIM_MOVE_UP;
	static const string GROB_ANIM_MOVE_RIGHT;
	static const string GROB_ANIM_MOVE_DOWN;
	static const string GROB_ANIM_MOVE_LEFT;

	static const string GROB_ANIM_ATTACK_UP;
	static const string GROB_ANIM_ATTACK_RIGHT;
	static const string GROB_ANIM_ATTACK_DOWN;
	static const string GROB_ANIM_ATTACK_LEFT;

	static const string GROB_ANIM_DIE;

	static const int GROB_STATE_IDLE;
	static const int GROB_STATE_MOVE;
	static const int GROB_STATE_DEAD;

	static int grobsKilled;

	float thinkTimer = 0;

	LivingEntity* target = nullptr;

	Grob(AnimationSet* animationSet);
	void update();
	void think();
	void telegraph();
	void attack();
	void die();
	void findNearestTarget();
	void changeAnimation(int newState, bool resetFrameToBeginning);
	void updateAnimation();
	void updateHitBox();
	void updateDamages();
};

