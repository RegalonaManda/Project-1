#ifndef __MODULE_ENEMIES_H__
#define __MODULE_ENEMIES_H__

#include "Module.h"

#define MAX_ENEMIES 150



enum class ENEMY_TYPE
{
	NO_TYPE,
	ZOMBIE,
	SKULL,
	WHITEWOLF,
	BROWNWOLF,
	NEFF,
	DRAGON,
	GRAVE
};

struct EnemySpawnpoint
{
	ENEMY_TYPE type = ENEMY_TYPE::NO_TYPE;
	float x, y;
	bool spawnalignment;

	bool borderL = false;
	bool borderR = false;

	bool last = false;

	bool SpawnZombie = false;
};

class Enemy;
struct SDL_Texture;

class ModuleEnemies : public Module
{
public:
	// Constructor
	ModuleEnemies(bool startEnabled);

	// Destructor
	~ModuleEnemies();

	// Called when the module is activated
	// Loads the necessary textures for the enemies
	bool Start() override;

	// Called at the middle of the application loop
	// Handles all enemies logic and spawning/despawning
	update_status Update() override;

	// Called at the end of the application loop
	// Iterates all the enemies and draws them
	update_status PostUpdate() override;

	// Called on application exit
	// Destroys all active enemies left in the array
	bool CleanUp() override;

	// Called when an enemi collider hits another collider
	// The enemy is destroyed and an explosion particle is fired
	void OnCollision(Collider* c1, Collider* c2) override;

	// Add an enemy into the queue to be spawned later
	bool AddEnemy(ENEMY_TYPE type, int x, int y, bool spawnalignment);

	bool AddGrave( int x, int y, bool borderL, bool borderR, bool Zombie);

	bool AddNeff(int x, int y, bool last);

	bool borderL = false;
	bool borderR = false;
	bool last = false;

	// Iterates the queue and checks for camera position
	void HandleEnemiesSpawn();

	// Destroys any enemies that have moved outside the camera limits
	void HandleEnemiesDespawn();

public:
	// Spawns a new enemy using the data from the queue
	void SpawnEnemy(const EnemySpawnpoint& info);

	// A queue with all spawn points information
	EnemySpawnpoint spawnQueue[MAX_ENEMIES];

	// All spawned enemies in the scene
	Enemy* enemies[MAX_ENEMIES] = { nullptr };

	
public:
	// The enemies sprite sheet
	SDL_Texture* texture = nullptr;

	SDL_Texture* BossText = nullptr;

	SDL_Texture* GraveText = nullptr;

	uint enemyDeath;


};

#endif // __MODULE_ENEMIES_H__