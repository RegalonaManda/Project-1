#ifndef __SCENE_INTRO_H__
#define __SCENE_INTRO_H__

#include "Module.h"
#include "Animation.h"


struct SDL_Texture;

class SceneIntro : public Module
{
public:
	//Constructor
	SceneIntro(bool startEnabled);

	//Destructor
	~SceneIntro();

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

	int changeCountdown = 50;

	SDL_Texture* muralTex = nullptr;
	SDL_Texture* blueTitleTex = nullptr;
	SDL_Texture* muralFadeTex = nullptr;
	SDL_Texture* assetsTex = nullptr;
	SDL_Texture** currentTex = nullptr;
	
	bool screenChange1 = false;
	bool screenChange2 = false;

	Animation muralFade;
	Animation titleFade;
	Animation letterFlash;
	Animation* currentAnim = nullptr;

	SDL_Rect blueScreen;
	SDL_Rect mural;
	SDL_Rect whiteLetters;
	SDL_Rect redLetters;
	SDL_Rect blueLetters; 
	SDL_Rect SEGA;
	float SegaPosX = 320.0f;
	float SegaSpeed = 1.0f;

	SDL_Rect InsertCoin;
	float CoinX; 
	int CoinInterval = 25;
	bool CoinVisible = false;
	
};

#endif
