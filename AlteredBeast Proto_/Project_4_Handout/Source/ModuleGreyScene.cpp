#include "ModuleScene.h"
#include "ModuleGreyScene.h"
#include "ModuleEnemies.h"
#include "ModulePower.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCollisions.h"
#include "ModuleAudio.h"



ModuleGreyScene::ModuleGreyScene(bool startEnabled) : Module(startEnabled)
{

	startEnabled = true;
	//Background
	background.x = 0;
	background.y = 17;
	background.w = 2868;
	background.h = 273;


	//Stone wal
	StoneWall.x = 0;
	StoneWall.y = 75;
	StoneWall.w = 2868;
	StoneWall.h = 93;

	// Tree layer
	TreeLayer.x = 0;
	TreeLayer.y = 53;
	TreeLayer.w = 1743;
	TreeLayer.h = 75;

	//sky
	SkyLayer.x = 0;
	SkyLayer.y = 0;
	SkyLayer.w = 1743;
	SkyLayer.h = 273;

	
}


// Load assets
bool ModuleGreyScene::Start()
{
	LOG("Loading Grey background assets");

	bool ret = true;

	layer2 = App->textures->Load("Assets/grey_Layer2.png");
	stone = App->textures->Load("Assets/grey_stone.png");
	trees = App->textures->Load("Assets/grey_trees.png");
	sky = App->textures->Load("Assets/grey_sky.png");
	




	return ret;
}

update_status ModuleGreyScene::Update()
{

	

	


	




	return update_status::UPDATE_CONTINUE;

}

// Update: draw background
update_status ModuleGreyScene::PostUpdate()
{
	// Draw everything --------------------------------------

	if(Grey == true) {
		App->render->Blit(sky, 0, 0, &SkyLayer, 0.4f);
		App->render->Blit(trees, 0, 58, &TreeLayer, 0.5f);
		App->render->Blit(stone, 0, 90, &StoneWall, 0.65f);
		App->render->Blit(layer2, 0, 5, &background, 0.75f);

	}

	


	return update_status::UPDATE_CONTINUE;
}

bool ModuleGreyScene::CleanUp() {

	App->textures->Unload(layer2);
	App->textures->Unload(stone);
	App->textures->Unload(trees);
	App->textures->Unload(sky);
	
	return true;
}