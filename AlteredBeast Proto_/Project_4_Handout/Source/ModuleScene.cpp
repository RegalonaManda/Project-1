#include "ModuleScene.h"
#include "ModuleEnemies.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCollisions.h"
#include "ModuleAudio.h"
#include "Enemy.h"


ModuleScene::ModuleScene()
{
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
	trees = App->textures->Load("Assets/trees.png");
	sky = App->textures->Load("Assets/sky.png");
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 400, 120);
	App->enemies->AddEnemy(ENEMY_TYPE::WHITEWOLF, 600, 140);
	App->audio->PlayMusic("Assets/Music/rise-from-your-grave.ogg", 1.0f, 50);


	backCamLimit = App->collisions->AddCollider({ App->render->camera.x, App->render->camera.y, 10, SCREEN_HEIGHT }, Collider::Type::CAMLIMIT, (Module*)App->player);
	frontCamLimit = App->collisions->AddCollider({ App->render->camera.x + SCREEN_WIDTH-10, App->render->camera.y + SCREEN_WIDTH - 10, 10, SCREEN_HEIGHT }, Collider::Type::CAMLIMIT, (Module*)App->player);

	return ret;
}

update_status ModuleScene::Update()
{
	
	//SCREEN SCROLL
	if (ScreenScroll == true) {
		App->render->camera.x += 1;

		aux = (App->render->camera.x + (SCREEN_WIDTH - 10) * SCREEN_SIZE ) * 0.3333333333f;

		backCamLimit->SetPos(App->render->camera.x * 0.3333333333f, 0);
		frontCamLimit->SetPos(aux, 0);
	}

	//backCamLimit->SetPos(0 + App->render->cameraSpeed, 0);
	//frontCamLimit->SetPos(SCREEN_WIDTH + App->render->cameraSpeed - 20, 0);

	App->enemies->Update();

	return update_status::UPDATE_CONTINUE;

}

// Update: draw background
update_status ModuleScene::PostUpdate()
{
	// Draw everything --------------------------------------
	App->render->Blit(sky, 0, 0, &SkyLayer, 0.4f);
	App->render->Blit(trees, 0, 58, &TreeLayer, 0.5f);
	App->render->Blit(stone, 0, 90, &StoneWall, 0.65f);
	App->render->Blit(layer2, 0, 5, &background, 0.75f); 



	

	return update_status::UPDATE_CONTINUE;
}