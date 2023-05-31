#ifndef __MODULE_SCENE2_H__
#define __MODULE_SCENE2_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class ModuleScene2 : public Module
{
public:
	//Constructor
	ModuleScene2(bool startEnabled);

	//Destructor
	~ModuleScene2();

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override;

	bool CleanUp() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations
	update_status Update();

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	update_status PostUpdate();

public:

	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* layer1 = nullptr;
	SDL_Texture* uiTexture = nullptr;
	SDL_Texture* gameOverTexture = nullptr;

	SDL_Texture* layer2 = nullptr;
	SDL_Texture* grey_layer = nullptr;

	// The sprite section for the background
	SDL_Rect background;
	SDL_Rect grey_background;

	// The different sprite sections for the flag

	Animation hpAnim;
	Animation livesAnim;
	Animation iconAnim;
	Animation gameOverAnim;

	Collider* Ground = nullptr;

	bool killedBoss = false;
	int FadeCnt = 120;

	int playOnce = 0;

};

#endif