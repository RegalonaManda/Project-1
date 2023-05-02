#ifndef __SCENE_INTRO_H__
#define __SCENE_INTRO_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class ScenePreIntro : public Module
{
public:
	//Constructor
	ScenePreIntro(bool startEnabled);

	//Destructor
	~ScenePreIntro();

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations
	update_status Update() override;

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	update_status PostUpdate() override;

public:
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* bgTexture = nullptr;
	SDL_Texture* lettersTex = nullptr;

	Animation letterFlash;
	Animation* currentAnim = nullptr;

	SDL_Rect whiteLetters;
	SDL_Rect redLetters;
	SDL_Rect blueLetters;  
};

#endif
