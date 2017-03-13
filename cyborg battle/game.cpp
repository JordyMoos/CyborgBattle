#include "game.h"

#include <string>


Game::Game()
{
	splashShowing = true;
	overlayTimer = 2;

	string resPath = getResourcePath();
	backgroundImage = loadTexture(resPath + "map.png", Globals::renderer);
	splashImage = loadTexture(resPath + "cyborgtitle.png", Globals::renderer);
	overlayImage = loadTexture(resPath + "overlay.png", Globals::renderer);

	// Holds a list of group types
	// Describing the types of groups of data our frame can have
	list<DataGroupType> dataGroupTypes;

	DataGroupType collisionBoxType;
	collisionBoxType.groupName = "collisionBox";
	collisionBoxType.dataType = DataGroupType::DATATYPE_BOX;

	DataGroupType hitBoxType;
	hitBoxType.groupName = "hitBox";
	hitBoxType.dataType = DataGroupType::DATATYPE_BOX;

	DataGroupType damageType;
	damageType.groupName = "damage";
	damageType.dataType = DataGroupType::DATATYPE_NUMBER;

	dataGroupTypes.push_back(collisionBoxType);
	dataGroupTypes.push_back(hitBoxType);
	dataGroupTypes.push_back(damageType);

	heroAnimationSet = new AnimationSet();
	heroAnimationSet->loadAnimationSet("udemyCyborg.fdset", dataGroupTypes, true, 0, true);

	globAnimationSet = new AnimationSet();
	globAnimationSet->loadAnimationSet("glob.fdset", dataGroupTypes, true, 0, true);

	wallAnimationSet = new AnimationSet();
	wallAnimationSet->loadAnimationSet("wall.fdset", dataGroupTypes);

	hero = new Hero(heroAnimationSet);
	heroInput.hero = hero;

	Entity::entities.push_back(hero);

	// Walls on top and bottom of screen
	int wallSize = 32;
	for (int i = 0; i < Globals::ScreenWidth; i += wallSize)
	{
		// Top row
		Wall* wall = new Wall(wallAnimationSet);
		wall->x = i + (wallSize / 2);
		wall->y = wallSize / 2;

		walls.push_back(wall);
		Entity::entities.push_back(wall);

		// Bottom row
		wall = new Wall(wallAnimationSet);
		wall->x = i + (wallSize / 2);
		wall->y = Globals::ScreenHeight - wallSize / 2;

		walls.push_back(wall);
		Entity::entities.push_back(wall);
	}

	// Wall sides
	for (int i = wallSize; i < Globals::ScreenHeight - wallSize; i += wallSize)
	{
		// Left row
		Wall* wall = new Wall(wallAnimationSet);
		wall->x = wallSize / 2;
		wall->y = i + (wallSize / 2);

		walls.push_back(wall);
		Entity::entities.push_back(wall);

		// Right row
		wall = new Wall(wallAnimationSet);
		wall->x = Globals::ScreenWidth - wallSize / 2;
		wall->y = i + (wallSize / 2);

		walls.push_back(wall);
		Entity::entities.push_back(wall);
	}
}


Game::~Game()
{
	cleanup(backgroundImage);
	Entity::removeAllFromList(&Entity::entities, false);

	delete heroAnimationSet;
	delete globAnimationSet;
	delete wallAnimationSet;
	delete hero;

	Entity::removeAllFromList(&enemies, true);
	Entity::removeAllFromList(&walls, true);
}


void Game::update()
{
	int enemiesToBuild = 2;
	int enemiesBuilt = 0;
	float enemyBuildTimer = 1;

	bool quit = false;
	SDL_Event e;

	TimeController::timeController.reset();

	while ( ! quit)
	{
		TimeController::timeController.updateTime();

		Entity::removeInactiveEntitiesFromList(&Entity::entities, false);
		Entity::removeInactiveEntitiesFromList(&enemies, true);

		// Handle input
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}

			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.scancode)
				{
				case SDL_SCANCODE_ESCAPE:
					quit = true;
					break;
				}
			}

			heroInput.update(&e);
		}

		// Update all entities
		for (auto it = Entity::entities.begin(); it != Entity::entities.end(); it++)
		{
			(*it)->update();
		}

		// Spawn enemies
		if (hero->hp > 0)
		{
			if (enemiesToBuild == enemiesBuilt)
			{
				enemiesToBuild = enemiesToBuild * 2;
				enemiesBuilt = 0;
				enemyBuildTimer = 4;
			}

			enemyBuildTimer -= TimeController::timeController.dT;
			if (enemyBuildTimer <= 0 && enemiesBuilt < enemiesToBuild && enemies.size() < 10)
			{
				Glob* enemy = new Glob(globAnimationSet);
				enemy->x = getRandomNumber(Globals::ScreenWidth - 96) + 32 + 16;
				enemy->y = getRandomNumber(Globals::ScreenHeight - 96) + 32 + 16;
				enemy->invincibleTimer = 0.1;

				enemies.push_back(enemy);
				Entity::entities.push_back(enemy);
			}
		}

		// Draw all entities
		draw();
	}
}


void Game::draw()
{
	SDL_SetRenderDrawColor(Globals::renderer, 145, 133, 129, 255);
	SDL_RenderClear(Globals::renderer);

	renderTexture(backgroundImage, Globals::renderer, 0, 0);

	// Draw all entities
	Entity::entities.sort(Entity::entityCompare);
	for (auto it = Entity::entities.begin(); it != Entity::entities.end(); it++)
	{
		(*it)->draw();
	}

	SDL_RenderPresent(Globals::renderer);
}

