#pragma once

#include "globals.h"
#include "timeController.h"
#include "animationSet.h"


class Entity
{
public:

	static const int DIR_UP = 0;
	static const int DIR_RIGHT = 1;
	static const int DIR_DOWN = 2;
	static const int DIR_LEFT = 3;
	static const int DIR_NONE = -1;

	int state;

	float x, y;
	int direction;
	bool solid = true;
	bool collideWithSolid = true;
	bool active = true;
	string type = "entity";
	bool moving;
	float angle;
	float moveSpeed;
	float moveSpeedMax;
	float slideAngle;
	float slideAmount;
	float moveLerp = 4;
	float totalXMove, totalYMove;

	SDL_Rect collisionBox;
	SDL_Rect lastCollisionBox;
	int collisionBoxWidth, collisionBoxHeight;
	float collisionBoxYOffset;

	AnimationSet* animationSet;
	Animation* currentAnimation;
	Frame* currentFrame;
	float frameTimer;

	virtual void update();
	virtual void draw();

	virtual void move(float angle);
	virtual void updateMovement();
	virtual void updateCollisionBox();

	virtual void changeAnimation(int newState, bool resetFrameToBeginning) = 0;
	virtual void updateCollisions();

	static float distanceBetweenTwoRects(SDL_Rect &r1, SDL_Rect &r2);
	static float distanceBetweenTwoEntities(Entity *e1, Entity *e2);
	static float angleBetweenTwoEntities(Entity *e1, Entity *e2);
	static bool checkCollision(SDL_Rect r1, SDL_Rect r2);
	static int  angleToDirection(float angle);
	static float angleBetweenTwoPoints(float cx1, float cy1, float cx2, float cy2);
	static float angleBetweenTwoRects(SDL_Rect &r1, SDL_Rect &r2);

	static list<Entity*> entities;
	static bool entityCompare(const Entity* const &lhs, const Entity* const &rhs);
	static void removeInactiveEntitiesFromList(list<Entity*> *entities, bool shouldDelete);
	static void removeAllFromList(list<Entity*> *entities, bool shouldDelete);
};
