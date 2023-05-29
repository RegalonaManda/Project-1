#include "ModulePlayer.h"
#include "Power1.h"
#include "Power2.h"
#include "WereWolf.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCollisions.h"
#include "ModuleFonts.h"
#include "ModuleAudio.h"
#include "ModuleScene.h"
#include "Colliders.h"
#include "ModuleScene2.h"
#include "ModuleEnemies.h"
#include "Enemy.h"
#include "ModulePower.h"
#include "ModuleFadeToBlack.h"
#include <stdio.h>
#include "SceneIntro.h"


#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib")
#pragma comment( lib, "SDL/libx86/SDL2main.lib")

#include "SDL/include/SDL_scancode.h"
//the lower the higher
//#define MAX_HEIGHT 147
//------- Animation Speeds ----------- //
#define WALKANIMSPEED 0.08f
#define PUNCHANIMSPEED 0.13f
#define CROUCHANIMSPEED 0.05f
#define CROUCHPUNCHSPEED 0.1f
#define KICKANIMSPEED 0.13f
#define CROUCHKICKSPEED 0.1f
#define DEATHANIMSPEED 0.05f
#define HITANIMSPEED 0.05f
#define JUMPANIMSPEED 0.025f
#define AIRKICKSPEED 0.06f





Uint32 startTime = 0;

int knockPos;



ModulePlayer::ModulePlayer(bool startEnabled) : Module(startEnabled)
{
	startEnabled = true;
	
	//Default direction
	dir = Direction::RIGHT;
	//Default airstate
	airSt = AirState::GROUND;
	//Default transformation
	tranSt = Transform::POWER1;
	//default attack 
	attack = 1;
	
}

WolfShot FireBall;
ModulePlayer::~ModulePlayer()
{

}

bool ModulePlayer::Start()
{
	LOG("Loading player textures");

	bool ret = true;

	destroyed = false;
	
	texture = App->textures->Load("Assets/ABfullspritesProto.png"); // arcade version

	//Initialize collider
	Pcollider = App->collisions->AddCollider({ 100,300,20,64 }, Collider::Type::PLAYER, this);
	Crouchcollider = App->collisions->AddCollider({ 100,300,29,34 }, Collider::Type::PLAYER, this);
	//CHANGE listener of attack to enemy
	attackCollider = App->collisions->AddCollider({ 100,300,33,19 }, Collider::Type::PLAYER_SHOT, this);
	kickCollider = App->collisions->AddCollider({ 100,300,19,33 }, Collider::Type::PLAYER_SHOT, this);
	WolfKickCollider = App->collisions->AddCollider({ 100,300,66,46 }, Collider::Type::PLAYER_SHOT, this);
	Deathcollider = App->collisions->AddCollider({ 100,300,19,33 }, Collider::Type::ENEMY_SHOT, this);
	Deathcollider->SetPos(1300, 1200);
	//font table
	char lookupTable[] = { "abcdefghijklmnopqrstuvwxyz0123456789.,ç'?!*$%&()+-/<=>© " };
	scoreFont = App->fonts->Load("Assets/font_spritesheet.png", lookupTable, 1);

	//player Sound FX
	currentAnimation = &AllAnimations.idleAnimRight;
	nonLethalAtt = App->audio->LoadFx("Assets/FX/Non-Lethal_Punch.wav");
	lethalAtt = App->audio->LoadFx("Assets/FX/Lethal_Punch.wav");
	loseHP = App->audio->LoadFx("Assets/FX/lose_1_hp.wav");
	powerUp = App->audio->LoadFx("Assets/FX/Power_Up.wav");
	playerDeathFX = App->audio->LoadFx("Assets/FX/Player_Death.wav");

	//Wolf Fireball
	

	FireBall.ShotText = App->textures->Load("Assets/Particle.png");
	FireBall.wolfRec = { 0,0,31,31 };
	FireBall.collider = App->collisions->AddCollider(FireBall.wolfRec, Collider::Type::PLAYER_SHOT, this);
	FireBall.ShotPosition = position;

	FireBall.despawned.PushBack({ 2,2,1,1 });
	FireBall.despawned.loop = true;

	FireBall.GrowAnim.PushBack({ 1,115,73,65 });
	FireBall.GrowAnim.PushBack({ 75,115,73,65 });
	FireBall.GrowAnim.PushBack({ 149,115,73,65 });
	FireBall.GrowAnim.PushBack({ 223,115,73,65 });
	FireBall.GrowAnim.speed = 0.1f;
	FireBall.GrowAnim.loop = false;

	FireBall.TravelAnim.PushBack({ 297,115, 73,65 });
	FireBall.TravelAnim.PushBack({ 371,115,73,65 });
	FireBall.TravelAnim.speed = 0.07f;
	FireBall.TravelAnim.loop = true;

	FireBall.ExplodeAnim.PushBack({ 445,115,73,65 });
	FireBall.ExplodeAnim.PushBack({ 519,115,73,65 });
	FireBall.ExplodeAnim.PushBack({ 593,115,73,65 });
	FireBall.ExplodeAnim.speed = 0.14f;
	FireBall.ExplodeAnim.loop = false;

	FireBall.GrowAnimL.PushBack({ 593,201,73,65 });
	FireBall.GrowAnimL.PushBack({ 519,201,73,65 });
	FireBall.GrowAnimL.PushBack({ 445,201,73,65 });
	FireBall.GrowAnimL.PushBack({ 371,201,73,65 });
	FireBall.GrowAnimL.speed = 0.1f;
	FireBall.GrowAnimL.loop = false;

	FireBall.TravelAnimL.PushBack({ 297,201,73,65 });
	FireBall.TravelAnimL.PushBack({ 223,201,73,65 });
	FireBall.TravelAnimL.speed = 0.07f;
	FireBall.TravelAnimL.loop = true;


	FireBall.CurrentShot = &FireBall.despawned;

	lives = 3;
	hp = 4;
	idle = true;
	dir = Direction::RIGHT;
	start = false;
	attack = 1;
	
	
	return ret;
}

update_status ModulePlayer::Update()
{
	// calculate the max h for the current ground lvl
  	if (airSt == AirState::GROUND) {
		MAX_HEIGHT = position.y - 52   ;
	}

	if (App->sceneIntro->IsEnabled() == false && start == false) {
		position.x = 50;
		position.y = 190;
		Deathcollider->SetPos(position.x, position.y);
		start = true;
	}

	//Transforming Animations, middleground between two Power stages
	if (transforming == true && tranSt == Transform::DEFAULT && !destroyed) {
		
		currentAnimation = &AllAnimations.powerUp1;
		
	}
	if (transforming == true && tranSt == Transform::POWER1 && !destroyed) {

		currentAnimation = &AllAnimations.powerUp2;

	}
	if (transforming == true && tranSt == Transform::POWER2 && !destroyed) {

		tranSt = Transform::WOLF;
		transforming = false;

	}
	if (AllAnimations.powerUp1.HasFinished() == true) {
		tranSt = Transform::POWER1;
		
		
		attack += 1;
		idle = true;
		AllAnimations.powerUp1.loopCount = 0;
		transforming = false;
		App->powers->Disable();
	}
	if (AllAnimations.powerUp2.HasFinished() == true) {
		tranSt = Transform::POWER2;


		attack += 1;
		idle = true;
		AllAnimations.powerUp2.loopCount = 0;
		transforming = false;
		App->powers->Disable();
	}
	
	/*if (airSt == AirState::GROUND && Pcollider->Intersects(Pcollider->rect) == false) {
		airSt = AirState::AIRBORN;
	}*/



	//F1, F2, F3, F4 functionalities (keyboard & gamepad)

	if (App->input->keys[SDL_SCANCODE_F1] == KEY_DOWN || App->input->pads[0].l3) {

		if (!GodMode) 
		{ 
			GodMode = true; 
			lives++;
		}
		else {
			GodMode = false;
			lives = 3;
		}

	}

	if (App->input->keys[SDL_SCANCODE_F3] == KEY_DOWN || App->input->pads[0].r1) {

		App->audio->PlayMusic("Assets/Music/Game_Over.ogg", 0.0f);

		if (tranSt == Transform::DEFAULT) {
			Deathcollider->SetPos(position.x + 20, position.y - 20);
		}
		if (tranSt == Transform::POWER1) {
			Deathcollider->SetPos(position.x + 36, position.y - 65);
		}
		if (tranSt == Transform::POWER2) {
			Deathcollider->SetPos(position.x + 36, position.y - 65);
		}
		lives = 0;
		idle = true;
		airSt = AirState::GROUND;
		tranSt = Transform::DEFAULT;
	}
	else if (App->input->keys[SDL_SCANCODE_F4] == KEY_DOWN || App->input->pads[0].l1) {

		App->audio->PlayMusic("Assets/Music/Win.ogg", 0.0f);
		/*App->enemies->Disable();*/
		App->collisions->Disable();

		App->player->KilledBoss = true;
		App->scene2->killedBoss = true;

	}



	ModulePlayer::Gravity_();

	// New jumping function

	if (App->input->keys[SDL_SCANCODE_SPACE] == KEY_DOWN || App->input->pads[0].x || App->input->pads[0].y) {
		if (idle == true && airSt != AirState::AIRBORN && airSt != AirState::LANDING) {
			position.y -= 5;
			airSt = AirState::AIRBORN;
			jumped = true;

		}
	}

	if (airSt == AirState::AIRBORN && jumped == true && transforming == false) {
		position.y -= impulse;
	}
	if (airSt == AirState::AIRBORN && jumped == false) {
		position.y += 3;
	}

	////Put a max height that makes the player fall faster for it to dont look like its floating ( not real but like the game )
	////impulse lesser, the faster it falls
	if (airSt == AirState::AIRBORN && position.y < MAX_HEIGHT) {
		impulse = -2.0;
		position.y = MAX_HEIGHT + 2;
	}

	if (airSt == AirState::CROUCH && (idle && (App->input->keys[SDL_SCANCODE_S] == KEY_UP|| App->input->pads[0].l_y <= 0.0f))) {
 
  		idle = true;
		airSt = AirState::GROUND;
	}

	//---------------Default--Movement---Function------------
 	if (tranSt == Transform::DEFAULT && transforming == false) {
		ModulePlayer::DefaultMovement();
	}

	
	//Power1
	if (tranSt == Transform::POWER1 && transforming == false) {

		ModulePlayer::Power1Movement();

	}
	if (tranSt == Transform::POWER2 && transforming == false) {

		ModulePlayer::Power2Movement();

	}

	//WereWolf
	if (tranSt == Transform::WOLF && transforming == false) {
		ModulePlayer::WereWolfMovement();
	}

	if (wolfPunch == false) {
		FireBall.CurrentShot = &FireBall.despawned;
	}
	FireBall.CurrentShot->Update();
	
		currentAnimation->Update();

	
		if (iFrames == true)
		{
			
			airSt == AirState::AIRBORN;
			knockImpulse -= Gravity;
			position.y -= knockImpulse;

			position.x--;

			iTimer--;
			if (iTimer <= 0) {
				iFrames = false;
				iTimer = 30;
			}
			
		}

		//if (App->input->keys[SDL_SCANCODE_F1] == KEY_DOWN || App->input->pads[0].l3) {
		//	if (!GodMode) { GodMode = true; }
		//	else if (GodMode) {
		//		GodMode = false;
		//		lives = 3;
		//	}
		//
		//}


		//if (App->input->keys[SDL_SCANCODE_F3] == KEY_DOWN || App->input->pads[0].l1) {

		//	App->audio->PlayMusic("Assets/Music/Game_Over.ogg", 0.0f);
		//	
		//	if (tranSt == Transform::DEFAULT) {
		//		Deathcollider->SetPos(position.x+20, position.y-20);
		//	} 
		//	if (tranSt == Transform::POWER1) {
		//		Deathcollider->SetPos(position.x + 36, position.y - 65);
		//	}
		//	lives = 0;
		//	idle = true;
		//	airSt = AirState::GROUND;
		//	tranSt = Transform::DEFAULT;
		//}
		//
		//if (App->input->keys[SDL_SCANCODE_F4] == KEY_DOWN || App->input->pads[0].l1) {

		//	App->audio->PlayMusic("Assets/Music/Win.ogg", 0.0f);
		//	/*App->enemies->Disable();*/
		//	App->collisions->Disable();
		//
		//	App->player->KilledBoss = true;
		//	App->scene2->killedBoss = true;
		//	
		//}


		if (KilledBoss == true) {

			

			if (FadeCnt == 200) { App->player->score += 20000; }
			FadeCnt--;
			
			if (FadeCnt <= 0) {
				KilledBoss = false;
				App->scene2->killedBoss = false;
				App->render->camera.x = 0;
				FadeCnt = 200;
				tranSt = Transform::DEFAULT;
				transforming = false;
				idle = true;
				airSt = AirState::GROUND;
				App->fade->FadeToBlack((Module*)App->scene, (Module*)App->sceneIntro, 60);
				/*this->CleanUp();*/
			}
		}
		if (startExTimer == true) {
			--exTimer;
		}
		return update_status::UPDATE_CONTINUE;
	
}

update_status ModulePlayer::PostUpdate()
{
	

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	App->render->Blit(texture, position.x, position.y - rect.h, &rect);

	SDL_Rect ShotRect = FireBall.CurrentShot->GetCurrentFrame();
	App->render->Blit(FireBall.ShotText, FireBall.ShotPosition.x, FireBall.ShotPosition.y, &ShotRect);

	sprintf_s(scoreText, 10, "%7d", score);

	App->fonts->BlitText(60, 8, scoreFont, scoreText);
	App->fonts->BlitText(230, 8, scoreFont, "50000");

	// Blit the wolf shot if it exists

	return update_status::UPDATE_CONTINUE;
}




void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{
	if (GodMode == false) {

		//--------------------------------------Explosion Collision ------------------------------------
		if (c1 == Pcollider && c2->type == Collider::Type::ATTACK_XplosionTrigger && !destroyed && !iFrames)
		{
			startExTimer = true;

			if (exTimer <= 0) {

				PlayerHit(c2);
				startExTimer = false;
				exTimer = 40;
			}
			

			playerDamaged();
		}

		if (c1 == attackCollider && c2->type == Collider::Type::ENEMY) {
			hitEnemy = true;

			hitEnemy = false;
		}
		//-------------------------------------Bumping into enemy----------------------------
		if (c1 == Pcollider && c2->type == Collider::Type::ENEMY && !destroyed && iFrames == false)
		{

			PlayerBump();
			/*knockImpulse = 1.0f;
			up to change */


			/*App->scene->ScreenScroll = false;*/
		}

		//------------------------------------------------Getting hit by enemy attack-------------------------------------

		//if (c1 == Pcollider && c2->type == Collider::Type::ENEMY_SHOT && !destroyed && iFrames == false)
		//{
		//	PlayerHit(c2);

		//	/*App->scene->ScreenScroll = false;*/
		//}

		if (c1 == Pcollider && c2->type == Collider::Type::ENEMY_SHOT && !destroyed && iFrames == false)
		{
			Deathcollider->SetPos(1300, 1200);
			firstHit = true;

			knockImpulse = 1.0f;
			iFrames = true;
			hp--;


			position.y -= 0.1f;
			if (position.y < 190) {
				//shoudl call a different knockbackfunction
				if (c2 != Deathcollider) {
					//KnockBack(25);
					playerKnocked = true;
					if (hp > 0) { App->audio->PlayFx(loseHP, 6); }
					if (dir == Direction::LEFT) {
						idle = false;
						currentAnimation = &AllAnimations.knockBackLeft;
					}
					if (dir == Direction::RIGHT) {
						currentAnimation = &AllAnimations.knockBackRight;
						idle = false;
					}
				}
			}

			playerDamaged();
		}

		//Getting hit by enemy attack
		if (c1 == Pcollider && c2->type == Collider::Type::ENEMY_SHOT && !destroyed && iFrames == false)
		{
			PlayerHit(c2);

			playerDamaged();

			/*App->scene->ScreenScroll = false;*/
		}

	}
	
	// -------------------------------------------Camera Collisions-------------------------------

	if (c1->type == Collider::Type::PLAYER && c2 == App->scene->backCamLimit) {
		AllAnimations.W_KickR.loopCount = 0;
		AllAnimations.W_KickL.loopCount = 0;
		idle = true;
		//Kai revisa esto cuando puedas, de momento lo he harcodeado

		/*while (position.x < (App->render->camera.x * 0.3333333333f - 13.3333333333f)) {
			position.x += 20;

		}*/

		position.x += 1.2*speed;

	}


	if (c1->type == Collider::Type::PLAYER && c2 == App->scene->frontCamLimit) {
		AllAnimations.W_KickR.loopCount = 0;
		AllAnimations.W_KickL.loopCount = 0;
		idle = true;

		/*while (position.x > App->scene->aux - 33.3333333333f) {
			position.x = App->scene->aux - 33.3333333333f;
		}*/
		position.x -= 1.2 * speed;

	}

	if (c1 == FireBall.collider && (c2 == App->scene->frontCamLimit || c2 == App->scene->backCamLimit)) {
		FireBall.destroyed = true;
		FireBall.CurrentShot = &FireBall.despawned;
		FireBall.collider->SetPos(-9000, -9000);
	}


	if (c1 == FireBall.collider && c2 != App->scene->frontCamLimit && c2 != App->scene->backCamLimit) {
		
		FireBall.CurrentShot = &FireBall.ExplodeAnim;
	}

	//-------------------------------------------PowerUp Collisions----------------------------------------
	
	if (c1 == Pcollider && c2->type == Collider::Type::POWER_UP && transforming == false) {

		transforming = true;
		App->audio->PlayFx(powerUp, -1);

		//El disable no funciona de momento, voy a esconderlo
		App->powers->gotten = true;
		App->powers->collider->SetPos(-3000, -3000);
		App->powers->Disable();


	}

	// -------------------------------------------Ground Collisions-------------------------------

	if (c1 == Pcollider && c2->type == Collider::Type::PLATFORM) {
		
		//Change to landing later
		currentAnimation->Reset();
		position.y--;
		airSt = AirState::GROUND;
		idle = true;
		jumped = false;
		
	}
	else {
		//airSt = AirState::AIRBORN;
	}

	if (c1 == Pcollider && c2->type == Collider::Type::WALL) {
		position.x-=2;
		AllAnimations.W_KickR.loopCount = 0;
		AllAnimations.W_KickL.loopCount = 0;

	}
	if (c1 == Pcollider && c2->type == Collider::Type::WALL_RIGHT) {
		position.x += 2;
		AllAnimations.W_KickR.loopCount = 0;
		AllAnimations.W_KickL.loopCount = 0;
	}

	// -------------------------------------------Border Collisions-------------------------------

	if (c1 == Pcollider && c2->type == Collider::Type::BORDER && airSt != AirState::AIRBORN) {
		airSt = AirState::AIRBORN;
	}

	//-------------------Neff Collisions-------------------//

	if (c1->type == Collider::Type::PLAYER && c2->type == Collider::Type::RANGE) {

		Neff_pushBack--;
		if (Neff_pushBack <= 0) {
			Neff_pushBack = 12 - delay;
			position.x -= 8;
			position.y -= 8;
			airSt = AirState::AIRBORN;
			delay++;
		}



	}

}


void ModulePlayer:: KnockBack(float x) {
	
		//airSt == AirState::AIRBORN;
		//knockImpulse -= Gravity;
		//position.y -= knockImpulse;
		//
		//position.x -= x;
		//if (position.y >= 190) {
		//	airSt = AirState::GROUND;
		//	position.y = 190;
		//	idle = true;
		//	//jumpRight.Reset();
		///*	knockImpulse = 10;*/
		//}
	
}

void ModulePlayer::KnockBackL(float x) {

	//airSt == AirState::AIRBORN;
	//knockImpulse -= Gravity;
	//position.y -= knockImpulse;
	//
	//position.x += x;
	//if (position.y >= 190) {
	//	airSt = AirState::GROUND;
	//	position.y = 190;
	//	idle = true;
	//	//jumpRight.Reset();
	//	/*knockImpulse = 1;*/
	//}

}
void ModulePlayer::PlayerHit(Collider* c2) {

	Deathcollider->SetPos(1300, 1200);
	firstHit = true;
	/*if ((c2->rect.x + c2->rect.w) >= Pcollider->rect.x) {
		knockImpulse = -1.0f;
	} else if (c2->rect.x < (Pcollider->rect.x+ Pcollider->rect.w) ) {
		knockImpulse = 1.0f;

	}*/
	
	knockImpulse = 1.0f;
	iFrames = true;
	hp--;


	position.y -= 0.1f;
	if (position.y < 190) {
		//shoudl call a different knockbackfunction
		if (c2 != Deathcollider) {
			//KnockBack(25);
			playerKnocked = true;
			if (hp > 0) { App->audio->PlayFx(loseHP, 6); }
			if (dir == Direction::LEFT) {
				idle = false;
				currentAnimation = &AllAnimations.knockBackLeft;
			}
			if (dir == Direction::RIGHT) {
				currentAnimation = &AllAnimations.knockBackRight;
				idle = false;
			}
		}
	}

	playerDamaged();

}
void ModulePlayer::PlayerBump() {

	knockImpulse = 1.0f;
	iFrames = true;

	if (hp > 0) {
		App->audio->PlayFx(loseHP, 6);
	}

	position.y -= 0.1f;
	if (position.y < 190) {
		if (dir == Direction::RIGHT) { KnockBack(0.5); }
		if (dir == Direction::LEFT) { KnockBackL(0.5); }

		if (dir == Direction::LEFT) {
			idle = false;
			currentAnimation = &AllAnimations.knockBackLeft;
		}
		if (dir == Direction::RIGHT) {
			currentAnimation = &AllAnimations.knockBackRight;
			idle = false;
		}
	}

	playerDamaged();


}
void ModulePlayer::playerDamaged() {
	if (hp <= 0)
	{
		hp = 3;
		lives--;
	}

	if (lives <= 0)
	{

		
		hp = 0;
		//DEATH
		App->audio->PlayFx(playerDeathFX, 4);
		App->audio->PlayMusic("Assets/Music/Game_Over.ogg", 0.0f);
		destroyed = true;
		start = false;
		
		App->fade->FadeToBlack((Module*)App->scene, (Module*)App->sceneIntro, 60.0f);
	}
}

void ModulePlayer::Gravity_() {
	// Gravity_ Function
	if (airSt == AirState::AIRBORN) {
		impulse -= Gravity;
	}
	else {
		impulse = 3.5f;
	}
}

bool kickR = false;
bool kickL = false;


void ModulePlayer::WereWolfMovement() {


	attack = 4;
	speed = 3.5;



	if (tranSt == Transform::WOLF) {

		if (idle == true && airSt == AirState::GROUND && iFrames == false) {
			AllAnimations.W_KickR.loopCount = 0;
			AllAnimations.W_KickL.loopCount = 0;
			knockImpulse = 0;
			WolfKickCollider->SetPos(-9000, 900);
		}

		if (AllAnimations.knockBackLeft.HasFinished() == true) {
			idle = true;
			airSt == AirState::GROUND;
			AllAnimations.knockBackLeft.loopCount--;
		}
		if (AllAnimations.knockBackRight.HasFinished() == true) {
			idle = true;
			airSt == AirState::GROUND;
			AllAnimations.knockBackRight.loopCount--;
		}


		//Update Collider to current player pos, change it depending on direction and AirState
 		if (dir == Direction::RIGHT && airSt == AirState::GROUND) { Pcollider->SetPos(position.x + 71, position.y - 68); }
		if (dir == Direction::LEFT && airSt == AirState::GROUND) { Pcollider->SetPos(position.x + 75, position.y - 68); }

		if (dir == Direction::RIGHT && airSt == AirState::CROUCH)
		{
			Pcollider->SetPos(0, position.y - 5000);
			Crouchcollider->SetPos(position.x + 75, position.y - 40);
		}
		if (dir == Direction::LEFT && airSt == AirState::CROUCH)
		{
			Pcollider->SetPos(0, position.y - 5000);
			Crouchcollider->SetPos(position.x + 40, position.y - 40);
		}
		if (airSt != AirState::CROUCH) { Crouchcollider->SetPos(0, -5000); }

		if (dir == Direction::RIGHT && airSt == AirState::AIRBORN && kickR == false ) { Pcollider->SetPos(position.x + 83, position.y - 60); }
		if (dir == Direction::LEFT && airSt == AirState::AIRBORN && kickL == false) { Pcollider->SetPos(position.x + 65, position.y - 60); }

		// When kicking in the air
		if (dir == Direction::RIGHT && airSt == AirState::AIRBORN && kickR == true) { 
			Pcollider->SetPos(position.x + 63, position.y - 75);
		}
		if (dir == Direction::LEFT && airSt == AirState::AIRBORN && kickL ==  true) 
		{ 
			Pcollider->SetPos(position.x + 65, position.y - 75); 
		}

		if (idle == true) {
			attackCollider->SetPos(1000, 1000);
			kickCollider->SetPos(1000, 1000);
		}
		//Jump function



		if (airSt == AirState::LANDING) {
			if (dir == Direction::LEFT) { currentAnimation = &AllAnimations.P1LandingL; }
			if (dir == Direction::RIGHT) { currentAnimation = &AllAnimations.P1LandingR; }
			landing--;
		}
		if (landing <= 0) {
			airSt = AirState::GROUND;
			landing = 5;
		}
		//Reset the currentAnimation back to idle, either left/right, ground/crouch before updating the logic
		if (idle == true && dir == Direction::RIGHT && airSt == AirState::GROUND)
		{
			currentAnimation = &AllAnimations.W_IdleR;
		}
		if (idle == true && dir == Direction::LEFT && airSt == AirState::GROUND)
		{
			currentAnimation = &AllAnimations.W_IdleL;
		}
		if (idle == true && dir == Direction::RIGHT && airSt == AirState::CROUCH) {
			currentAnimation = &AllAnimations.W_crouchR;
			//Pcollider->SetPos(-5612, -5612);
		}
		if (idle == true && dir == Direction::LEFT && airSt == AirState::CROUCH) {
			currentAnimation = &AllAnimations.W_crouchL;
			Pcollider->SetPos(-5612, -5612);
		}
		if (idle == true && dir == Direction::RIGHT && airSt == AirState::AIRBORN) {
			currentAnimation = &AllAnimations.W_jumpR;
		}
		if (idle == true && dir == Direction::LEFT && airSt == AirState::AIRBORN) {
			currentAnimation = &AllAnimations.W_jumpL;
		}



		if (App->input->keys[SDL_SCANCODE_D] == KEY_REPEAT || App->input->pads[0].l_x > 0.5f) {
			if (!destroyed && knockImpulse == 0) {
				if (idle == true && airSt == AirState::GROUND)/* Can't move if punching */ {
					//change direction
					dir = Direction::RIGHT;
					currentAnimation = &AllAnimations.W_walkR;
					position.x += speed;
				}
				//Air
				if (airSt == AirState::AIRBORN) {
					position.x += AirSpeed;
				}
			}

		}


		if (App->input->keys[SDL_SCANCODE_A] == KEY_REPEAT || App->input->pads[0].l_x < -0.5f) {
			if (!destroyed && knockImpulse == 0) {
				if (idle == true && airSt == AirState::GROUND)/* Can't move if punching */ {
					//change direction
					dir = Direction::LEFT;

					currentAnimation = &AllAnimations.W_walkL;
					position.x -= speed;
				}
			}
			//Air
			if (airSt == AirState::AIRBORN) {
				position.x -= AirSpeed;
			}
		}

		if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT || App->input->pads[0].l_y > 0.5f) {

			if (airSt == AirState::GROUND && idle == true) {

				airSt = AirState::CROUCH;

				if (dir == Direction::LEFT) {
					currentAnimation = &AllAnimations.W_crouchL;
				}
				if (dir == Direction::RIGHT) {
					currentAnimation = &AllAnimations.W_crouchR;
				}
			}
		}

		// Punch with fireball
		if (FireBall.destroyed && (App->input->keys[SDL_SCANCODE_Z] == KEY_DOWN || App->input->pads[0].b)) {

			FireBall.destroyed = false;
			FireBall.exploded = false;

			if (hitEnemy == false) {
				App->audio->PlayFx(nonLethalAtt, 3);
			}

			//FireBall direction is player direction
			FireBall.dir = dir;

			if (idle == true && dir == Direction::RIGHT && airSt == AirState::GROUND) {
				AllAnimations.W_punchR.Reset();
				currentAnimation = &AllAnimations.W_punchR;

				// FireBall stuff
				
					wolfPunch = true;
					FireBall.CurrentShot = &FireBall.GrowAnim;
					FireBall.ShotPosition.x = position.x + 40;
					FireBall.ShotPosition.y = position.y - 80;
				

				idle = false;
			}

			if (idle == true && dir == Direction::LEFT && airSt == AirState::GROUND)
			{
				AllAnimations.W_punchL.Reset();
				currentAnimation = &AllAnimations.W_punchL;

				// FireBall stuff

				wolfPunch = true;
				FireBall.CurrentShot = &FireBall.GrowAnimL;
				FireBall.ShotPosition.x = position.x + 40;
				FireBall.ShotPosition.y = position.y - 80;


				idle = false;
			}

			if (idle == true && dir == Direction::RIGHT && airSt == AirState::CROUCH) {
				AllAnimations.W_crouch_punchR.Reset();
				currentAnimation = &AllAnimations.W_crouch_punchR;
				attackCollider->SetPos(position.x + 112, position.y - 40);

				// FireBall stuff

				wolfPunch = true;
				FireBall.CurrentShot = &FireBall.GrowAnim;
				FireBall.ShotPosition.x = position.x + 40;
				FireBall.ShotPosition.y = position.y - 60;

				idle = false;
			}

			if (idle == true && dir == Direction::LEFT && airSt == AirState::CROUCH) {
				AllAnimations.W_crouch_punchL.Reset();
				currentAnimation = &AllAnimations.W_crouch_punchL;
				
				// FireBall stuff

				wolfPunch = true;
				FireBall.CurrentShot = &FireBall.GrowAnimL;
				FireBall.ShotPosition.x = position.x + 40;
				FireBall.ShotPosition.y = position.y - 60;

				attackCollider->SetPos(position.x + 4, position.y - 40);
				idle = false;
			}

			if (airSt == AirState::AIRBORN) {
				if (dir == Direction::LEFT) {
					currentAnimation = &AllAnimations.W_jump_punchL;
					// FireBall stuff

					wolfPunch = true;
					FireBall.CurrentShot = &FireBall.GrowAnimL;
					FireBall.ShotPosition.x = position.x + 20;
					FireBall.ShotPosition.y = position.y - 60;

					attackCollider->SetPos(position.x + 40, position.y - 40);
					idle = false;
				}
				if (dir == Direction::RIGHT) {
					currentAnimation = &AllAnimations.W_jump_punchR;
					// FireBall stuff

					wolfPunch = true;
					FireBall.CurrentShot = &FireBall.GrowAnim;
					FireBall.ShotPosition.x = position.x + 20;
					FireBall.ShotPosition.y = position.y - 60;

					attackCollider->SetPos(position.x + 40, position.y - 40);
					idle = false;
				}
			}
		}

		// Punch without fireball
		else if ((App->input->keys[SDL_SCANCODE_Z] == KEY_DOWN || App->input->pads[0].b) && FireBall.destroyed == false) {

			

			if (hitEnemy == false) {
				App->audio->PlayFx(nonLethalAtt, 3);
			}

			if (idle == true && dir == Direction::RIGHT && airSt == AirState::GROUND) {
				AllAnimations.W_punchR_depleted.Reset();
				currentAnimation = &AllAnimations.W_punchR_depleted;


				//activate punch collider when player punches
				attackCollider->SetPos(position.x + 96, position.y - 60);

				idle = false;
			}

			if (idle == true && dir == Direction::LEFT && airSt == AirState::GROUND)
			{
				AllAnimations.W_punchL_depleted.Reset();
				currentAnimation = &AllAnimations.W_punchL_depleted;

				//activate punch collider when player punches
				attackCollider->SetPos(position.x + 18, position.y - 60);

				idle = false;
			}

			if (idle == true && dir == Direction::RIGHT && airSt == AirState::CROUCH) {
				AllAnimations.W_crouch_punchR.Reset();
				currentAnimation = &AllAnimations.W_crouch_punchR;
				attackCollider->SetPos(position.x + 112, position.y - 40);
				idle = false;
			}

			if (idle == true && dir == Direction::LEFT && airSt == AirState::CROUCH) {
				AllAnimations.W_crouch_punchL.Reset();
				currentAnimation = &AllAnimations.W_crouch_punchL;
				//CHANGE x
				attackCollider->SetPos(position.x + 4, position.y - 40);
				idle = false;
			}

			if (airSt == AirState::AIRBORN) {
				if (dir == Direction::LEFT) {
					currentAnimation = &AllAnimations.W_jump_punchL;

					idle = false;
				}
				if (dir == Direction::RIGHT) {
					currentAnimation = &AllAnimations.W_jump_punchR;

					idle = false;
				}
			}
		}


		// after punch fireball movement

		ModulePlayer::FireBallMovement();

		//FireBall animation update
		if (FireBall.GrowAnim.HasFinished() == true) {
			FireBall.GrowAnim.Reset();
			FireBall.CurrentShot = &FireBall.TravelAnim;
			FireBall.GrowAnim.loopCount = 0;
		}
		if (FireBall.GrowAnimL.HasFinished() == true) {
			FireBall.GrowAnimL.Reset();
			FireBall.CurrentShot = &FireBall.TravelAnimL;
			FireBall.GrowAnimL.loopCount = 0;
		}


		if (FireBall.ExplodeAnim.HasFinished() == true) {
			FireBall.ExplodeAnim.loopCount = 0;
			FireBall.ExplodeAnim.Reset();
			FireBall.destroyed = true;
			FireBall.exploded = true;
			FireBall.CurrentShot = &FireBall.despawned;
			FireBall.collider->SetPos(-9000, -9000);
		}

		if (AllAnimations.W_crouch_punchL.HasFinished() == true) {
			AllAnimations.W_crouch_punchL.Reset();
			AllAnimations.W_crouch_punchL.loopCount = 0;
			idle = true;
		}

		if (AllAnimations.W_crouch_punchR.HasFinished() == true) {
			AllAnimations.W_crouch_punchR.Reset();
			AllAnimations.W_crouch_punchR.loopCount = 0;
			idle = true;
		}

		//CHANGE
		if (currentAnimation == &AllAnimations.P1JumpPunchL) { attackCollider->SetPos(position.x + 0, position.y - 60); }
		if (currentAnimation == &AllAnimations.P1JumpPunchR) { attackCollider->SetPos(position.x + 59, position.y - 60); }
		if (currentAnimation == &AllAnimations.P1JumpKickL) { attackCollider->SetPos(position.x + 0, position.y - 40); }
		if (currentAnimation == &AllAnimations.P1JumpKickR) { attackCollider->SetPos(position.x + 55, position.y - 40); }


		if (App->input->keys[SDL_SCANCODE_X] == KEY_DOWN || App->input->pads[0].a) {

			if (hitEnemy == false) {
				App->audio->PlayFx(nonLethalAtt, 3);
			}

			if (idle == true && dir == Direction::RIGHT && airSt == AirState::GROUND) {
				AllAnimations.W_KickR.Reset();
				currentAnimation = &AllAnimations.W_KickR;

				idle = false;
				kickR = true;
			}
			if (idle == true && dir == Direction::LEFT && airSt == AirState::GROUND)
			{
				AllAnimations.W_KickL.Reset();
				currentAnimation = &AllAnimations.W_KickL;

				idle = false;
				kickL = true;
			}
			if (idle == true && dir == Direction::RIGHT && airSt == AirState::CROUCH) {
				AllAnimations.W_KickR.Reset();
				currentAnimation = &AllAnimations.W_KickR;
				position.y += 15;
				idle = false;
				kickR = true;
			}
			if (idle == true && dir == Direction::LEFT && airSt == AirState::CROUCH) {
				AllAnimations.W_KickL.Reset();
				currentAnimation = &AllAnimations.W_KickL;
				position.y += 15;
				idle = false;
				kickL = true;
			}
			if (airSt == AirState::AIRBORN) {
				if (dir == Direction::LEFT) {
					AllAnimations.W_KickL.Reset();
					currentAnimation = &AllAnimations.W_KickL;
					position.y -= 10;
					idle = false;
					kickL = true;
				}
				if (dir == Direction::RIGHT) {
					AllAnimations.W_KickR.Reset();
					currentAnimation = &AllAnimations.W_KickR;
					position.y -= 10;
					idle = false;
					kickR = true;
				}
			}
		}

		if (currentAnimation == &AllAnimations.W_KickR) {
			WolfKick();
		}
		if (currentAnimation == &AllAnimations.W_KickL) {
			WolfKick();
		}

		//OUTSIDE THE IF
		if (AllAnimations.W_punchR.HasFinished() == true) {
			AllAnimations.W_punchR.loopCount--;
			idle = true;
		}
		//OUTSIDE THE IF
		if (AllAnimations.W_punchL.HasFinished() == true) {
			AllAnimations.W_punchL.loopCount--;
			idle = true;
		}
		//OUTSIDE THE IF
		if (AllAnimations.W_punchR_depleted.HasFinished() == true) {
			AllAnimations.W_punchR_depleted.loopCount--;
			idle = true;
		}
		//OUTSIDE THE IF
		if (AllAnimations.W_punchL_depleted.HasFinished() == true) {
			AllAnimations.W_punchL_depleted.loopCount--;
			idle = true;
		}

		if (AllAnimations.P1CrouchPunchR.HasFinished() == true) {
			AllAnimations.P1CrouchPunchR.loopCount--;
			idle = true;
			if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT || App->input->pads[0].l_y > 0.5f) {
				airSt = AirState::CROUCH;
			}
			else { airSt = AirState::GROUND; }

		}
		if (AllAnimations.P1CrouchPunchL.HasFinished() == true) {
			AllAnimations.P1CrouchPunchL.loopCount--;
			idle = true;
			if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT|| App->input->pads[0].l_y > 0.5f) {
				airSt = AirState::CROUCH;
			}
			else { airSt = AirState::GROUND; }

		}
		if (AllAnimations.P1CrouchKickR.HasFinished() == true) {
			AllAnimations.P1CrouchKickR.loopCount--;
			idle = true;
			if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT || App->input->pads[0].l_y > 0.5f) {
				airSt = AirState::CROUCH;
			}
			else { airSt = AirState::GROUND; }
		}
		if (AllAnimations.P1CrouchKickL.HasFinished() == true) {
			AllAnimations.P1CrouchKickL.loopCount--;
			idle = true;
			if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT || App->input->pads[0].l_y > 0.5f) {
				airSt = AirState::CROUCH;
			}
			else { airSt = AirState::GROUND; }
		}

		if (AllAnimations.W_KickR.HasFinished() == true) {
			kickR = false;
			AllAnimations.W_KickR.loopCount = 0;   //VERY IMPORTANT , since HasFinished checks if the loop count has surpassed 0, after the animation has finished reset loop count
			idle = true;
		}
		//OUTSIDE THE IF
		if (AllAnimations.W_KickL.HasFinished() == true) {
			kickL = false;
			AllAnimations.P1KickLeft.loopCount = 0;   //VERY IMPORTANT , since HasFinished checks if the loop count has surpassed 0, after the animation has finished reset loop count
			idle = true;
		}

		if (AllAnimations.P1CrouchKickL.HasFinished() == true) {
			AllAnimations.P1CrouchKickL.loopCount--;
			idle = true;
		}

		if (AllAnimations.P1CrouchKickR.HasFinished() == true) {
			AllAnimations.P1CrouchKickR.loopCount--;
			idle = true;
		}


		//Player gets killed
		if (destroyed) {

			if (dir == Direction::RIGHT) {
				if (destroyedCountdown > 297)
				{
					position.x -= 0.7f;
				}
				currentAnimation = &AllAnimations.deathAnimRight;

			}
			else if (dir == Direction::LEFT) {
				if (destroyedCountdown > 297) {
					position.x += 0.7f;
				}
				currentAnimation = &AllAnimations.deathAnimLeft;
			}

			impulse -= Gravity;
			position.y -= impulse;

			if (position.y >= 190) {
				position.x += 0;
				position.y = 190;

				destroyedCountdown -= 0.5f;
				if (destroyedCountdown <= 0) {
					//return update_status::UPDATE_STOP;

					App->fade->FadeToBlack((Module*)App->scene, (Module*)App->sceneIntro, 60);
					idle = true;
					airSt = AirState::GROUND;
					tranSt = Transform::DEFAULT;
					this->CleanUp();
				}

			}
			//if (position.y >= 190) {
			//	position.x += 0;
			//	position.y = 190;
			//	destroyedCountdown -= 0.5f;
			//	if (destroyedCountdown <= 0) {
			//		//return update_status::UPDATE_STOP;

			//		App->fade->FadeToBlack((Module*)App->scene, (Module*)App->sceneIntro, 60.0f);
			//		this->CleanUp();
			//	}

			//}

		}

		//FireBall.bugPatched--;
		//if (FireBall.bugPatched <= 0) {
		//	FireBall.destroyed = false;
		//}


	}
}


void ModulePlayer::WolfKick() {

	if (dir == Direction::RIGHT) {

		position.x += 4;

		WolfKickCollider->SetPos(position.x + 78, position.y - 70);


	}

	if (dir == Direction::LEFT) {
		position.x -= 4;
		WolfKickCollider->SetPos(position.x + 5, position.y - 70);
	}

	if (airSt == AirState::AIRBORN) {
		position.y-=2;
	}

	 
}

void ModulePlayer::FireBallMovement() {

	if (FireBall.dir == Direction::RIGHT && FireBall.CurrentShot != &FireBall.ExplodeAnim && FireBall.exploded == false) {
		FireBall.ShotPosition.x += 6;

		// Detroy when out of bounds
		// CHANGE TO CAMERA BORDER
		if (FireBall.ShotPosition.x > position.x + 400) {
			
		}
		FireBall.collider->SetPos(FireBall.ShotPosition.x + 20, FireBall.ShotPosition.y + 17);
	}

	if (FireBall.dir == Direction::LEFT && FireBall.CurrentShot != &FireBall.ExplodeAnim && FireBall.exploded == false) {
		FireBall.ShotPosition.x -= 6;
		FireBall.collider->SetPos(FireBall.ShotPosition.x + 20, FireBall.ShotPosition.y + 17);
	}

	
}