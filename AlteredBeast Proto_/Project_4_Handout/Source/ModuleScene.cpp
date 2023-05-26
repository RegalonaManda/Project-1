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
#include "ModulePlayer.h"


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

	//------Dragon_Death-------//

	DDeath.PushBack({ 325,727 , 73,90 });
	DDeath.PushBack({ 399,727 , 73,90 });
	DDeath.PushBack({ 473,727 , 73,90 });
	DDeath.loop = true;
	DDeath.speed = 0.08f;




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
	ExplosionText = App->textures->Load("Assets/Particle.png");



	App->enemies->AddEnemy(ENEMY_TYPE::DRAGON, 200, 0, false);

	// border L ,  border R
	//App->enemies->AddGrave( 150, 130, true, false,false);
	//App->enemies->AddGrave(185, 130, false,false ,true);
	//App->enemies->AddGrave( 220, 130, false,false,false);
	//App->enemies->AddGrave( 255, 130, false, true,false);
	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 250, 200, true);
	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 400, 200, false);
	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 450, 200, true);
	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 500, 200,false);
	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 350, 200, true);
	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 525, 200,false);
	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 550, 200, true);
	//App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 300, 120, true);
	


	//App->enemies->AddGrave( 380, 130, true, false, true);
	//App->enemies->AddGrave( 415, 130, false,false, false);
	//App->enemies->AddGrave( 450, 130, false,false, false);
	//App->enemies->AddGrave( 485, 130, false,true , false);




	/*App->enemies->AddEnemy(ENEMY_TYPE::BROWNWOLF, 600, 140, true);*/
	/*App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 650, 200, false);*/
	/*App->enemies->AddEnemy(ENEMY_TYPE::WHITEWOLF, 700, 140,false);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 705, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 740, 200,false);
	App->enemies->AddEnemy(ENEMY_TYPE::WHITEWOLF, 800, 140, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 900, 200,false);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 905, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::BROWNWOLF, 910, 140, false);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 1000, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::BROWNWOLF, 1100, 140,false);
	App->enemies->AddEnemy(ENEMY_TYPE::WHITEWOLF, 1105, 140, true);*/
	//App->enemies->AddEnemy(ENEMY_TYPE::NEFF, 1161 + 250, 110,false);
	
	
	
	//App->enemies->AddEnemy(ENEMY_TYPE::NEFF, 200, 110, false);

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
	if (App->render->camera.x > 1161) { ScreenScroll = false; }
	if (ScreenScroll == false) {
		App->render->camera.x = 1161;
		//WHY does it slightly move backwards???
	}
	//SCREEN SCROLL
	if (ScreenScroll == true) {
		App->render->camera.x += 1;

		aux = (App->render->camera.x + (SCREEN_WIDTH - 10) * SCREEN_SIZE );

		backCamLimit->SetPos(App->render->camera.x, 0);
		frontCamLimit->SetPos(aux, 0);
	}

	

	App->enemies->Update();
	if (App->player->tranSt == Transform::DEFAULT || App->player->tranSt == Transform::POWER1 || App->player->tranSt == Transform::WOLF) {
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
		else if (EnemyCN == 7) {
			
			if (HasEnemyDied) {
				Dcurrent = &DDeath;
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
	if (HasEnemyDied && EnemyCN == 7) {
		DeathFrame = Dcurrent->GetCurrentFrame();
		
		App->render->Blit(EnemyTexture, enemyX, enemyY--, &DeathFrame);
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

	if (EnemyCN == 7 && enemyY < -100){
		HasEnemyDied = false;
		DDeath.loopCount = 0;  
	}


	if (EnemyAttacking == true) {
		explosionCnt--;
	}

	if (EnemyAttacking == true && explosionCnt <= 0 && EnemyCN == 1) {
		SDL_Rect explosion = Xcurrent->GetCurrentFrame();
		App->render->Blit(ExplosionText, enemyX - 25, enemyY, &explosion);
	}
	if (explode.HasFinished() == true && explosionCnt <= 0 && EnemyCN == 1) {
		explode.Reset();
		explode.loopCount--;
		EnemyAttacking = false;
		explosionCnt = 40;
	}
	if (App->input->keys[SDL_SCANCODE_ESCAPE] == KEY_DOWN || App->input->pads[0].guide) {

		return update_status::UPDATE_STOP;


	}
	if (App->input->keys[SDL_SCANCODE_1] == KEY_DOWN || App->input->pads[0].up) {

		App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, App->player->position.x + 150, 200, true);

	}
	else if (App->input->keys[SDL_SCANCODE_2] == KEY_DOWN || App->input->pads[0].down) {

		App->enemies->AddEnemy(ENEMY_TYPE::SKULL, App->player->position.x + 150, 200, true);

	}
	else if (App->input->keys[SDL_SCANCODE_3] == KEY_DOWN || App->input->pads[0].right) {
		App->enemies->AddEnemy(ENEMY_TYPE::BROWNWOLF, App->player->position.x + 150, 200, true);

	}
	else if (App->input->keys[SDL_SCANCODE_4] == KEY_DOWN || App->input->pads[0].left) {
		App->enemies->AddEnemy(ENEMY_TYPE::WHITEWOLF, App->player->position.x + 150, 200, true);

	}
	else if (App->input->keys[SDL_SCANCODE_5] == KEY_DOWN)
	{
		App->enemies->AddEnemy(ENEMY_TYPE::DRAGON, App->player->position.x + 150, 200, true);
	}

	// including more enemies could only be done through keyboard input only
	

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