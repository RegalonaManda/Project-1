#include "ModuleEnemies.h"

#include "Application.h"

#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"

#include "Enemy.h"

#include "Zombie.h"
#include "Wolf.h"


#define SPAWN_MARGIN 50


ModuleEnemies::ModuleEnemies(bool startEnabled) : Module(startEnabled)
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
		enemies[i] = nullptr;
}

ModuleEnemies::~ModuleEnemies()
{

}

bool ModuleEnemies::Start()
{
	//CHANGE load enemy texture
	texture = App->textures->Load("Assets/Enemies Proto.png");

	//Load Enemy Death Sound FX
	enemyDeath = App->audio->LoadFx("Assets/FX/NPC_Death.wav");
	

	return true;
}

update_status ModuleEnemies::Update()
{
	HandleEnemiesSpawn();

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
			enemies[i]->Update();
	}

	HandleEnemiesDespawn();

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleEnemies::PostUpdate()
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
			enemies[i]->Draw();
	}

	return update_status::UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleEnemies::CleanUp()
{
	LOG("Freeing all enemies");

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
	}

	return true;
}

bool ModuleEnemies::AddEnemy(ENEMY_TYPE type, int x, int y)
{
	bool ret = false;

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (spawnQueue[i].type == ENEMY_TYPE::NO_TYPE)
		{
			spawnQueue[i].type = type;
			spawnQueue[i].x = x;
			spawnQueue[i].y = y;
			ret = true;
			break;
		}
	}

	return ret;
}

void ModuleEnemies::HandleEnemiesSpawn()
{
	// Iterate all the enemies queue
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (spawnQueue[i].type != ENEMY_TYPE::NO_TYPE)
		{
			// Spawn a new enemy if the screen has reached a spawn position
			if (spawnQueue[i].x * SCREEN_SIZE < App->render->camera.x + (App->render->camera.w * SCREEN_SIZE) + SPAWN_MARGIN)
			{
				LOG("Spawning enemy at %d", spawnQueue[i].x * SCREEN_SIZE);

				SpawnEnemy(spawnQueue[i]);
				spawnQueue[i].type = ENEMY_TYPE::NO_TYPE; // Removing the newly spawned enemy from the queue
			
			}
		}
	}
	//comprobacion 
	/*for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] == nullptr)
		{
			LOG("Enemy number %d does not spawn", i);
		}
		else {
			LOG("Enemy number %d spawns", i);
		}
	}*/

}

void ModuleEnemies::HandleEnemiesDespawn()
{
	// Iterate existing enemies
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr )
		{
			// Delete the enemy when it has reached the end of the screen
			if (enemies[i]->position.x * SCREEN_SIZE < (App->render->camera.x) - SPAWN_MARGIN)
			{
				LOG("DeSpawning enemy at %d", enemies[i]->position.x * SCREEN_SIZE);

				delete enemies[i];
				enemies[i] = nullptr;
			}
		}
	}
}

void ModuleEnemies::SpawnEnemy(const EnemySpawnpoint& info)
{
	// Find an empty slot in the enemies array
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] == nullptr)
		{
			switch (info.type)
			{
			case ENEMY_TYPE::ZOMBIE:
				
				enemies[i] = new Zombie(info.x, info.y);
				

				enemies[i]->texture = texture;
				break;
			
			case ENEMY_TYPE::WHITEWOLF:

				enemies[i] = new Wolf(info.x, info.y);


				enemies[i]->texture = texture;
				break;
			}
			break;
		}

			
			
		
	}
}

void ModuleEnemies::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr && enemies[i]->GetCollider() == c1)
		{
			enemies[i]->OnCollision(c2); //Notify the enemy of a collision

			if(enemies[i]->hp <= 0){

				
				App->audio->PlayFx(enemyDeath, 1);
				delete enemies[i];
				enemies[i] = nullptr;
				
				break;
				
			}
		}
	}
}