#include "wall.h"


Wall::Wall(AnimationSet* animationSet)
{
	this->animationSet = animationSet;

	solid = true;
	collisionBoxWidth = 32;
	collisionBoxHeight = 32;
	collisionBoxYOffset = -16;

	updateCollisionBox();
	changeAnimation(0, false);
}


void Wall::update()
{
	updateCollisionBox();

	if (currentFrame == nullptr || currentAnimation == nullptr)
	{
		return;
	}

	frameTimer += TimeController::timeController.dT;
	if (frameTimer >= currentFrame->duration)
	{
		frameTimer = 0;
		currentFrame = currentAnimation->getNextFrame(currentFrame);
	}
}


void Wall::changeAnimation(int newState, bool resetFrameToBeginning)
{
	currentAnimation = animationSet->getAnimation("wall");
	currentFrame = currentAnimation->getFrame(0);
}


