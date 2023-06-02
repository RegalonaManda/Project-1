#pragma once;

#include "ModuleEnemies.h"


#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"

#include "Zombie.h"
#include "Enemy.h"
#include "Tomb.h"
#include "WhiteWolf.h"
#include "BrownWolf.h"
#include "Neff.h"
#include "Skull.h"
#include "Dragon.h"
#include "ModuleScene.h"

#define SPAWN_MARGIN 50


ModuleEnemies::ModuleEnemies(bool startEnabled) : Module(startEnabled)
{
	startEnabled = true;
	for (uint i = 0; i < MAX_ENEMIES; ++i)
		enemies[i] = nullptr;
}

ModuleEnemies::~ModuleEnemies()
{

}

bool ModuleEnemies::Start()
{
	//CHANGE load enemy texture
	texture = App->textures->Load("Assets/EnemiesProto.png");
	//BossText = App->textures->Load("Assets/BossGrid.png");
	//Load Enemy Death Sound FX
	enemyDeath = App->audio->LoadFx("Assets/FX/NPC_Death.wav");
	
	GraveText = App->textures->Load("Assets/toomb.png");


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

bool ModuleEnemies::AddEnemy(ENEMY_TYPE type, int x, int y, bool spawnalignment)
{
	bool ret = false;

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (spawnQueue[i].type == ENEMY_TYPE::NO_TYPE)
		{
			spawnQueue[i].type = type;
			spawnQueue[i].x = x;
			spawnQueue[i].y = y;
			spawnQueue[i].spawnalignment = spawnalignment;
			ret = true;
			break;
		}
	}

	return ret;
}

bool ModuleEnemies::AddGrave(int x, int y, bool borderL, bool borderR, bool Zombie)
{
	bool ret = false;

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (spawnQueue[i].type == ENEMY_TYPE::NO_TYPE){
			spawnQueue[i].type = ENEMY_TYPE::GRAVE;
			spawnQueue[i].x = x;
			spawnQueue[i].y = y;
			spawnQueue[i].borderL = borderL;
			spawnQueue[i].borderR = borderR;  
			spawnQueue[i].SpawnZombie = Zombie;
			ret = true;
			break;
		}
		
	}

	return ret;
}

bool ModuleEnemies::AddNeff(int x, int y, bool last)
{
	bool ret = false;

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (spawnQueue[i].type == ENEMY_TYPE::NO_TYPE) {
			spawnQueue[i].type = ENEMY_TYPE::NEFF;
			spawnQueue[i].x = x;
			spawnQueue[i].y = y;
			spawnQueue[i].last = last;
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
			if ((spawnQueue[i].x * SCREEN_SIZE) < App->render->camera.x * SCREEN_SIZE + (App->render->camera.w * SCREEN_SIZE) + SPAWN_MARGIN)
			{
				LOG("Spawning enemy at %g", spawnQueue[i].x * SCREEN_SIZE);

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
			if (enemies[i]->position.x * SCREEN_SIZE < (App->render->camera.x)*SCREEN_SIZE - SPAWN_MARGIN)
			{
				LOG("DeSpawning enemy at %g", enemies[i]->position.x * SCREEN_SIZE);

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
				
				enemies[i] = new Zombie(info.x, info.y, info.spawnalignment);

				enemies[i]->texture = texture;

				break;

			case ENEMY_TYPE::WHITEWOLF:

				enemies[i] = new WhiteWolf(info.x, info.y);

				enemies[i]->texture = texture;

				break;

			case ENEMY_TYPE::BROWNWOLF:

				enemies[i] = new BrownWolf(info.x, info.y);

				enemies[i]->texture = texture;

				break;

			case ENEMY_TYPE::NEFF:

				enemies[i] = new Neff(info.x, info.y, info.last);

				enemies[i]->texture = texture;

				break;


			case ENEMY_TYPE::GRAVE:

				enemies[i] = new Tomb(info.x, info.y,info.borderL,info.borderR, info.SpawnZombie);

				enemies[i]->texture = texture;

				break;

			case ENEMY_TYPE::SKULL:
				enemies[i] = new Skull(info.x, info.y);
				enemies[i]->texture = texture;
				break;

			case ENEMY_TYPE::DRAGON:
				enemies[i] = new Dragon(info.x, info.y);
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
			enemies[i]->OnCollision(c2); //Notify the enemy of a collision to subtract it's health
			
			if (enemies[i]->CodeN == 2) {

			}
				
			if(enemies[i]->hp <= 0){

				if (enemies[i]->AttackCollider != nullptr) { enemies[i]->AttackCollider->SetPos(1000, 1000); }
				if (enemies[i]->SelfDestruct != nullptr) { enemies[i]->SelfDestruct->SetPos(-2000, -2000); }
				if (enemies[i]->XplosionTrigger != nullptr) { enemies[i]->XplosionTrigger->SetPos(-2000, -2000); }
				
				App->audio->PlayFx(enemyDeath, 1);
				delete enemies[i];
				enemies[i] = nullptr;
				
				break;
				
			}
			
		}
		//In case player steps into attack XplosionTrigger
		if (enemies[i] != nullptr && enemies[i]->GetXplosionTriggerCollider() == c1 && c2->type == Collider::Type::PLAYER) {

			enemies[i]->attacking = true;
			//This will call from the enemy's update their Attack() method

		}
		// If enemy self destructs (Zombie)
		if (enemies[i] != nullptr && enemies[i]->GetDestructCollider() == c1 && c2 == enemies[i]->GetCollider()) {
			if (enemies[i]->SelfDestruct != nullptr) { enemies[i]->SelfDestruct->SetPos(-2000, -2000); }
			if (enemies[i]->XplosionTrigger != nullptr) { enemies[i]->XplosionTrigger->SetPos(-2000, -2000); }
			enemies[i]->hp = 0;

		}
		
	}
}