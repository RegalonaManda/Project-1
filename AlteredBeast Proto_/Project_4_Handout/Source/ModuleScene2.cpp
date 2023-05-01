#include "ModuleScene2.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModuleScene2::ModuleScene2(bool startEnabled) : Module(startEnabled)
{
	/*startEnabled = true;*/

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

	iconAnim.PushBack({ 142,10,17,15 });
	iconAnim.PushBack({ 160,10,17,15 });

	gameOverAnim.PushBack({ 0,0,140,20 });
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
	gameOverTexture = App->textures->Load("Assets/gameOver.png");

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
	SDL_Rect rec;
	SDL_Rect rac;
	SDL_Rect ric;
	SDL_Rect reckt = {0,0,140,20};

	if (App->player->hp == 3)
	{
		rec = hpAnim.frames[0];
	}
	if (App->player->hp == 2)
	{
		rec = hpAnim.frames[3];
	}
	if (App->player->hp == 1)
	{
		rec = hpAnim.frames[6];
	}//se necessita investigar mas como funcionan las vidas i la hp

	if (App->player->lives == 3)
	{
		rac = livesAnim.frames[0];
	}
	if (App->player->lives == 2)
	{
		rac = livesAnim.frames[1];
	}
	if (App->player->lives == 1)
	{
		rac = livesAnim.frames[2];
	}//se necessita investigar mas como funcionan las vidas i la hp

	ric = iconAnim.frames[0]; // 0 or 1

	
	
	App->render->Blit(layer1, 0, 145, &background, 1.2);

	App->render->Blit(uiTexture, 30, 200, &rec, 0); // blue hp dots
	App->render->Blit(uiTexture, 40, 10, &rac, 0);// golden lives
	App->render->Blit(uiTexture, 10, 10, &ric, 0);//human-wolf icon
	if (App->player->lives == 0) {
		App->render->Blit(gameOverTexture, SCREEN_WIDTH/2-70 , SCREEN_HEIGHT/2-10, &reckt, 0);//Game over text
	}
	

	

	

	return update_status::UPDATE_CONTINUE;
}