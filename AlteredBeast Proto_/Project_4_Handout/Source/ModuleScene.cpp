#include "ModuleScene.h"
#include "ModuleEnemies.h"
#include "ModulePower.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCollisions.h"
#include "ModuleAudio.h"
#include "Enemy.h"
#include "Zombie.h"
#include "EnemyDeath.h"
#include "WhiteWolf.h"
#include "BrownWolf.h"


ModuleScene::ModuleScene(bool startEnabled) : Module(startEnabled)
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
	explode.speed = 0.18f;

	WDeathRight.PushBack({ 1,121,33,50 });
	WDeathRight.PushBack({ 34,121,33,50 });
	WDeathRight.speed = 0.08f;
	WDeathRight.loop = false;
	WDeathRight.totalFrames = 2;

	WDeathLeft.PushBack({ 446,121,33,50 });
	WDeathLeft.PushBack({ 413,121,33,50 });
	WDeathLeft.speed = 0.08f;
	WDeathLeft.loop = false;
	WDeathLeft.totalFrames = 2;

	BDeathRight.PushBack({135,121,33,50});
	BDeathRight.PushBack({ 169,121,33,50 });
	BDeathRight.speed = 0.08f;
	BDeathRight.loop = false;
	BDeathRight.totalFrames = 2;

}


// Load assets
bool ModuleScene::Start()
{
	LOG("Loading background assets");



	App->audio->PlayMusic("Assets/Music/rise-from-your-grave.ogg", 0.0f);

	bool ret = true;

	layer2 = App->textures->Load("Assets/Layer2.png");
	stone = App->textures->Load("Assets/stone.png");
	trees = App->textures->Load("Assets/trees.png");
	sky = App->textures->Load("Assets/sky.png");
	EnemyTexture = App->textures->Load("Assets/EnemiesProto.png");
	ExplosionText = App->textures->Load("Assets/ParticleGrid.png");

	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 300, 120);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 400, 120);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 450, 120);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 500, 120);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 350, 120);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 525, 120);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 550, 120);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 575, 120);
	App->enemies->AddEnemy(ENEMY_TYPE::BROWNWOLF, 600, 140);
	App->enemies->AddEnemy(ENEMY_TYPE::WHITEWOLF, 700, 140);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 705, 120);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 740, 120);
	App->enemies->AddEnemy(ENEMY_TYPE::WHITEWOLF, 800, 140);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 900, 120);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 905, 120);
	App->enemies->AddEnemy(ENEMY_TYPE::BROWNWOLF, 910, 140);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 1000, 120);
	App->enemies->AddEnemy(ENEMY_TYPE::BROWNWOLF, 1100, 140);
	App->enemies->AddEnemy(ENEMY_TYPE::WHITEWOLF, 1105, 140);
	App->enemies->AddEnemy(ENEMY_TYPE::NEFF, 1161 + 250, 110);
	
	
	

	backCamLimit = App->collisions->AddCollider({ App->render->camera.x, App->render->camera.y, 10, SCREEN_HEIGHT }, Collider::Type::CAMLIMIT, (Module*)App->player);
	frontCamLimit = App->collisions->AddCollider({ App->render->camera.x + SCREEN_WIDTH-10, App->render->camera.y + SCREEN_WIDTH - 10, 10, SCREEN_HEIGHT }, Collider::Type::CAMLIMIT, (Module*)App->player);

	App->player->Enable();
	App->enemies->Enable();
	App->scene2->Enable();
	App->collisions->Enable();
	App->audio->Enable();
	

	//App->powers->Enable();
	ScreenScroll = true;

	return ret;
}

update_status ModuleScene::Update()
{
	
	//This is so camera stops when reaching boss
	if (App->render->camera.x > 1161*3) { ScreenScroll = false; }
	if (ScreenScroll == false) {
		App->render->camera.x = 1161*3;
		//WHY does it slightly move backwards???
	}
	//SCREEN SCROLL
	if (ScreenScroll == true) {
		App->render->camera.x += 1;

		aux = (App->render->camera.x + (SCREEN_WIDTH - 10) * SCREEN_SIZE ) * 0.3333333333f;

		backCamLimit->SetPos(App->render->camera.x * 0.3333333333f, 0);
		frontCamLimit->SetPos(aux, 0);
	}

	

	App->enemies->Update();
	if (App->player->tranSt == ModulePlayer::Transform::DEFAULT || App->player->tranSt == ModulePlayer::Transform::POWER1) {
		if (EnemyCN == 1) {
			if (HasEnemyDied == true && EnemyAttacking == false) {
				//Death->KillZombie(enemyX, enemyY);
				Ecurrent = &deathAnim;
				Ecurrent->Update();
			}

			if (EnemyAttacking == true) {
				//Kai Kaboom sound pls
				Xcurrent = &explode;
				Xcurrent->Update();
			}
		}
		else if (EnemyCN == 2) {
			if (HasEnemyDied && (bool)WhiteWolf::Direction::RIGHT) {
				Wcurrent = &WDeathRight;
				Wcurrent->Update();
			}
			else if (HasEnemyDied && (bool)WhiteWolf::Direction::LEFT) {
				Wcurrent = &WDeathLeft;
				Wcurrent->Update();
			}
		}
		else if (EnemyCN == 3) {
			if (HasEnemyDied && (bool)BrownWolf::Direction::RIGHT) {
				Bcurrent = &BDeathRight;
				Bcurrent->Update();
			}
			else if (HasEnemyDied && (bool)BrownWolf::Direction::LEFT) {
				Bcurrent = &BDeathLeft;
				Bcurrent->Update();
			}
		}
		else if (EnemyCN == 4) {
			if (HasEnemyDied) {
				Dcurrent = &explode;
				Dcurrent->Update();
			}
		}
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



	SDL_Rect DeathFrame;
	if (HasEnemyDied == true && EnemyAttacking == false && EnemyCN == 1) {
		DeathFrame = Ecurrent->GetCurrentFrame();
		App->render->Blit(EnemyTexture, enemyX, enemyY, &DeathFrame);
	}
	if (deathAnim.HasFinished() == true) {
		HasEnemyDied = false;
		deathAnim.loopCount--;
	}
	if (HasEnemyDied && EnemyCN == 2) {
		DeathFrame = Wcurrent->GetCurrentFrame();
		App->render->Blit(EnemyTexture, enemyX, enemyY, &DeathFrame);
	}
	if (HasEnemyDied && EnemyCN == 3) {
		DeathFrame = Bcurrent->GetCurrentFrame();
		App->render->Blit(EnemyTexture, enemyX, enemyY, &DeathFrame);
	}
	if (HasEnemyDied && EnemyCN == 4) {
		DeathFrame = Dcurrent->GetCurrentFrame();
		App->render->Blit(ExplosionText, enemyX, enemyY, &explosion);
	}

	if (WDeathRight.HasFinished()) {
		HasEnemyDied = false;
		WDeathRight.loopCount--;
	}

	if (WDeathLeft.HasFinished()) {
		HasEnemyDied = false;
		WDeathLeft.loopCount--;
	}

	if (BDeathRight.HasFinished()) {
		HasEnemyDied = false;
		BDeathRight.loopCount--;
	}

	if (BDeathLeft.HasFinished()) {
		HasEnemyDied = false;
		BDeathLeft.loopCount--;
	}

	if (EnemyAttacking == true) {
		explosionCnt--;
	}

	if (EnemyAttacking == true && explosionCnt <= 0 && EnemyCN == 1) {
		SDL_Rect explosion = Xcurrent->GetCurrentFrame();
		App->render->Blit(ExplosionText, enemyX-25, enemyY, &explosion);
	}
	if (explode.HasFinished() == true && explosionCnt <= 0 && EnemyCN == 1) {
		explode.Reset();
		explode.loopCount--;
		EnemyAttacking = false;
		explosionCnt = 40;
	}
	if (App->input->keys[SDL_SCANCODE_ESCAPE] == KEY_DOWN) {

		return update_status::UPDATE_STOP;
		

	}


	

	return update_status::UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp() {

	App->textures->Unload(layer2);
	App->textures->Unload(stone);
	App->textures->Unload(trees);
	App->textures->Unload(sky);
	App->textures->Unload(EnemyTexture);
	App->textures->Unload(ExplosionText);

	App->player->Disable();
	App->enemies->Disable();
	App->scene2->Disable();
	App->powers->Disable();
	App->collisions->Disable();
	/*App->audio->Disable();*/

	return true;
}