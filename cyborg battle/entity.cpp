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
		Entity* entity;

		for (auto it = entities.begin(); it != entities.end(); it++)
		{
			entity = *it;
			if (entity->active && entity->type != this->type
				&& entity->solid
				&& checkCollision(collisionBox, entity->collisionBox))
			{
				collisions.push_back(entity);
			}
		}

		// Multisample check for collisions from where we started to where we are planning to go to
		if (collisions.size() > 0)
		{
			updateCollisionBox();

			float boxTravelSize = (collisionBox.w < collisionBox.h)
				? collisionBox.w / 4
				: collisionBox.h / 4;

			SDL_Rect sampleBox = lastCollisionBox;
			float movementAngle = angleBetweenTwoRects(lastCollisionBox, collisionBox);

			bool foundCollision = false;
			while ( ! foundCollision)
			{
				// Check sample box for collisions
				SDL_Rect intersectionBox;
				Entity* entity;
				for (auto it = collisions.begin(); it != collisions.end(); it++)
				{
					entity = *it;
					if (SDL_IntersectRect(&sampleBox, &entity->collisionBox, &intersectionBox))
					{
						foundCollision = true;
						moveSpeed = 0;
						moving = false;
						slideAngle = angleBetweenTwoEntities(entity, this);

						// Collision resolution
						// Move to the direction based on which direction we started
						if (intersectionBox.w < intersectionBox.h)
						{
							if (lastCollisionBox.x + lastCollisionBox.w / 2 < entity->collisionBox.x + entity->collisionBox.w / 2)
							{
								sampleBox.x -= intersectionBox.w;
							}
							else
							{
								sampleBox.x += intersectionBox.w;
							}
						}
						else
						{
							if (lastCollisionBox.y + lastCollisionBox.h / 2 < entity->collisionBox.y + entity->collisionBox.h / 2)
							{
								sampleBox.y -= intersectionBox.h;
							}
							else
							{
								sampleBox.y += intersectionBox.h;
							}
						}
					}
				}

				// Stop if we have a collision or when we are at the end
				if (foundCollision || (sampleBox.x == collisionBox.x && sampleBox.y == collisionBox.y))
				{
					break;
				}

				// Increment sample box more to the end
				if (distanceBetweenTwoRects(sampleBox, collisionBox) > boxTravelSize)
				{
					float xMove = boxTravelSize * cos(movementAngle * Globals::PI / 180);
					float yMove = boxTravelSize * sin(movementAngle * Globals::PI / 180);

					sampleBox.x += xMove;
					sampleBox.y += yMove;
				}
				else
				{
					sampleBox = collisionBox;
				}
			}

			if (foundCollision)
			{
				// Move our entity to where our samplebox ended up
				slideAmount = slideAmount / 2;
				x = sampleBox.x + sampleBox.w / 2;
				y = sampleBox.y - collisionBoxYOffset;
			}

			updateCollisionBox();
		}
	}
}


float Entity::distanceBetweenTwoRects(SDL_Rect &r1, SDL_Rect &r2)
{
	SDL_Point e1, e2;
	e1.x = r1.x + r1.w / 2;
	e1.y = r1.y + r1.h / 2;

	e2.x = r2.x + r2.w / 2;
	e2.y = r2.y + r2.h / 2;

	float distance = abs(sqrt(pow(e2.x - e1.x, 2) + pow(e2.y - e1.y, 2)));

	return distance;
}


float Entity::distanceBetweenTwoEntities(Entity *e1, Entity *e2)
{
	float distance = abs(sqrt(pow(e2->x - e1->x, 2) + pow(e2->y - e1->y, 2)));
	
	return distance;
}


float Entity::angleBetweenTwoEntities(Entity *e1, Entity *e2)
{
	float dx, dy;
	float x1 = e1->x, y1 = e1->y;
	float x2 = e2->x, y2 = e2->y;

	dx = x2 - x1;
	dy = y2 = y1;

	return atan2(dy, dx) * 180 / Globals::PI;
}


bool Entity::checkCollision(SDL_Rect r1, SDL_Rect r2)
{
	return SDL_IntersectRect(&r1, &r2, nullptr);
}


int Entity::angleToDirection(float angle)
{
	if ((angle >= 0 && angle <= 45) || (angle >= 315 && angle <= 360))
	{
		return DIR_RIGHT;
	}
	else if (angle >= 45 && angle <= 135)
	{
		return DIR_DOWN;
	}
	else if (angle >= 135 && angle <= 225)
	{
		return DIR_LEFT;
	}
	else
	{
		return DIR_UP;
	}
}


float Entity::angleBetweenTwoPoints(float cx1, float cy1, float cx2, float cy2)
{
	float dx = cx2 - cx1;
	float dy = cy2 - cy1;

	return atan2(dy, dx) * 180 / Globals::PI;
}


float Entity::angleBetweenTwoRects(SDL_Rect &r1, SDL_Rect &r2)
{
	float cx1 = r1.x + r1.w / 2;
	float cy1 = r1.y + r1.h / 2;

	float cx2 = r2.x + r2.w / 2;
	float cy2 = r2.y + r2.h / 2;

	return angleBetweenTwoPoints(cx1, cy1, cx2, cy2);
}


list<Entity*> Entity::entities;


bool Entity::entityCompare(const Entity* const &lhs, const Entity* const &rhs)
{
	if (lhs == nullptr || rhs == nullptr)
	{
		return false;
	}

	return lhs->y < rhs->y;
}


void Entity::removeInactiveEntitiesFromList(list<Entity*> *entities, bool shouldDelete)
{
	Entity *entity = nullptr;
	for (auto it = entities->begin(); it != entities->end(); )
	{
		entity = *it;
		if ( ! entity->active)
		{
			if (shouldDelete)
			{
				delete entity;
			}
			it = entities->erase(it);
		}
		else
		{
			it++;
		}
	}
}


void Entity::removeAllFromList(list<Entity*> *entities, bool shouldDelete)
{
	Entity *entity = nullptr;
	for (auto it = entities->begin(); it != entities->end();)
	{
		entity = *it;
		if (shouldDelete)
		{
			delete entity;
		}
		it = entities->erase(it);
	}
}


