#include "ModuleScene.h"
#include "ModuleEnemies.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCollisions.h"
#include "Enemy.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModuleScene::ModuleScene()
{
	//Background
	background.x = 0;
	background.y = 17;
	background.w = 2868;
	background.h = 273;


	//why is the stone wall a different size?
	StoneWall.x = 0;
	StoneWall.y = 75;
	StoneWall.w = 2868;
	StoneWall.h = 93;

	



}

ModuleScene::~ModuleScene()
{

}

// Load assets
bool ModuleScene::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	layer2 = App->textures->Load("Assets/Layer2.png");
	stone = App->textures->Load("Assets/stone.png");
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 400, 130);

	return ret;
}

update_status ModuleScene::Update()
{
	
	//SCREEN SCROLL
	if (ScreenScroll == true) {
		App->render->camera.x += 1;

	}

	App->enemies->Update();

	return update_status::UPDATE_CONTINUE;

}

// Update: draw background
update_status ModuleScene::PostUpdate()
{
	// Draw everything --------------------------------------
App->render->Blit(stone, 0, 90, &StoneWall, 0.65f);
App->render->Blit(layer2, 0, 5, &background, 0.75f); 


	

	return update_status::UPDATE_CONTINUE;
}