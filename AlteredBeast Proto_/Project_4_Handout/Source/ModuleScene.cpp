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
#include "ModuleGreyScene.h"
#include "ModulePlayer.h"
#include "ModuleBoss.h"


ModuleScene::ModuleScene(bool startEnabled) : Module(startEnabled)
{

	startEnabled = true;
	//Background
	background.x = 0;
	background.y = 17;
	background.w = 4662;
	background.h = 273;


	//Stone wal
	StoneWall.x = 0;
	StoneWall.y = 75;
	StoneWall.w = 5484;
	StoneWall.h = 93;

	// Tree layer
	TreeLayer.x = 0;
	TreeLayer.y = 53;
	TreeLayer.w = 2977;
	TreeLayer.h = 75;

	//sky
	SkyLayer.x = 0;
	SkyLayer.y = 0;
	SkyLayer.w = 3016;
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
	DDeath.PushBack({ 547,727 , 73,90 });
	DDeath.PushBack({ 621,727 , 73,90 });
	DDeath.PushBack({ 695,727 , 73,90 });
	DDeath.loop = true;
	DDeath.speed = 0.12f;




}


// Load assets
bool ModuleScene::Start()
{
	LOG("Loading background assets");

	App->audio->PlayMusic("Assets/Music/rise-from-your-grave.ogg", 0.0f);
	App->grey_scene->Grey = false;
	bool ret = true;
	

	layer2 = App->textures->Load("Assets/Layer2.png");
	stone = App->textures->Load("Assets/stone.png");
	trees = App->textures->Load("Assets/trees.png");
	sky = App->textures->Load("Assets/sky.png");
	EnemyTexture = App->textures->Load("Assets/EnemiesProto.png");
	ExplosionText = App->textures->Load("Assets/Particle.png");

	App->player->tranSt = Transform::DEFAULT;

	
	
	
	ModuleScene::PlaceEnemies();
	
	
	
	//ModuleScene::TestEnemies();

	backCamLimit = App->collisions->AddCollider({ (int)App->render->camera.x, (int)App->render->camera.y, 10, SCREEN_HEIGHT }, Collider::Type::CAMLIMIT, (Module*)App->player);
	frontCamLimit = App->collisions->AddCollider({ (int)App->render->camera.x + SCREEN_WIDTH-10, (int)App->render->camera.y + SCREEN_WIDTH - 10, 10, SCREEN_HEIGHT }, Collider::Type::CAMLIMIT, (Module*)App->player);

	App->player->Enable();
	App->enemies->Enable();
	App->scene2->Enable();
	App->grey_scene->Enable();
	App->collisions->Enable();
	App->audio->Enable();
	
	//App->render->camera.dynamicSpeed = 0.5;


	//App->powers->Enable();
	ScreenScroll = true;

	return ret;
}

update_status ModuleScene::Update()
{
	
	//This is so camera stops when reaching boss
	//if (App->render->camera.x > 1161) { ScreenScroll = false; }
	//if (ScreenScroll == false) {
	//	App->render->camera.x = 1161;
	//	//WHY does it slightly move backwards???
	//}
	//SCREEN SCROLL

 	if (ScreenScroll == true) {
		//App->render->camera.x += App->render->camera.dynamicSpeed;
		App->render->camera.x += 0.3;
		//aux = (App->render->camera.x + (SCREEN_WIDTH - 10) * SCREEN_SIZE );

		aux = (App->render->camera.x + (SCREEN_WIDTH - 10));

		backCamLimit->SetPos(App->render->camera.x, 0);
		frontCamLimit->SetPos(aux, 0);
	}

	

	App->enemies->Update();
	if (App->player->tranSt == Transform::DEFAULT || App->player->tranSt == Transform::POWER1 || App->player->tranSt == Transform::POWER2 || App->player->tranSt == Transform::WOLF) {
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

	if (App->input->keys[SDL_SCANCODE_P] == KEY_DOWN) {
		if (App->powers->IsEnabled() == false) {
			App->powers->Enable();
			App->powers->position.x = App->player->position.x+129;
			App->powers->spawnPos.x = App->player->position.x + 129;
			App->powers->position.y = 150;
		}
		else {
			App->powers->Disable();
		}
	}

	if (EnemyCoolDown != 30) {
		EnemyCoolDown++;
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
		
		waveRatio += waveRatioSpeed;

		enemyX2 = enemyX + (waveHeight * sinf(waveRatio));

		enemyY -= 1.4;

		App->render->Blit(EnemyTexture, enemyX2, enemyY, &DeathFrame);
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
	if (App->bossfight->Neff_activ == false) {
		if (EnemyCoolDown == 30 && (App->input->keys[SDL_SCANCODE_5] == KEY_DOWN || App->input->pads[0].up)) {

			App->enemies->AddEnemy(ENEMY_TYPE::DRAGON, App->player->position.x + 150, 0, true);
			EnemyCoolDown = 0;
		}
		else if (EnemyCoolDown == 30 && (App->input->keys[SDL_SCANCODE_4] == KEY_DOWN || App->input->pads[0].down)) {

			App->enemies->AddEnemy(ENEMY_TYPE::SKULL, App->player->position.x + 150, 170, true);
			EnemyCoolDown = 0;
		}
		else if (EnemyCoolDown == 30 && (App->input->keys[SDL_SCANCODE_3] == KEY_DOWN || App->input->pads[0].right)) {
			App->enemies->AddEnemy(ENEMY_TYPE::BROWNWOLF, App->player->position.x + 150, 200, true);
			EnemyCoolDown = 0;
		}
		else if (EnemyCoolDown == 30 && (App->input->keys[SDL_SCANCODE_1] == KEY_DOWN || App->input->pads[0].left)) {
			App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, App->player->position.x + 150, 200, true);
			EnemyCoolDown = 0;
		}
		else if (EnemyCoolDown == 30 && (App->input->keys[SDL_SCANCODE_2] == KEY_DOWN))
		{
			App->enemies->AddEnemy(ENEMY_TYPE::WHITEWOLF, App->player->position.x + 150, 200, true);
			EnemyCoolDown = 0;
		}
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

	//App->audio->Disable();

	return true;
}


void ModuleScene::PlaceEnemies() {
	//----------------First Part ( 200 - 700 )

	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 260, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 280, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 180, 200, false);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 310, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 320, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 340, 200, true);

	// Graves

	App->enemies->AddGrave(340, 120, true, false, true);
	App->enemies->AddGrave(380, 120, false, false, false);
	App->enemies->AddGrave(420, 120, false, false, false);
	App->enemies->AddGrave(460, 120, false, false, false);
	App->enemies->AddGrave(500, 120, false, false, false);
	App->enemies->AddGrave(540, 120, false, false, false);
	App->enemies->AddGrave(580, 120, false, false, true);
	App->enemies->AddGrave(620, 120, false, true, true);

	App->enemies->AddEnemy(ENEMY_TYPE::WHITEWOLF, 380, 200, false);
	App->enemies->AddEnemy(ENEMY_TYPE::WHITEWOLF, 400, 200, false);
	App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 430, 165, false);
	
	//-------------------Second Part ( 700 - 900  )

	App->enemies->AddEnemy(ENEMY_TYPE::BROWNWOLF, 700, 200, false);

	// Graves

	App->enemies->AddGrave(710, 120, true, false, true);
	App->enemies->AddGrave(750, 120, false, true, true);

	App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 790, 165, false);
	App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 800, 165, false);
	App->enemies->AddEnemy(ENEMY_TYPE::WHITEWOLF, 820, 200, false);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 850, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 860, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 860, 165, false);
	App->enemies->AddEnemy(ENEMY_TYPE::WHITEWOLF, 870, 200, false);

	//-------------------Last Part ( 900 - 1100 )

	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 950, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 990, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 970, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 1000, 165, false);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 1020, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 1010, 200, false);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 1030, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 1020, 200, false);
	App->enemies->AddEnemy(ENEMY_TYPE::DRAGON, 1090, 0, true);
	App->enemies->AddEnemy(ENEMY_TYPE::DRAGON, 1092, 0, true);

	//---------------------------------------------------------------------First Neff 

	App->enemies->AddNeff(1100, 100, false);

	//----------------First Part ( 1300 - 1800 )

	App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 1300, 165, false);
	App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 1360, 165, false);

	App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 1500, 165, false);

	// Graves

	App->enemies->AddGrave(1350, 120, true, false, true);
	App->enemies->AddGrave(1390, 120, false, false, false);
	App->enemies->AddGrave(1430, 120, false, false, false);
	App->enemies->AddGrave(1470, 120, false, false, false);
	App->enemies->AddGrave(1510, 120, false, false, false);
	App->enemies->AddGrave(1550, 120, false, true, true);

	App->enemies->AddEnemy(ENEMY_TYPE::WHITEWOLF, 1600, 200, false);

	App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 1760, 165, false);
	App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 1800, 165, false);

	//-----------------Second Part ( 1800 - 2400 )
	
	// Graves

	App->enemies->AddGrave(1900, 120, true, false, false);
	App->enemies->AddGrave(1940, 120, false, true, false);

	App->enemies->AddEnemy(ENEMY_TYPE::WHITEWOLF, 1960, 200, false);

	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 2100, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 2130, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 2160, 200, true);

	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 2090, 200, false);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 2110, 200, false);

	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 2200, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 2230, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 2260, 200, true);

	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 2200, 200, false);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 2210, 200, false);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 2230, 200, false);

	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 2300, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 2330, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 2360, 200, true);

	//-----------------Last Part ( 2400 - 2700 )

	App->enemies->AddEnemy(ENEMY_TYPE::DRAGON, 2420, 0, true);

	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 2500, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 2530, 200, true);

	App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 2600, 165, false);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 2650, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 2680, 200, true);

	App->enemies->AddEnemy(ENEMY_TYPE::DRAGON, 2720, 0, true);

	//----------------------------------------------------------------------Second Neff 

	App->enemies->AddNeff(2700, 100, false);

	//---------------First Part ( 2900 - 3400 )
	
	//Graves

	App->enemies->AddGrave(2800, 120, true, false, false);
	App->enemies->AddGrave(2840, 120, false, false, false);
	App->enemies->AddGrave(2880, 120, false, false, false);
	App->enemies->AddGrave(2920, 120, false, true, true);

	App->enemies->AddEnemy(ENEMY_TYPE::BROWNWOLF, 2910, 200, false);
	App->enemies->AddEnemy(ENEMY_TYPE::BROWNWOLF, 2970, 200, false);
	App->enemies->AddEnemy(ENEMY_TYPE::WHITEWOLF, 3020, 200, false);

	App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 3100, 165, false);
	App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 3150, 165, false);
	App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 3200, 165, false);

	//--------------Second Part ( 3400 - 4200 )

	App->enemies->AddEnemy(ENEMY_TYPE::BROWNWOLF, 3300, 200, false);

	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 3400, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 3450, 200, true);

	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 3600, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 3650, 200, true);

	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 3800, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 3850, 200, true);

	App->enemies->AddEnemy(ENEMY_TYPE::WHITEWOLF, 3900, 200, false);

	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 4100, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 4150, 200, true);

	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 4300, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 4350, 200, true);

	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 4500, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 4550, 200, true);

	App->enemies->AddEnemy(ENEMY_TYPE::WHITEWOLF, 4650, 200, false);
	App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 4700, 165, false);
	App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 4750, 165, false);

	//--------------Third Part ( 4900 - 6000 )

	App->enemies->AddEnemy(ENEMY_TYPE::BROWNWOLF, 4900, 200, false);
	App->enemies->AddEnemy(ENEMY_TYPE::BROWNWOLF, 4950, 200, false);
	App->enemies->AddEnemy(ENEMY_TYPE::WHITEWOLF, 5000, 200, false);

	//Graves

	App->enemies->AddGrave(5100, 120, true, false, true);
	App->enemies->AddGrave(5120, 120, false, false, true);
	App->enemies->AddGrave(5140, 120, false, true, true);

	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 5250, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 5300, 200, true);

	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 5500, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 5530, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 5560, 200, true);

	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 5610, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 5630, 200, true);
	/*App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 5700, 165, false);
	App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 5750, 165, false);
	App->enemies->AddEnemy(ENEMY_TYPE::DRAGON, 5800, 0, true);
	App->enemies->AddEnemy(ENEMY_TYPE::DRAGON, 5820, 0, true);*/


	//----------------------------------------------------------------------Last Neff 

	App->enemies->AddNeff(5500, 100, true);

	// Original
	
	//App->enemies->AddGrave(2900, 120, true, false, false);
	//App->enemies->AddGrave(2940, 120, false, false, false);
	//App->enemies->AddGrave(2980, 120, false, false, false);
	//App->enemies->AddGrave(3020, 120, false, true, true);

	//App->enemies->AddEnemy(ENEMY_TYPE::BROWNWOLF, 3010, 200, false);
	//App->enemies->AddEnemy(ENEMY_TYPE::BROWNWOLF, 3070, 200, false);
	//App->enemies->AddEnemy(ENEMY_TYPE::WHITEWOLF, 3120, 200, false);

	//App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 3200, 165, false);
	//App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 3250, 165, false);
	//App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 3300, 165, false);

	////--------------Second Part ( 3400 - 4200 )

	//App->enemies->AddEnemy(ENEMY_TYPE::BROWNWOLF, 3400, 200, false);

	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 3500, 200, true);
	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 3550, 200, true);

	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 3700, 200, true);
	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 3750, 200, true);

	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 3900, 200, true);
	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 3950, 200, true);

	//App->enemies->AddEnemy(ENEMY_TYPE::WHITEWOLF, 4000, 200, false);

	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 4200, 200, true);
	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 4250, 200, true);
	//										   
	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 4400, 200, true);
	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 4450, 200, true);
	//										   
	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 4600, 200, true);
	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 4650, 200, true);

	//App->enemies->AddEnemy(ENEMY_TYPE::WHITEWOLF, 4750, 200, false);
	//App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 4800, 165, false);
	//App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 4850, 165, false);

	////--------------Third Part ( 4900 - 6000 )

	//App->enemies->AddEnemy(ENEMY_TYPE::BROWNWOLF, 5000, 200, false);
	//App->enemies->AddEnemy(ENEMY_TYPE::BROWNWOLF, 5050, 200, false);
	//App->enemies->AddEnemy(ENEMY_TYPE::WHITEWOLF, 5100, 200, false);

	////Graves
	//
	//App->enemies->AddGrave(5200, 120, true, false, true);
	//App->enemies->AddGrave(5220, 120, false, false, true);
	//App->enemies->AddGrave(5240, 120, false, true, true);

	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 5350, 200, true);
	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 5400, 200, true);

	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 5600, 200, true);
	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 5630, 200, true);
	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 5660, 200, true);

	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 5710, 200, true);
	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 5730, 200, true);
	//App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 5800, 165, false);
	//App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 5850, 165, false);
	//App->enemies->AddEnemy(ENEMY_TYPE::DRAGON, 5900, 0, true);
	//App->enemies->AddEnemy(ENEMY_TYPE::DRAGON, 5920, 0, true);


	////----------------------------------------------------------------------Last Neff 

	//App->enemies->AddNeff(5875, 100, true);



	//// Behind Player
	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 800, 200, false);
	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 810, 200, false);
	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 1600, 200, false);
	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 1720, 200, false);
	//// In front of player
	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 260, 200, true);
	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 320, 200, true);
	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 400, 200, true);

	//App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 700, 165, true);
	//App->enemies->AddEnemy(ENEMY_TYPE::BROWNWOLF, 750, 200, true);
	//App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 800, 165, true);
	//App->enemies->AddEnemy(ENEMY_TYPE::WHITEWOLF, 850, 200, true);

	//App->enemies->AddNeff(875, 100, false);
	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 1100, 200, true);
	//App->enemies->AddEnemy(ENEMY_TYPE::WHITEWOLF, 1150, 150, true);

	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 1400, 200, false);
	//App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 1450, 165, false);

	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 1600, 200, true);
	//App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 1650, 165, false);
	//App->enemies->AddEnemy(ENEMY_TYPE::SKULL, 1670, 165, false);
	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 1690, 200, true);
	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 1720, 200, true);
	//App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 1740, 200, true);

	//App->enemies->AddNeff(1900, 100, true);

	//// Graves
	//App->enemies->AddGrave(400, 120, true, false, true);
	//App->enemies->AddGrave(440, 120, false, false, true);
	//App->enemies->AddGrave(480, 120, false, false, true);
	//App->enemies->AddGrave(520, 120, false, true, true);

	//App->enemies->AddGrave(1200, 120, true, false, true);
	//App->enemies->AddGrave(1240, 120, false, false, false);
	//App->enemies->AddGrave(1280, 120, false, true, true);
	//
	//// Should stand still
	//
	//App->enemies->AddEnemy(ENEMY_TYPE::WHITEWOLF, 550, 200, false);
	//App->enemies->AddEnemy(ENEMY_TYPE::DRAGON, 900, 50, true);
	//App->enemies->AddEnemy(ENEMY_TYPE::DRAGON, 1480, 50, true);
	//App->enemies->AddEnemy(ENEMY_TYPE::DRAGON, 1520, 50, true);




}

void ModuleScene::TestEnemies() {

	// Behind Player
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 128, 200, false);

	// In front of player
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 260, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 303, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 246, 200, true);
	App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, 389, 200, true);

	App->enemies->AddGrave(150, 130, true, false, false);
	App->enemies->AddGrave(185, 130, false,false ,true);
	App->enemies->AddGrave( 220, 130, false,false,true);
	App->enemies->AddGrave( 255, 130, false, true,true);


	// Should stand still
	App->enemies->AddEnemy(ENEMY_TYPE::WHITEWOLF, 395, 200, false);





}