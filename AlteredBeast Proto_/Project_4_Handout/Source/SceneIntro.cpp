#include "SceneIntro.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "SDL/include/SDL.h"
#include "ModulePlayer.h"

//video reference: https://www.youtube.com/watch?v=NVrWoPUSW6E&ab_channel=ArcadeGameClips (first 7 seconds)

SceneIntro::SceneIntro(bool startEnabled) : Module(startEnabled)
{
	

	//Rects
	mural = { 1,1126,320,224 };
	blueScreen = { 1285,226,320,224 };
	SEGA = { 1285,1126,80,27 };
	InsertCoin = { 1285,1154,86,8 };

	//variables 
	CoinX = (float)(SCREEN_WIDTH / 2.0f) - (InsertCoin.w / 2.0f);
	
	//Animations
	muralFade.PushBack({ 1,1,320,224 });
	muralFade.PushBack({ 322,1,320,224 });
	muralFade.PushBack({ 643,1,320,224 });
	muralFade.PushBack({ 964,1,320,224 });
	muralFade.PushBack({ 1285,1,320,224 });
	muralFade.PushBack({ 1,226,320,224 });
	muralFade.PushBack({ 322,226,320,224 });
	muralFade.PushBack({ 643,226,320,224 });
	muralFade.PushBack({ 964,226,320,224 });
	muralFade.PushBack({ 1285,226,320,224 });
	muralFade.totalFrames = 10;
	muralFade.loop = false;
	muralFade.speed = 0.15f;

	titleFade.PushBack({ 1,451,320,224 });
	titleFade.PushBack({ 322,451,320,224 });
	titleFade.PushBack({ 643,451,320,224 });
	titleFade.PushBack({ 964,451,320,224 });
	titleFade.PushBack({ 1285,451,320,224 });
	titleFade.PushBack({ 1,676,320,224 });
	titleFade.PushBack({ 322,676,320,224 });
	titleFade.PushBack({ 643,676,320,224 });
	titleFade.PushBack({ 964,676,320,224 });
	titleFade.PushBack({ 1285,676,320,224 });
	titleFade.totalFrames = 10;
	titleFade.loop = false;
	titleFade.speed = 0.15f;

	bgFade.PushBack({ 1,901,320,224 });
	bgFade.PushBack({ 322,901,320,224 });
	bgFade.PushBack({ 643,901,320,224 });
	bgFade.PushBack({ 964,901,320,224 });
	bgFade.PushBack({ 1285,901,320,224 });
	bgFade.loop = false;
	bgFade.totalFrames = 5;
	bgFade.speed = 0.07f;

	letterFlash.PushBack({ 322,1126,320,224 });
	letterFlash.PushBack({ 643,1126,320,224 });
	letterFlash.PushBack({ 322,1126,320,224 });
	letterFlash.PushBack({ 643,1126,320,224 });
	letterFlash.PushBack({ 964,1126,320,224 });
	letterFlash.totalFrames = 5;
	letterFlash.speed = 0.15f;
	letterFlash.loop = false;
}

SceneIntro::~SceneIntro()
{
	App->textures->Unload(assetsTex);
}

// Load assets
bool SceneIntro::Start()
{
	LOG("Loading background assets");

	bool ret = true;
	
	
	assetsTex = App->textures->Load("Assets/Intro/AnimationsCombined.png");

	App->render->camera.x = 0;
	App->render->camera.y = 0;
	
	return ret;

}

update_status SceneIntro::Update()
{
	if (SegaPosX - SegaSpeed < (SCREEN_WIDTH / 2 - SEGA.w / 2)) {
		SegaPosX = SCREEN_WIDTH / 2 - SEGA.w / 2;
		SEGAStop = true;
	}
	else {
		SegaPosX -= SegaSpeed;
	}

	CoinInterval--;
	if (CoinInterval <= 0) {
		CoinVisible = !CoinVisible;
		CoinInterval = 50;
	}

	changeCountdown--;
	if (changeCountdown <= 0) { screenChange = true; }

	if (screenChange){

		currentAnim = &muralFade;
		
		if (muralFade.HasFinished()) {
			currentAnim = &titleFade;
			if (titleFade.HasFinished()) {
				currentAnim = &bgFade;
			}
		}
		if (bgFade.HasFinished() && SEGAStop) { currentAnim = &letterFlash; }

		currentAnim->Update();
	}

	if (App->input->keys[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN)
	{
		
		App->fade->FadeToBlack(this, (Module*)App->scene, 60.0f);
		App->player->Enable();
		
		
	}
	

	return update_status::UPDATE_CONTINUE;
}

// Update: draw background
update_status SceneIntro::PostUpdate()
{
	// Draw everything --------------------------------------
	if (!screenChange) {
		App->render->Blit(assetsTex, 0, 0, &mural);
	} 
	else {
		App->render->Blit(assetsTex, 0, 0, &currentAnim->GetCurrentFrame());

	}

	App->render->Blit(assetsTex, SegaPosX, 157, &SEGA);

	if (CoinVisible) {
		App->render->Blit(assetsTex, CoinX, 193, &InsertCoin);
	}
	


	return update_status::UPDATE_CONTINUE;
}