#include "ModuleScene.h"
#include "ModuleEnemies.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCollisions.h"
#include "ModuleAudio.h"
#include "Enemy.h"
#include "Zombie.h"
#include "EnemyDeath.h"


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

	deathAnim.PushBack({ 127,1,41,68 });
	deathAnim.PushBack({ 132,1,41,68 });
	deathAnim.PushBack({ 127,1,41,68 });
	deathAnim.PushBack({ 132,1,41,68 });
	deathAnim.PushBack({ 351,547,1,1 });
	deathAnim.loop = false;
	deathAnim.totalFrames = 4;
	deathAnim.speed = 0.1f;

	//Zombie explosion
	explode.PushBack({ 1,1,93,93 });
	explode.PushBack({ 95,1,93,93 });
	explode.PushBack({ 189,1,93,93 });
	explode.PushBack({ 1,1,93,93 });
	explode.PushBack({ 95,1,93,93 });
	explode.PushBack({ 189,1,93,93 });
	explode.PushBack({ 1,1,93,93 });
	explode.PushBack({ 95,1,93,93 });
	explode.PushBack({ 189,1,93,93 });
	explode.loop = false;
	explode.totalFrames = 9;
	explode.speed = 0.2f;

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
	EnemyTexture = App->textures->Load("Assets/Enemies Proto.png");
	ExplosionText = App->textures->Load("Assets/ParticleGrid.png");

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

	

	App->enemies->Update();

	//for (int i = 0; i < MAX_ENEMIES; i++) {
	//	if (App->enemies->enemies[i] != nullptr) {

	//		
	//		if (App->enemies->enemies[i]->hp <= 0 && App->enemies->enemies[i]->destroyed == false) {
	//			//App->enemies->enemies[i]->Death(App->enemies->enemies[i]->position.x)
	//			Death->KillZombie(App->enemies->enemies[i]->position.x, App->enemies->enemies[i]->position.y);
	//		}

	//	}
	//}

	if (HasEnemyDied == true) {
		//Death->KillZombie(enemyX, enemyY);
		Ecurrent = &deathAnim;
		Ecurrent->Update();
	}

	if (EnemyAttacking == true) {
		//Kai Kaboom sound pls
		Xcurrent = &explode;
		Xcurrent->Update();
	}


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

	

	if (HasEnemyDied == true) {
		SDL_Rect DeathFrame = Ecurrent->GetCurrentFrame();
		App->render->Blit(EnemyTexture, enemyX, enemyY, &DeathFrame);
	}
	if (deathAnim.HasFinished() == true) {
		HasEnemyDied = false;
	}

	if (EnemyAttacking == true) {
		explosionCnt--;
	}

	if (EnemyAttacking == true && explosionCnt <= 0) {
		SDL_Rect explosion = Xcurrent->GetCurrentFrame();
		App->render->Blit(ExplosionText, enemyX-25, enemyY, &explosion);
	}
	if (explode.HasFinished() == true) {
		EnemyAttacking = false;
	}

	return update_status::UPDATE_CONTINUE;
}