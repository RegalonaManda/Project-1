#include "ScenePreIntro.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "SDL/include/SDL.h"
#include "ModulePlayer.h"

ScenePreIntro::ScenePreIntro(bool startEnabled) : Module(startEnabled)
{

}

ScenePreIntro::~ScenePreIntro()
{

}

// Load assets
bool ScenePreIntro::Start()
{
	LOG("Loading background assets");
	timer = 0;
	bool ret = true;

	bgTexture = App->textures->Load("Assets/IntroScene.png");

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;

}

update_status ScenePreIntro::Update()
{

	if (App->input->keys[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN && timer >= 400)
	{
		
		App->fade->FadeToBlack(this, (Module*)App->sceneIntro, 60.0f);
		
	}
	
	++timer;

	return update_status::UPDATE_CONTINUE;
}


// Update: draw background
update_status ScenePreIntro::PostUpdate()
{
	// Draw everything --------------------------------------
	App->render->Blit(bgTexture, 0, -1*timer);

	return update_status::UPDATE_CONTINUE;
}