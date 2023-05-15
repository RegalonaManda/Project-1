#include "ModuleGraves.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFonts.h"


ModuleGraves::ModuleGraves(bool startEnabled) : Module(startEnabled)
{
	

}

// Load assets
bool ModuleGraves::Start()
{
	LOG("Loading grave assets");

	//texture = App->textures->Load("Assets/toomb.png");



	bool ret = true;

	return ret;
}

update_status ModuleGraves::Update()
{



	return update_status::UPDATE_CONTINUE;

}

// Update: draw background
update_status ModuleGraves::PostUpdate()
{
	// Blit


	return update_status::UPDATE_CONTINUE;
}