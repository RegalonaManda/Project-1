#include "SceneIntro.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "SDL/include/SDL.h"
#include "ModulePlayer.h"

SceneIntro::SceneIntro(bool startEnabled) : Module(startEnabled)
{
	whiteLetters = { 321,225,320,224 };
	redLetters = { 642,225,320,224 };
	blueLetters = { 0,225,320,224 };

	letterFlash.PushBack(whiteLetters);
	letterFlash.PushBack(redLetters);
	letterFlash.PushBack(whiteLetters);
	letterFlash.PushBack(redLetters);
	letterFlash.totalFrames = 4;
	letterFlash.speed = 0.05f;
	letterFlash.loop = false;
}

SceneIntro::~SceneIntro()
{

}

// Load assets
bool SceneIntro::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	bgTexture = App->textures->Load("Assets/Intro/introScreen6.png");
	lettersTex = App->textures->Load("Assets/Intro/introScreenUnited.png");

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;

}

update_status SceneIntro::Update()
{
	if (App->input->keys[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN)
	{
		App->player->Enable();
		App->fade->FadeToBlack(this, (Module*)App->scene, 60.0f);
		
	}
	else {
		currentAnim = &letterFlash;
	}
	return update_status::UPDATE_CONTINUE;
}

// Update: draw background
update_status SceneIntro::PostUpdate()
{
	// Draw everything --------------------------------------
	App->render->Blit(lettersTex, 0, 0, &currentAnim->GetCurrentFrame());

	return update_status::UPDATE_CONTINUE;
}