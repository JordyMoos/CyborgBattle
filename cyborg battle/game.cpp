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

	// Load sounds
	SoundManager::soundmanager.loadSound("hit", resPath + "Randomize2.wav");
	SoundManager::soundmanager.loadSound("enemyHit", resPath + "Hit_Hurt9.wav");
	SoundManager::soundmanager.loadSound("swing", resPath + "Randomize21.wav");
	SoundManager::soundmanager.loadSound("dash", resPath + "dash.wav");
	SoundManager::soundmanager.loadSound("growl", resPath + "Randomize34.wav");
	SoundManager::soundmanager.loadSound("enemyDie", resPath + "Randomize41.wav");

	song = Mix_LoadMUS((resPath + "Fatal Theory.wav").c_str());
	if (song != nullptr)
	{
		Mix_PlayMusic(song, -1);
	}

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

	grobAnimationSet = new AnimationSet();
	grobAnimationSet->loadAnimationSet("grob.fdset", dataGroupTypes, true, 0, true);

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
	Mix_PausedMusic();
	if (song != nullptr)
	{
		Mix_FreeMusic(song);
	}

	cleanup(backgroundImage);
	cleanup(splashImage);
	cleanup(overlayImage);
	cleanup(scoreTexture);

	Entity::removeAllFromList(&Entity::entities, false);

	delete heroAnimationSet;
	delete globAnimationSet;
	delete grobAnimationSet;
	delete wallAnimationSet;
	delete hero;

	Entity::removeAllFromList(&enemies, true);
	Entity::removeAllFromList(&walls, true);
}


void Game::update()
{
	int enemiesToBuild = 2;
	int enemiesBuilt = 0;
	float enemyBuildTimer = 3;

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

				case SDL_SCANCODE_SPACE:
					if (splashShowing)
					{
						splashShowing = false;
					}

					if (overlayTimer <= 0 && hero->hp < 1)
					{
						enemiesToBuild = 2;
						enemiesBuilt = 0;
						enemyBuildTimer = 3;
						overlayTimer = 2;
						Glob::globsKilled = 0;
						cleanup(scoreTexture);
						scoreTexture = nullptr;

						for (auto it = enemies.begin(); it != enemies.end(); it++)
						{
							(*it)->active = false;
						}

						hero->revive();
					}
				}
			}

			heroInput.update(&e);
		}

		if (hero->hp < 1 && overlayTimer > 0)
		{
			overlayTimer -= TimeController::timeController.dT;
		}

		// Update all entities
		for (auto it = Entity::entities.begin(); it != Entity::entities.end(); it++)
		{
			(*it)->update();
		}

		// Spawn enemies
		if (hero->hp > 0 && !splashShowing)
		{
			if (enemiesToBuild == enemiesBuilt)
			{
				enemiesBuilt = 0;
				enemiesToBuild++;
				enemyBuildTimer = 3;
			}

			enemyBuildTimer -= TimeController::timeController.dT;
			if (enemyBuildTimer <= 0 && enemiesBuilt < enemiesToBuild && enemies.size() < 10)
			{
				LivingEntity* enemy = nullptr;
				if (getRandomNumber(10) < 7)
				{
					enemy = new Glob(globAnimationSet);
				}
				else
				{
					enemy = new Grob(grobAnimationSet);
				}

				enemy->x = getRandomNumber(Globals::ScreenWidth - 96) + 32 + 16;
				enemy->y = getRandomNumber(Globals::ScreenHeight - 96) + 32 + 16;
				enemy->invincibleTimer = 0.1;

				enemies.push_back(enemy);
				Entity::entities.push_back(enemy);
				enemiesBuilt++;
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

	if (splashShowing)
	{
		renderTexture(splashImage, Globals::renderer, 0, 0);
	}
	else
	{
		renderTexture(backgroundImage, Globals::renderer, 0, 0);

		// Draw all entities
		Entity::entities.sort(Entity::entityCompare);
		for (auto it = Entity::entities.begin(); it != Entity::entities.end(); it++)
		{
			(*it)->draw();
		}

		if (overlayTimer <= 0 && hero->hp < 1)
		{
			renderTexture(overlayImage, Globals::renderer, 0, 0);
			if (scoreTexture == nullptr)
			{
				SDL_Color color = { 255, 255, 255, 255 };
				stringstream ss;
				ss << "Enemies dispatched: " << (Glob::globsKilled + Grob::grobsKilled);

				string resPath = getResourcePath();
				scoreTexture = renderText(ss.str(), resPath + "vermin_vibes_1989.ttf", color, 30, Globals::renderer);
			}
			renderTexture(scoreTexture, Globals::renderer, 20, 50);
		}
	}

	SDL_RenderPresent(Globals::renderer);
}

