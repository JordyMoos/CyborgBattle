#include "livingEntity.h"


void LivingEntity::updateHitBox()
{
	damage = 0;

	GroupBox* hitBoxes = (GroupBox*) GroupBuilder::findGroupByName("hitBox", currentFrame->frameData);
	if (hitBoxes != nullptr && hitBoxes->numberOfDataInGroup() > 0)
	{
		SDL_Rect newHitBox = hitBoxes->data.front();
		hitBox.x = x - currentFrame->offSet.x + newHitBox.x;
		hitBox.y = y - currentFrame->offSet.y + newHitBox.y;
		hitBox.w = newHitBox.w;
		hitBox.h = newHitBox.h;

		GroupNumber* damages = (GroupNumber*) GroupBuilder::findGroupByName("damage", currentFrame->frameData);
		if (damages != nullptr && damages->numberOfDataInGroup() > 0)
		{
			damage = damages->data.front();
		}
	}
}


void LivingEntity::updateInvincibleTimer()
{
	if (invincibleTimer > 0)
	{
		invincibleTimer -= TimeController::timeController.dT;
	}
}


void LivingEntity::draw()
{
	if (active && currentFrame != nullptr)
	{
		currentFrame->draw(animationSet->spriteSheet, x, y);
	}

	if (Globals::debugging && solid)
	{
		SDL_SetRenderDrawColor(Globals::renderer, 0x00, 0x00, 0xFF, 0xFF);
		SDL_RenderDrawRect(Globals::renderer, &lastCollisionBox);

		SDL_SetRenderDrawColor(Globals::renderer, 0xFF, 0x00, 0x00, 0xFF);
		SDL_RenderDrawRect(Globals::renderer, &collisionBox);

		SDL_SetRenderDrawColor(Globals::renderer, 0xFF, 0x00, 0xFF, 0xFF);
		SDL_RenderDrawRect(Globals::renderer, &hitBox);
	}
}
