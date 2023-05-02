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

	mural = { 0,0,320,224 };
	blueScreen = { 321,0,320,224 };
	whiteLetters = { 321,225,320,224 };
	redLetters = { 642,225,320,224 };
	blueLetters = { 0,225,320,224 };
	SEGA = {1, 449, 80, 28};

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

	titleFade.PushBack({ 1,1,320,224 });
	titleFade.PushBack({ 322,1,320,224 });
	titleFade.PushBack({ 643,1,320,224 });
	titleFade.PushBack({ 964,1,320,224 });
	titleFade.PushBack({ 1285,1,320,224 });
	titleFade.PushBack({ 1,226,320,224 });
	titleFade.PushBack({ 322,226,320,224 });
	titleFade.PushBack({ 643,226,320,224 });
	titleFade.PushBack({ 964,226,320,224 });
	titleFade.PushBack({ 1285,226,320,224 });
	titleFade.totalFrames = 10;
	titleFade.loop = false;
	titleFade.speed = 0.15f;

	letterFlash.PushBack(whiteLetters);
	letterFlash.PushBack(redLetters);
	letterFlash.PushBack(whiteLetters);
	letterFlash.PushBack(redLetters);
	/*letterFlash.PushBack(redTitle);*/
	letterFlash.totalFrames = 5;
	letterFlash.speed = 0.25f;
	letterFlash.loop = false;
}

SceneIntro::~SceneIntro()
{
	App->textures->Unload(assetsTex);
	App->textures->Unload(blueTitleTex);
	App->textures->Unload(muralFadeTex);

}

// Load assets
bool SceneIntro::Start()
{
	LOG("Loading background assets");

	bool ret = true;
	
	blueTitleTex = App->textures->Load("Assets/Intro/blueTitleUnited.png");
	muralFadeTex = App->textures->Load("Assets/Intro/muralScreenUnited.png");
	assetsTex = App->textures->Load("Assets/Intro/introScreenUnited.png");

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;

}

update_status SceneIntro::Update()
{
	if (SegaPosX - 1.0f < (SCREEN_WIDTH / 2 - SEGA.w / 2)) {
		SegaPosX = SCREEN_WIDTH / 2 - SEGA.w / 2;
		
	}
	else {
		SegaPosX -= 1.0f;
	}

	changeCountdown--;
	if (changeCountdown <= 0) { screenChange1 = true; }

	if (screenChange1){

		currentTex = &muralFadeTex;
		currentAnim = &muralFade;
		
		if (muralFade.HasFinished()) {
			currentTex = &blueTitleTex;
			currentAnim = &titleFade;
			

		}
		currentAnim->Update();
	}

	
	
	

	if (App->input->keys[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN)
	{
		/*App->player->Enable();*/
		App->fade->FadeToBlack(this, (Module*)App->scene, 60.0f);
		
	}
	
	
	
	return update_status::UPDATE_CONTINUE;
}

// Update: draw background
update_status SceneIntro::PostUpdate()
{
	// Draw everything --------------------------------------
	if (!screenChange1) {
		App->render->Blit(assetsTex, 0, 0, &mural);
	} else {
		App->render->Blit(*currentTex, 0, 0, &currentAnim->GetCurrentFrame());

	}
	App->render->Blit(assetsTex, SegaPosX, 157, &SEGA);

	return update_status::UPDATE_CONTINUE;
}