#include "ModuleScene2.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModuleScene2::ModuleScene2()
{

	// Background / sky
	background.x = 0;
	background.y = 191;
	background.w = 4283;
	background.h = 84;



	
}

ModuleScene2::~ModuleScene2()
{

}

// Load assets
bool ModuleScene2::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	layer1 = App->textures->Load("Assets/Layer1.png");
	

	return ret;
}

update_status ModuleScene2::Update()
{
	/*flag.Update();
	App->render->Blit(layer1, 0, 120, &background, 0.75f);*/
	return update_status::UPDATE_CONTINUE;
}

// Update: draw background
update_status ModuleScene2::PostUpdate()
{
	// Draw everything --------------------------------------
	App->render->Blit(layer1, 0, 145, &background, 1.2);


	

	

	return update_status::UPDATE_CONTINUE;
}