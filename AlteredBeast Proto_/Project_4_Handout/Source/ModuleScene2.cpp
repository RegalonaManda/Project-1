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

	hpAnim.PushBack({ 1,10,46,8 });
	hpAnim.PushBack({ 48,10,46,8 });
	hpAnim.PushBack({ 95,10,46,8 });
	hpAnim.PushBack({ 1,19,46,8 });
	hpAnim.PushBack({ 48,19,46,8 });
	hpAnim.PushBack({ 95,19,46,8 });
	hpAnim.PushBack({ 1,28,46,8 });
	hpAnim.PushBack({ 48,28,46,8 });
	hpAnim.PushBack({ 95,28,46,8 });

	livesAnim.PushBack({ 177,10,23,16 });
	livesAnim.PushBack({ 201,10,23,16 });
	livesAnim.PushBack({ 225,10,23,16 });
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
	uiTexture = App->textures->Load("Assets/UiElements.png");
	

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
	
	
	
	App->render->Blit(layer1, 0, 145, &background, 1.2);
	App->render->Blit(uiTexture, 30, 200, &hpAnim.GetCurrentFrame(), 0);
	App->render->Blit(uiTexture, 40, 10, &livesAnim.GetCurrentFrame(), 0);


	

	

	return update_status::UPDATE_CONTINUE;
}