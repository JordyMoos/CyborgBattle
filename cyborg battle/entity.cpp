#include "entity.h"


void Entity::update()
{

}


void Entity::draw()
{
	if (active && currentFrame != nullptr)
	{
		currentFrame->draw(animationSet->spriteSheet, x, y);
	}

	if (Globals::debugging && solid)
	{
		SDL_SetRenderDrawColor(Globals::renderer, 0xFF, 0x00, 0x00, 0xFF);
		SDL_RenderDrawRect(Globals::renderer, &collisionBox);
	}
}


void Entity::move(float angle)
{
	moving = true;
	moveSpeed = moveSpeedMax;
	this->angle = angle;

	int newDirection = angleToDirection(angle);
	if (direction != newDirection)
	{
		direction = newDirection;
		changeAnimation(state, false);
	}
}


void Entity::updateMovement()
{
	updateCollisionBox();
	lastCollisionBox = collisionBox;

	totalXMove = 0;
	totalYMove = 0;

	if (moveSpeed > 0)
	{
		float moveDistance = moveSpeed * TimeController::timeController.dT * moveLerp;
		if (moveDistance > 0)
		{
			float xMove = moveDistance * cos(angle * Globals::PI / 180);
			float yMove = moveDistance * sin(angle * Globals::PI / 180);

			x += xMove;
			y += yMove;

			totalXMove = xMove;
			totalYMove = yMove;

			if ( ! moving)
			{
				moveSpeed -= moveDistance;
			}
		}
	}

	if (slideAmount > 0)
	{
		float slideDistance = slideAmount * TimeController::timeController.dT * moveLerp;
		if (slideDistance > 0)
		{
			float xMove = slideDistance * cos(slideAngle * Globals::PI / 180);
			float yMove = slideDistance * sin(slideAngle * Globals::PI / 180);

			x += xMove;
			y += yMove;

			totalXMove += xMove;
			totalYMove += yMove;
			slideAmount -= slideDistance;
		}
		else
		{
			slideAmount = 0;
		}
	}

	// Update collisionbox up to where we are now
	updateCollisionBox();

	// To help with collision checking, union collisionBox with lastCollisionBox
	SDL_UnionRect(&collisionBox, &lastCollisionBox, &collisionBox);
}


void Entity::updateCollisionBox()
{
	collisionBox.x = x - collisionBox.w / 2;
	collisionBox.y = y + collisionBoxYOffset;
	collisionBox.w = collisionBoxWidth;
	collisionBox.h = collisionBoxHeight;
}


void Entity::updateCollisions()
{
	if (active && collideWithSolid && (moveSpeed > 0 || slideAmount > 0))
	{
		// List of potential collisions
		list<Entity*> collisions;

		for (auto it = entities.begin(); it != entities.end(); it++)
		{
			if ((*it)->active && (*it)->type != this->type
				&& (*it)->solid
				&& checkCollision(collisionBox, (*it)->collisionBox))
			{

			}
		}
	}
}


float Entity::distanceBetweenTwoRects(SDL_Rect &lhs, SDL_Rect &rhs)
{

}


float Entity::distanceBetweenTwoEntities(Entity *lhs, Entity *rhs)
{

}


float Entity::angleBetweenTwoEntities(Entity *lhs, Entity *rhs)
{

}


bool Entity::checkCollision(SDL_Rect &lhs, SDL_Rect &rhs)
{

}


int Entity::angleToDirection(float angle)
{

}


float Entity::angleBetweenTwoPoints(float cx1, float cy1, float cx2, float cy2)
{

}


float Entity::angleBetweenTwoRects(SDL_Rect &lhs, SDL_Rect &rhs)
{

}


bool Entity::entityCompare(const Entity* const &lhs, const Entity* const &rhs)
{

}


void Entity::removeInactiveEntitiesFromList(list<Entity*> *entities, bool shouldDelete)
{

}


void Entity::removeAllFromList(list<Entity*> *entities, bool shouldDelete)
{

}


