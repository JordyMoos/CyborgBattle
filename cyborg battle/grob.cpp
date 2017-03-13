#include "grob.h"


const string Grob::GROB_ANIM_IDLE_UP = "idleUp";
const string Grob::GROB_ANIM_IDLE_RIGHT = "idleRight";
const string Grob::GROB_ANIM_IDLE_DOWN = "idleDown";
const string Grob::GROB_ANIM_IDLE_LEFT = "idleLeft";

const string Grob::GROB_ANIM_MOVE_UP = "up";
const string Grob::GROB_ANIM_MOVE_RIGHT = "right";
const string Grob::GROB_ANIM_MOVE_DOWN = "down";
const string Grob::GROB_ANIM_MOVE_LEFT = "left";

const string Grob::GROB_ANIM_ATTACK_UP = "attackUp";
const string Grob::GROB_ANIM_ATTACK_RIGHT = "attackRight";
const string Grob::GROB_ANIM_ATTACK_DOWN = "attackDown";
const string Grob::GROB_ANIM_ATTACK_LEFT = "attackLeft";

const string Grob::GROB_ANIM_DIE = "die";

const int Grob::GROB_STATE_IDLE = 0;
const int Grob::GROB_STATE_MOVE = 1;
const int Grob::GROB_STATE_DEAD = 2;

int Grob::grobsKilled = 0;


Grob::Grob(AnimationSet* animationSet)
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

	changeAnimation(GROB_STATE_IDLE, true);
	updateCollisionBox();
}


void Grob::update()
{
	if (hp < 1 && state != GROB_STATE_DEAD)
	{
		changeAnimation(GROB_STATE_DEAD, true);
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


void Grob::think()
{
	if (state == GROB_STATE_IDLE || state == GROB_STATE_MOVE)
	{
		thinkTimer -= TimeController::timeController.dT;
		if (thinkTimer <= 0)
		{
			thinkTimer = rand() % 5; // 0-4 seconds

			int action = rand() % 10;
			if (action < 3)
			{
				moving = false;
				changeAnimation(GROB_STATE_IDLE, true);
			}
			else
			{
				findNearestTarget();
				if (target != nullptr && target->hp > 0)
				{
					float distance = Entity::distanceBetweenTwoEntities(this, target);
					moving = true;
					changeAnimation(GROB_STATE_MOVE, state != GROB_STATE_MOVE);
				}

				// Dont have a target
				else
				{
					moving = false;
					changeAnimation(GROB_STATE_IDLE, true);
				}
			}
		}
	}

	if (state == GROB_STATE_MOVE && hp > 0 && active)
	{
		angle = Entity::angleBetweenTwoEntities(this, target);
		move(angle);
	}
}


void Grob::die()
{
	moving = false;
	changeAnimation(GROB_STATE_DEAD, true);

	Grob::grobsKilled++;

	SoundManager::soundmanager.playSound("enemyDie");
}


void Grob::findNearestTarget()
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
				target = (LivingEntity*)entity;
			}
			else
			{
				if (Entity::distanceBetweenTwoEntities(this, entity) < Entity::distanceBetweenTwoEntities(this, target))
				{
					target = (LivingEntity*)entity;
				}
			}
		}
	}
}


void Grob::changeAnimation(int newState, bool resetFrameToBeginning)
{
	state = newState;

	switch (state)
	{
	case GROB_STATE_IDLE:
		switch (direction)
		{
		case DIR_UP: currentAnimation = animationSet->getAnimation(GROB_ANIM_IDLE_UP); break;
		case DIR_RIGHT: currentAnimation = animationSet->getAnimation(GROB_ANIM_IDLE_RIGHT); break;
		case DIR_DOWN: currentAnimation = animationSet->getAnimation(GROB_ANIM_IDLE_DOWN); break;
		case DIR_LEFT: currentAnimation = animationSet->getAnimation(GROB_ANIM_IDLE_LEFT); break;
		}
		break;

	case GROB_STATE_MOVE:
		switch (direction)
		{
		case DIR_UP: currentAnimation = animationSet->getAnimation(GROB_ANIM_MOVE_UP); break;
		case DIR_RIGHT: currentAnimation = animationSet->getAnimation(GROB_ANIM_MOVE_RIGHT); break;
		case DIR_DOWN: currentAnimation = animationSet->getAnimation(GROB_ANIM_MOVE_DOWN); break;
		case DIR_LEFT: currentAnimation = animationSet->getAnimation(GROB_ANIM_MOVE_LEFT); break;
		}
		break;

	case GROB_STATE_DEAD:
		currentAnimation = animationSet->getAnimation(GROB_ANIM_DIE);
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


void Grob::updateAnimation()
{
	if (currentFrame == nullptr || currentAnimation == nullptr)
	{
		return;
	}

	if (state == GROB_STATE_MOVE && !moving)
	{
		changeAnimation(GROB_STATE_IDLE, true);
	}

	if (state != GROB_STATE_MOVE && moving)
	{
		changeAnimation(GROB_STATE_MOVE, true);
	}

	frameTimer += TimeController::timeController.dT;
	if (frameTimer >= currentFrame->duration)
	{
		if (currentFrame->frameNumber == currentAnimation->getEndFrameNumber())
		{
			if (state == GROB_STATE_DEAD)
			{
				if (hp > 0)
				{
					moving = true;
					changeAnimation(GROB_STATE_MOVE, true);
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


void Grob::updateHitBox()
{
	hitBox.x = collisionBox.x - 1;
	hitBox.y = collisionBox.y - 1;
	hitBox.w = collisionBox.w + 2;
	hitBox.h = collisionBox.h + 2;

	damage = (hp > 0) ? 10 : 0;
}


void Grob::updateDamages()
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
				enemy = (LivingEntity*)entity;
				if (enemy->damage > 0 && Entity::checkCollision(collisionBox, enemy->hitBox))
				{
					hp -= enemy->damage;
					if (hp > 0)
					{
						invincibleTimer = 0.1;

						SoundManager::soundmanager.playSound("enemyHit");
					}

					slideAngle = Entity::angleBetweenTwoEntities(entity, this);
					slideAmount = 300;
				}
			}
		}
	}
}

