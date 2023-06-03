#ifndef __MODULE_SCENE_H__
#define __MODULE_SCENE_H__

#include "Module.h"
#include "Animation.h"
#include "Colliders.h"
#include "EnemyDeath.h"

struct SDL_Texture;

class ModuleScene : public Module
{
public:
	//Constructor
	ModuleScene(bool startEnabled);

	//Destructor
	~ModuleScene() {}


	void PlaceEnemies();

	void TestEnemies();

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations
	update_status Update();

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	update_status PostUpdate();

	bool CleanUp() override;

public:
	
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* stone = nullptr;
	
	SDL_Texture* layer2 = nullptr;

	SDL_Texture* trees = nullptr;
	
	SDL_Texture* sky = nullptr;

	SDL_Texture* EnemyTexture = nullptr;

	SDL_Texture* ExplosionText = nullptr;

	
	// The sprite section for the background
	SDL_Rect StoneWall;
	SDL_Rect background;
	SDL_Rect TreeLayer;
	SDL_Rect SkyLayer;
	SDL_Rect Enemy;
	SDL_Rect explosion;
	Collider* backCamLimit = nullptr;
	Collider* frontCamLimit = nullptr;

	Collider* GravityCollider1;
	Collider* GravityCollider2;
	Collider* GravityCollider3;
	Collider* GravityCollider4;

	float aux;
	// The different sprite sections for the flag
	Animation flag;

	//ACTIVATE CAMERASCROLL
	bool ScreenScroll = true;
	
	int ScrollSpeed;

	bool HasEnemyDied = false;
	bool EnemyAttacking = false;
	float enemyX = 0;
	float enemyX2 = 0;
	float enemyY = 0;
	int explosionCnt = 40;

	int EnemyCN = 0;

	int EnemyCoolDown = 30;
	
	Animation deathAnim;
	Animation explode;
	Animation* Ecurrent;
	Animation* Xcurrent;

	Animation* Bcurrent;
	Animation BDeathRight;
	Animation BDeathLeft;

	Animation* Dcurrent;
	Animation DDeath;

	Animation* Wcurrent;
	Animation WDeathRight;
	Animation WDeathLeft;
	

	float waveRatio = 0.0f;
	float waveRatioSpeed = 0.1f;
	int waveHeight = 15;

	int spawn_y = 0;
	

	

};

#endif