#include "hero.h"


const string Hero::HERO_ANIM_IDLE_UP = "idleUp";
const string Hero::HERO_ANIM_IDLE_RIGHT = "idleRight";
const string Hero::HERO_ANIM_IDLE_DOWN = "idleDown";
const string Hero::HERO_ANIM_IDLE_LEFT = "idleLeft";

const string Hero::HERO_ANIM_MOVE_UP = "up";
const string Hero::HERO_ANIM_MOVE_RIGHT = "right";
const string Hero::HERO_ANIM_MOVE_DOWN = "down";
const string Hero::HERO_ANIM_MOVE_LEFT = "left";

const string Hero::HERO_ANIM_SLASH_UP = "slashUp";
const string Hero::HERO_ANIM_SLASH_RIGHT = "slashRight";
const string Hero::HERO_ANIM_SLASH_DOWN = "slashDown";
const string Hero::HERO_ANIM_SLASH_LEFT = "slashLeft";

const string Hero::HERO_ANIM_DASH_UP = "dashUp";
const string Hero::HERO_ANIM_DASH_RIGHT = "dashRight";
const string Hero::HERO_ANIM_DASH_DOWN = "dashDown";
const string Hero::HERO_ANIM_DASH_LEFT = "dashLeft";

const string Hero::HERO_ANIM_DIE = "die";


Hero::Hero(AnimationSet *animationSet)
{
	this->animationSet = animationSet;
	type = "hero";

	x = Globals::ScreenWidth / 2;
	y = Globals::ScreenHeight / 2;
	invincibleTimer = 0;
	moveSpeed = 0;
	moveSpeedMax = 80;
	hp = hpMax = 20;
	damage = 0;
	collisionBoxWidth = 20;
	collisionBoxHeight = 24;
	collisionBoxYOffset = -20;
	direction = DIR_DOWN;

	changeAnimation(HERO_STATE_IDLE, true);
	updateCollisionBox();
}


void Hero::update()
{
	if (state != HERO_STATE_DEAD && hp < 1)
	{
		changeAnimation(HERO_STATE_DEAD, true);
		moving = false;
		die();
	}

	updateCollisionBox();
	updateMovement();
	updateCollisions();

	updateHitBox();
	updateDamages();

	updateAnimation();
	updateInvincibleTimer();
}


void Hero::slash()
{
	if (hp > 0 && (state == HERO_STATE_MOVE || state == HERO_STATE_IDLE))
	{
		moving = false;
		frameTimer = 0;
		changeAnimation(HERO_STATE_SLASH, true);
	}
}


void Hero::dash()
{
	if (hp > 0 && (state == HERO_STATE_MOVE || state == HERO_STATE_IDLE))
	{
		moving = false;
		frameTimer = 0;

		// Push the hero in the direction are are travelling
		slideAngle = angle;
		slideAmount = 300;
		invincibleTimer = 0.1;

		changeAnimation(HERO_STATE_DASH, true);
	}
}


void Hero::die()
{
	moving = false;
	changeAnimation(HERO_STATE_DEAD, true);
}


void Hero::revive()
{
	if (hp > 0)
	{
		return;
	}

	hp = hpMax;
	moving = false;
	changeAnimation(HERO_STATE_IDLE, true);
	x = Globals::ScreenWidth / 2;
	y = Globals::ScreenHeight / 2;
	slideAmount = 0;
}


void Hero::changeAnimation(int newState, bool resetFrameToBeginning)
{
	state = newState;

	switch (state)
	{
	case HERO_STATE_IDLE:
		switch (direction)
		{
		case DIR_UP:
			currentAnimation = animationSet->getAnimation(HERO_ANIM_IDLE_UP);
			break;

		case DIR_RIGHT:
			currentAnimation = animationSet->getAnimation(HERO_ANIM_IDLE_RIGHT);
			break;

		case DIR_DOWN:
			currentAnimation = animationSet->getAnimation(HERO_ANIM_IDLE_DOWN);
			break;

		case DIR_LEFT:
			currentAnimation = animationSet->getAnimation(HERO_ANIM_IDLE_LEFT);
			break;
		}
		break;

	case HERO_STATE_MOVE:
		switch (direction)
		{
		case DIR_UP:
			currentAnimation = animationSet->getAnimation(HERO_ANIM_MOVE_UP);
			break;

		case DIR_RIGHT:
			currentAnimation = animationSet->getAnimation(HERO_ANIM_MOVE_RIGHT);
			break;

		case DIR_DOWN:
			currentAnimation = animationSet->getAnimation(HERO_ANIM_MOVE_DOWN);
			break;

		case DIR_LEFT:
			currentAnimation = animationSet->getAnimation(HERO_ANIM_MOVE_LEFT);
			break;
		}
		break;

	case HERO_STATE_SLASH:
		switch (direction)
		{
		case DIR_UP:
			currentAnimation = animationSet->getAnimation(HERO_ANIM_SLASH_UP);
			break;

		case DIR_RIGHT:
			currentAnimation = animationSet->getAnimation(HERO_ANIM_SLASH_RIGHT);
			break;

		case DIR_DOWN:
			currentAnimation = animationSet->getAnimation(HERO_ANIM_SLASH_DOWN);
			break;

		case DIR_LEFT:
			currentAnimation = animationSet->getAnimation(HERO_ANIM_SLASH_LEFT);
			break;
		}
		break;

	case HERO_STATE_DASH:
		switch (direction)
		{
		case DIR_UP:
			currentAnimation = animationSet->getAnimation(HERO_ANIM_DASH_UP);
			break;

		case DIR_RIGHT:
			currentAnimation = animationSet->getAnimation(HERO_ANIM_DASH_RIGHT);
			break;

		case DIR_DOWN:
			currentAnimation = animationSet->getAnimation(HERO_ANIM_DASH_DOWN);
			break;

		case DIR_LEFT:
			currentAnimation = animationSet->getAnimation(HERO_ANIM_DASH_LEFT);
			break;
		}
		break;

	case HERO_STATE_DEAD:
		currentAnimation = animationSet->getAnimation(HERO_ANIM_DIE);
		break;
	}

	if (resetFrameToBeginning)
	{
		currentFrame = currentAnimation->getFrame(0);
	}
	else
	{
		currentFrame = currentAnimation->getFrame(currentFrame->frameNumber);
	}
}


void Hero::updateAnimation()
{
	if (currentFrame == nullptr || currentAnimation == nullptr)
	{
		return;
	}

	if (state == HERO_STATE_MOVE && ! moving)
	{
		changeAnimation(HERO_STATE_IDLE, true);
	}

	if (state != HERO_STATE_MOVE && moving)
	{
		changeAnimation(HERO_STATE_MOVE, true);
	}

	frameTimer += TimeController::timeController.dT;
	if (frameTimer >= currentFrame->duration)
	{
		if (currentFrame->frameNumber == currentAnimation->getEndFrameNumber())
		{
			if (state == HERO_STATE_SLASH || state == HERO_STATE_DASH)
			{
				changeAnimation(HERO_STATE_MOVE, true);
			}
			else if (state == HERO_STATE_DEAD && hp > 0)
			{
				changeAnimation(HERO_STATE_MOVE, true);
			}
			else
			{
				currentFrame = currentAnimation->getFrame(0);
			}
		}
		else
		{
			currentFrame = currentAnimation->getNextFrame(currentFrame);
		}

		frameTimer = 0;
	}
}


void Hero::updateDamages()
{
	if (active && hp > 0 && invincibleTimer <= 0)
	{
		Entity* entity = nullptr;
		for (auto it = entities.begin(); it != entities.end(); it++)
		{
			entity = *it;
			if (entity->active && entity->type == "enemy")
			{
				LivingEntity* enemy = (LivingEntity*) entity;
				if (enemy->damage > 0 && Entity::checkCollision(collisionBox, enemy->hitBox))
				{
					hp -= enemy->damage;
					if (hp > 0)
					{
						invincibleTimer = 0.3;
					}

					slideAngle = Entity::angleBetweenTwoEntities(entity, this);
					slideAmount = 200;
				}
			}
		}
	}
}


