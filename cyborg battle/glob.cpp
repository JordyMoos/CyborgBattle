#include "glob.h"


const string Glob::GLOB_ANIM_IDLE_UP = "idleUp";
const string Glob::GLOB_ANIM_IDLE_RIGHT = "idleRight";
const string Glob::GLOB_ANIM_IDLE_DOWN = "idleDown";
const string Glob::GLOB_ANIM_IDLE_LEFT = "idleLeft";

const string Glob::GLOB_ANIM_MOVE_UP = "up";
const string Glob::GLOB_ANIM_MOVE_RIGHT = "right";
const string Glob::GLOB_ANIM_MOVE_DOWN = "down";
const string Glob::GLOB_ANIM_MOVE_LEFT = "left";

const string Glob::GLOB_ANIM_ATTACK_UP = "attackUp";
const string Glob::GLOB_ANIM_ATTACK_RIGHT = "attackRight";
const string Glob::GLOB_ANIM_ATTACK_DOWN = "attackDown";
const string Glob::GLOB_ANIM_ATTACK_LEFT = "attackLeft";

const string Glob::GLOB_ANIM_TELEGRAPH_UP = "telegraphUp";
const string Glob::GLOB_ANIM_TELEGRAPH_RIGHT = "telegraphRight";
const string Glob::GLOB_ANIM_TELEGRAPH_DOWN = "telegraphDown";
const string Glob::GLOB_ANIM_TELEGRAPH_LEFT = "telegraphLeft";

const string Glob::GLOB_ANIM_DIE = "die";

const int Glob::GLOB_STATE_IDLE = 0;
const int Glob::GLOB_STATE_MOVE = 1;
const int Glob::GLOB_STATE_ATTACK = 2;
const int Glob::GLOB_STATE_TELEGRAPH = 3;
const int Glob::GLOB_STATE_DEAD = 4;

const int Glob::GLOB_AI_NORMAL = 0;
const int Glob::GLOB_AI_CHASE = 1;

int Glob::globsKilled = 0;


Glob::Glob(AnimationSet* animationSet)
{
	this->animationSet = animationSet;
	type = "enemy";

	x = Globals::ScreenWidth / 2;
	y = Globals::ScreenHeight / 2;
	moveSpeed = 0;
	moveSpeedMax = 20;
	hp = hpMax = 10 + (rand() % 20);
	damage = 0;
	collisionBoxWidth = 18;
	collisionBoxHeight = 20;
	collisionBox.w = collisionBoxWidth;
	collisionBox.h = collisionBoxHeight;
	collisionBoxYOffset = -14;
	direction = DIR_DOWN;

	changeAnimation(GLOB_STATE_IDLE, true);
	updateCollisionBox();
}


void Glob::update()
{
	if (hp < 1 && state != GLOB_STATE_DEAD)
	{
		changeAnimation(GLOB_STATE_DEAD, true);
		moving = false;
		die();
	}

	think();

	updateCollisionBox();
	updateMovement();
	updateCollisions();
	updateHitBox();
	updateDamages();

	updateAnimation();
	updateInvincibleTimer();
}


void Glob::think()
{
	if (state == GLOB_STATE_IDLE || state == GLOB_STATE_MOVE)
	{
		thinkTimer -= TimeController::timeController.dT;
		if (thinkTimer <= 0)
		{
			thinkTimer = rand() % 5; // 0-4 seconds

			int action = rand() % 10;
			if (action < 3)
			{
				moving = false;
				aiState = GLOB_AI_NORMAL;
				changeAnimation(GLOB_STATE_IDLE, true);
			}
			else
			{
				findNearestTarget();
				if (target != nullptr && target->hp > 0)
				{
					float distance = Entity::distanceBetweenTwoEntities(this, target);
					// Attack if in range
					if (distance < 100)
					{
						telegraph();
						aiState = GLOB_AI_NORMAL;
					}

					// Move to the target
					else
					{
						aiState = GLOB_AI_CHASE;
						moving = true;
						changeAnimation(GLOB_STATE_MOVE, state != GLOB_STATE_MOVE);
					}
				}

				// Dont have a target
				else
				{
					moving = false;
					aiState = GLOB_AI_NORMAL;
					changeAnimation(GLOB_STATE_IDLE, true);
				}
			}
		}
	}

	if (aiState == GLOB_AI_CHASE && hp > 0 && active)
	{
		angle = Entity::angleBetweenTwoEntities(this, target);
		move(angle);
	}
}


void Glob::telegraph()
{
	moving = false;
	frameTimer = 0;
	changeAnimation(GLOB_STATE_TELEGRAPH, true);
}


void Glob::attack()
{
	moving = false;
	frameTimer = 0;
	slideAmount = 100;
	slideAngle = angle;
	changeAnimation(GLOB_STATE_ATTACK, true);
}


void Glob::die()
{
	moving = false;
	changeAnimation(GLOB_STATE_DEAD, true);

	Glob::globsKilled++;
}


void Glob::findNearestTarget()
{
	target = nullptr;
	Entity* entity;
	for (auto it = Entity::entities.begin(); it != Entity::entities.end(); it++)
	{
		entity = *it;
		if (entity->type == "hero" && entity->active)
		{
			if (target == nullptr)
			{
				target = (LivingEntity*) entity;
			}
			else
			{
				if (Entity::distanceBetweenTwoEntities(this, entity) < Entity::distanceBetweenTwoEntities(this, target))
				{
					target = (LivingEntity*) entity;
				}
			}
		}
	}
}


void Glob::changeAnimation(int newState, bool resetFrameToBeginning)
{
	state = newState;

	switch (state)
	{
	case GLOB_STATE_IDLE:
		switch (direction)
		{
		case DIR_UP: currentAnimation = animationSet->getAnimation(GLOB_ANIM_IDLE_UP); break;
		case DIR_RIGHT: currentAnimation = animationSet->getAnimation(GLOB_ANIM_IDLE_RIGHT); break;
		case DIR_DOWN: currentAnimation = animationSet->getAnimation(GLOB_ANIM_IDLE_DOWN); break;
		case DIR_LEFT: currentAnimation = animationSet->getAnimation(GLOB_ANIM_IDLE_LEFT); break;
		}
		break;

	case GLOB_STATE_MOVE:
		switch (direction)
		{
		case DIR_UP: currentAnimation = animationSet->getAnimation(GLOB_ANIM_MOVE_UP); break;
		case DIR_RIGHT: currentAnimation = animationSet->getAnimation(GLOB_ANIM_MOVE_RIGHT); break;
		case DIR_DOWN: currentAnimation = animationSet->getAnimation(GLOB_ANIM_MOVE_DOWN); break;
		case DIR_LEFT: currentAnimation = animationSet->getAnimation(GLOB_ANIM_MOVE_LEFT); break;
		}
		break;

	case GLOB_STATE_ATTACK:
		switch (direction)
		{
		case DIR_UP: currentAnimation = animationSet->getAnimation(GLOB_ANIM_ATTACK_UP); break;
		case DIR_RIGHT: currentAnimation = animationSet->getAnimation(GLOB_ANIM_ATTACK_RIGHT); break;
		case DIR_DOWN: currentAnimation = animationSet->getAnimation(GLOB_ANIM_ATTACK_DOWN); break;
		case DIR_LEFT: currentAnimation = animationSet->getAnimation(GLOB_ANIM_ATTACK_LEFT); break;
		}
		break;

	case GLOB_STATE_TELEGRAPH:
		switch (direction)
		{
		case DIR_UP: currentAnimation = animationSet->getAnimation(GLOB_ANIM_TELEGRAPH_UP); break;
		case DIR_RIGHT: currentAnimation = animationSet->getAnimation(GLOB_ANIM_TELEGRAPH_RIGHT); break;
		case DIR_DOWN: currentAnimation = animationSet->getAnimation(GLOB_ANIM_TELEGRAPH_DOWN); break;
		case DIR_LEFT: currentAnimation = animationSet->getAnimation(GLOB_ANIM_TELEGRAPH_LEFT); break;
		}
		break;

	case GLOB_STATE_DEAD:
		currentAnimation = animationSet->getAnimation(GLOB_ANIM_DIE);
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


void Glob::updateAnimation()
{
	if (currentFrame == nullptr || currentAnimation == nullptr)
	{
		return;
	}

	if (state == GLOB_STATE_MOVE && !moving)
	{
		changeAnimation(GLOB_STATE_IDLE, true);
	}

	if (state != GLOB_STATE_MOVE && moving)
	{
		changeAnimation(GLOB_STATE_MOVE, true);
	}

	frameTimer += TimeController::timeController.dT;
	if (frameTimer >= currentFrame->duration)
	{
		if (currentFrame->frameNumber == currentAnimation->getEndFrameNumber())
		{
			if (state == GLOB_STATE_TELEGRAPH)
			{
				attack();
			}
			else if (state == GLOB_STATE_ATTACK)
			{
				changeAnimation(GLOB_STATE_MOVE, true);
			}
			else if (state == GLOB_STATE_DEAD)
			{
				if (hp > 0)
				{
					moving = true;
					changeAnimation(GLOB_STATE_MOVE, true);
				}
				else
				{
					active = false;
				}
			}
			else
			{
				currentFrame = currentAnimation->getFrame(0);
			}
		}

		// Not at the end of an animation
		else
		{
			currentFrame = currentAnimation->getNextFrame(currentFrame);
		}

		frameTimer = 0;
	}
}


void Glob::updateDamages()
{
	if (active && hp > 0 && invincibleTimer <= 0)
	{
		Entity* entity;
		LivingEntity* enemy;
		for (auto it = Entity::entities.begin(); it != Entity::entities.end(); it++)
		{
			entity = *it;
			if (entity->active && entity->type == "hero")
			{
				enemy = (LivingEntity*) entity;
				if (enemy->damage > 0 && Entity::checkCollision(collisionBox, enemy->hitBox))
				{
					hp -= enemy->damage;
					if (hp > 0)
					{
						invincibleTimer = 0.1;
					}

					slideAngle = Entity::angleBetweenTwoEntities(entity, this);
					slideAmount = 300;
				}
			}
		}
	}
}

