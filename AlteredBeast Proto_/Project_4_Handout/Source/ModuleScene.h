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

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations
	update_status Update();

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	update_status PostUpdate();

public:
	
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* stone = nullptr;
	
	SDL_Texture* layer2 = nullptr;

	SDL_Texture* trees = nullptr;
	
	SDL_Texture* sky = nullptr;

	SDL_Texture* EnemyTexture = nullptr;


	// The sprite section for the background
	SDL_Rect StoneWall;
	SDL_Rect background;
	SDL_Rect TreeLayer;
	SDL_Rect SkyLayer;
	SDL_Rect Enemy;
	Collider* backCamLimit = nullptr;
	Collider* frontCamLimit = nullptr;

	float aux;
	// The different sprite sections for the flag
	Animation flag;

	//ACTIVATE CAMERASCROLL
	bool ScreenScroll = true;
	
	int ScrollSpeed;

	bool HasEnemyDied = false;
	float enemyX = 0;
	float enemyY = 0;

	Animation deathAnim;
	Animation* current;

	

};

#endif