#include "ModulePlayer.h"

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

#include <stdio.h>

#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib")
#pragma comment( lib, "SDL/libx86/SDL2main.lib")

#include "SDL/include/SDL_scancode.h"
//the lower the higher
#define MAX_HEIGHT 147
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


#define Gravity 0.07f;


//deberia estar en el .h
Uint32 startTime = 0;
bool idle = true;
int knockPos;


ModulePlayer::ModulePlayer()
{
	

	position.x = 100;
	position.y = 190;
	

	//Default direction
	dir = Direction::RIGHT;
	//Default airstate
	airSt = AirState::GROUND;
	//Default transformation
	tranSt = Transform::DEFAULT;

	// idle animation (arcade sprite sheet)
	// x,y (top of rect in sprite sheet) , width, height of rect
	idleAnimRight.PushBack({0, 0, 54, 75});
	idleAnimRight.speed = 0.1f;

	idleAnimLeft.PushBack({ 165,75, 54,75 });
	idleAnimLeft.speed = 0.1f;

	// walk forward animation (arcade sprite sheet)
	forwardAnim.PushBack({56, 0, 54, 75});
	forwardAnim.PushBack({ 112, 0, 54, 75 });
	forwardAnim.PushBack({ 168, 0, 54, 75 });

	forwardAnim.loop = true;
	
	forwardAnim.speed = WALKANIMSPEED;


	backAnim.PushBack({ 0,76,54,75 });
	backAnim.PushBack({ 56,76,54,75 });
	backAnim.PushBack({ 111,76,54,75 });
	backAnim.speed = WALKANIMSPEED;

	//Punch animation
	punchAnimRight.PushBack({ 0,151,54,75 });
	punchAnimRight.PushBack({ 56,151,54,75 });
	punchAnimRight.PushBack({ 110,151,111,75 });//Extra large frame for extended punch
	punchAnimRight.loop = false;
	punchAnimRight.totalFrames = 3;
	
	punchAnimRight.speed = PUNCHANIMSPEED;


	punchAnimLeft.PushBack({ 385,227,54,75 });
	punchAnimLeft.PushBack({ 330,227,54,75 });
	punchAnimLeft.PushBack({ 261,227,73,75 });//Extra large frame for extended punch
	punchAnimLeft.loop = false;

	punchAnimLeft.speed = PUNCHANIMSPEED;
	punchAnimLeft.totalFrames = 3;


	crouchAnimRight.PushBack({ 0, 226, 54, 75 });
	crouchAnimRight.speed = CROUCHANIMSPEED;
	crouchAnimRight.totalFrames = 1;

	crouchAnimLeft.PushBack({ 386,303,54,75 });
	crouchAnimLeft.speed = CROUCHANIMSPEED;
	crouchAnimLeft.totalFrames = 1;


	crouchPunchRight.PushBack({55, 226, 54, 75 });
	crouchPunchRight.PushBack({110, 226, 108, 75 });
	crouchPunchRight.loop = false;
	crouchPunchRight.totalFrames = 2;
	crouchPunchRight.speed = CROUCHPUNCHSPEED;

	crouchPunchLeft.PushBack({ 331, 303, 54, 75 });
	crouchPunchLeft.PushBack({ 253, 303, 63, 75 });
	crouchPunchLeft.loop = false;
	crouchPunchLeft.totalFrames = 2;
	crouchPunchLeft.speed = CROUCHPUNCHSPEED;


	kickAnimRight.PushBack({221, 379, 87, 75});
	kickAnimRight.PushBack({309, 379, 87, 75 });
	kickAnimRight.PushBack({397, 379, 87, 75 });
	kickAnimRight.PushBack({309, 379, 87, 75 });

	kickAnimRight.speed = KICKANIMSPEED;
	kickAnimRight.loop = false;
	kickAnimRight.totalFrames = 4;


	kickAnimLeft.PushBack({ 221, 455, 87, 75 });
	kickAnimLeft.PushBack({ 309, 455, 87, 75 });
	kickAnimLeft.PushBack({ 397, 455, 87, 75 });
	kickAnimLeft.PushBack({ 309, 455, 87, 75 });

	kickAnimLeft.speed = KICKANIMSPEED;
	kickAnimLeft.loop = false;
	kickAnimLeft.totalFrames = 4;

	kickCrouchRight.PushBack({ 1,455,72,75 });
	kickCrouchRight.PushBack({ 74,455,72,75 });
	kickCrouchRight.PushBack({ 1,455,72,75 });
	kickCrouchRight.speed = CROUCHKICKSPEED;
	kickCrouchRight.totalFrames = 3;
	kickCrouchRight.loop = false;

	kickCrouchLeft.PushBack({ 1,531,72,75 });
	kickCrouchLeft.PushBack({ 74,531,72,75 });
	kickCrouchLeft.PushBack({ 1,531,72,75 });
	kickCrouchLeft.speed = CROUCHKICKSPEED;
	kickCrouchLeft.totalFrames = 3;
	kickCrouchLeft.loop = false;


	deathAnimRight.PushBack({ 266,531,72,75 });
	deathAnimRight.PushBack({ 339,531,72,75 });
	deathAnimRight.PushBack({ 412,531,72,75 });
	deathAnimRight.speed = DEATHANIMSPEED;
	deathAnimRight.totalFrames = 3;
	deathAnimRight.loop = false;

	hitAnimRight.PushBack({ 1,607,68,70 });
	hitAnimRight.PushBack({ 70,607,68,70 });
	hitAnimRight.totalFrames = 2;
	hitAnimRight.loop = false;
	hitAnimRight.speed = HITANIMSPEED;

	hitAnimLeft.PushBack({1,678,68,70});
	hitAnimLeft.PushBack({70,678,68,70});
	hitAnimLeft.totalFrames = 2;
	hitAnimLeft.loop = false;
	hitAnimLeft.speed = HITANIMSPEED;

	knockBackRight.PushBack({266, 531, 72, 75});
	knockBackRight.totalFrames = 1;
	knockBackRight.loop = false;
	knockBackRight.speed = 0.07f;

	knockBackLeft.PushBack({266,607,72,75});
	knockBackLeft.totalFrames = 1;
	knockBackLeft.loop = false;
	knockBackLeft.speed = 0.07f;

	HitContinueRight.PushBack({ 266,531,72,75 });
	HitContinueRight.PushBack({ 339,531,72,75 });
	HitContinueRight.PushBack({ 412,531,72,75 });
	HitContinueRight.PushBack({139, 607, 72, 75});
	HitContinueRight.PushBack({208, 607, 72, 75});
	HitContinueRight.totalFrames = 5;
	HitContinueRight.loop = false;
	HitContinueRight.speed = 0.07f;

	HitContinueLeft.PushBack({ 266,531,72,75 });
	HitContinueLeft.PushBack({ 339,531,72,75 });
	HitContinueLeft.PushBack({ 412,531,72,75 });
	HitContinueLeft.PushBack({ 139, 678, 72, 75 });
	HitContinueLeft.PushBack({ 208, 678, 72, 75 });
	HitContinueLeft.totalFrames = 5;
	HitContinueLeft.loop = false;
	HitContinueLeft.speed = 0.07f;

	deathAnimLeft.PushBack({ 266,607,72,75 });
	deathAnimLeft.PushBack({ 339,607,72,75 });
	deathAnimLeft.PushBack({ 412,607,72,75 });
	deathAnimLeft.speed = DEATHANIMSPEED;
	deathAnimLeft.totalFrames = 3;
	deathAnimLeft.loop = false;
	
	jumpRight.PushBack({ 0,303,54,75 });
	jumpRight.PushBack({ 56,303,54,75 });
	jumpRight.speed = JUMPANIMSPEED;
	
	jumpLeft.PushBack({ 0,379,54,75 });
	jumpLeft.PushBack({ 56,379,54,75 });
	jumpLeft.speed = JUMPANIMSPEED;

	airPunchLeft.PushBack({ 354,749,54,75 });
	airPunchLeft.loop = true;
	airPunchRight.PushBack({ 0,749,60,75 });
	airPunchRight.loop = true;
	
	airKickRight.PushBack({ 70,749,68,75 });
	airKickRight.PushBack({ 140,749,68,75 });
	airKickRight.speed = AIRKICKSPEED;

	airKickLeft.PushBack({ 277,749,68,75 });
	airKickLeft.PushBack({ 208,749,68,75 });
	airKickLeft.speed = AIRKICKSPEED;
	airKickLeft.totalFrames = 2;

	LandingLeft.PushBack({ 111,379,54,75 });
	LandingRight.PushBack({ 111,303,54,75 });
}


ModulePlayer::~ModulePlayer()
{

}

bool ModulePlayer::Start()
{
	LOG("Loading player textures");

	bool ret = true;
	
	texture = App->textures->Load("Assets/ABfullspritesProto.png"); // arcade version

	//Initialize collider
	Pcollider = App->collisions->AddCollider({ 100,300,20,68 }, Collider::Type::PLAYER, this);
	//CHANGE listener of attack to enemy
	attackCollider = App->collisions->AddCollider({ 100,300,33,19 }, Collider::Type::PLAYER_SHOT, this);
	kickCollider = App->collisions->AddCollider({ 100,300,19,33 }, Collider::Type::PLAYER_SHOT, this);
	//font table
	char lookupTable[] = { "abcdefghijklmnopqrstuvwxyz0123456789.,ç'?!*$%&()+-/<=>© " };
	scoreFont = App->fonts->Load("Assets/font_spritesheet.png", lookupTable, 1);

	//player Sound FX
	playerDeathFX = App->audio->LoadFx("Assets/FX/Player_Death.wav");
	nonLethalAtt = App->audio->LoadFx("Assets/FX/Non-Lethal_Punch.wav");
	lethalAtt = App->audio->LoadFx("Assets/FX/Lethal_Punch.wav");
	loseHP = App->audio->LoadFx("Assets/FX/lose_1_hp.wav");
	powerUp = App->audio->LoadFx("Assets/FX/Power_Up.wav");

	return ret;
}

update_status ModulePlayer::Update()
{
	if (idle == true && airSt == AirState::GROUND && iFrames == false) { 
		position.y = 190;
		knockImpulse = 0;
	}

	if (knockBackLeft.HasFinished() == true) {
		idle = true;
		airSt == AirState::GROUND;
		knockBackLeft.loopCount--;
	}
	if (knockBackRight.HasFinished() == true) {
		idle = true;
		airSt == AirState::GROUND;
		knockBackRight.loopCount--;
	}

	//Update Collider to current player pos, change it depending on direction and AirState
	if (dir == Direction::RIGHT && airSt == AirState::GROUND) { Pcollider->SetPos(position.x + 18, position.y - 65); }
	if (dir == Direction::LEFT && airSt == AirState::GROUND) { Pcollider->SetPos(position.x + 20, position.y - 65); }

	if (dir == Direction::RIGHT && airSt == AirState::CROUCH)
	{
		Pcollider->SetPos(position.x + 20, position.y - 40);
	}
	if (dir == Direction::LEFT && airSt == AirState::CROUCH)
	{
		Pcollider->SetPos(position.x + 17, position.y - 40);
	}

	if (dir == Direction::RIGHT && airSt == AirState::AIRBORN) { Pcollider->SetPos(position.x + 20, position.y - 60); }
	if (dir == Direction::LEFT && airSt == AirState::AIRBORN) { Pcollider->SetPos(position.x + 20, position.y - 60); }

	if (idle == true) {
			attackCollider->SetPos(1000, 1000);
	}
	//Jump function
	//impulse bigger, the stronger it jumps
	if (idle == true && airSt == AirState::GROUND && App->input->keys[SDL_SCANCODE_SPACE] == KEY_DOWN) {
		landing = 5;
		impulse = 3.2;
		position.y += 0.5;
		airSt = AirState::AIRBORN;
	}
	if (airSt == AirState::AIRBORN) {
		impulse -= Gravity;
		position.y -= impulse;
	}
	//Put a max height that makes the player fall faster for it to dont look like its floating ( not real but like the game )
	//impulse lesser, the faster it falls
	if (airSt == AirState::AIRBORN && position.y < MAX_HEIGHT) {
		impulse = -2.3;
		position.y = MAX_HEIGHT + 2;
	}
	//Reset state to ground when touching the ground
	if (position.y >= 190 && airSt == AirState::AIRBORN) {
		airSt = AirState::LANDING;
		position.y = 190;
		idle = true;
		//jumpRight.Reset();
	}
	if (airSt == AirState::LANDING) {
		if (dir == Direction::LEFT) { currentAnimation = &LandingLeft; }
		if (dir == Direction::RIGHT) { currentAnimation = &LandingRight; }
		landing--;
	}
	if (landing <= 0) {
		airSt = AirState::GROUND;
		landing = 5;
	}
	//Reset the currentAnimation back to idle, either left/right, ground/crouch before updating the logic
	if (idle == true && dir == Direction::RIGHT && airSt == AirState::GROUND)
	{
		currentAnimation = &idleAnimRight;
	}
	if (idle == true && dir == Direction::LEFT && airSt == AirState::GROUND)
	{
		currentAnimation = &idleAnimLeft;
	}
	if (idle == true && dir == Direction::RIGHT && airSt == AirState::CROUCH) {
		currentAnimation = &crouchAnimRight;
	}
	if (idle == true && dir == Direction::LEFT && airSt == AirState::CROUCH) {
		currentAnimation = &crouchAnimLeft;
	}
	if (idle == true && dir == Direction::RIGHT && airSt == AirState::AIRBORN) {
		currentAnimation = &jumpRight;
	}
	if (idle == true && dir == Direction::LEFT && airSt == AirState::AIRBORN) {
		currentAnimation = &jumpLeft;
	}
	if (App->input->keys[SDL_SCANCODE_D] == KEY_REPEAT && !destroyed && knockImpulse == 0)
	{
		if (idle == true && airSt == AirState::GROUND)/* Can't move if punching */ {
			//change direction
			dir = Direction::RIGHT;
			currentAnimation = &forwardAnim;
			position.x += speed;
		}
		//Air
		if (airSt == AirState::AIRBORN) {
			position.x += AirSpeed;
		}
	}


	if (App->input->keys[SDL_SCANCODE_A] == KEY_REPEAT && !destroyed && knockImpulse == 0) {
		if (idle == true && airSt == AirState::GROUND)/* Can't move if punching */ {
			//change direction
			dir = Direction::LEFT;

			currentAnimation = &backAnim;
			position.x -= speed;
		}
		//Air
		if (airSt == AirState::AIRBORN) {
			position.x -= AirSpeed;
		}
	}

	if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT && airSt == AirState::GROUND && idle == true) {

		airSt = AirState::CROUCH;

		if (dir == Direction::LEFT) {
			currentAnimation = &crouchAnimLeft;
		}
		if (dir == Direction::RIGHT) {
			currentAnimation = &crouchAnimRight;
		}

	}

	if (App->input->keys[SDL_SCANCODE_Z] == KEY_DOWN) {

		if (hitEnemy == false) {
			App->audio->PlayFx(nonLethalAtt);
		}

		if (idle == true && dir == Direction::RIGHT && airSt == AirState::GROUND) {
			punchAnimRight.Reset();
			currentAnimation = &punchAnimRight;

			
			
			//activate punch collider when player punches
			attackCollider->SetPos(position.x + 38, position.y - 60);

			idle = false;
		}
		if (idle == true && dir == Direction::LEFT && airSt == AirState::GROUND)
		{
			punchAnimLeft.Reset();
			currentAnimation = &punchAnimLeft;
			//activate punch collider when player punches
			attackCollider->SetPos(position.x + 0, position.y - 60);

				idle = false;
		}
		if (idle == true && dir == Direction::RIGHT && airSt == AirState::CROUCH) {
			crouchPunchRight.Reset();
			currentAnimation = &crouchPunchRight;
			attackCollider->SetPos(position.x + 43, position.y - 40);
			idle = false;
		}
		if (idle == true && dir == Direction::LEFT && airSt == AirState::CROUCH) {
			crouchPunchLeft.Reset();
			currentAnimation = &crouchPunchLeft;
			//CHANGE x
			attackCollider->SetPos(position.x + 0, position.y - 40);
			idle = false;
		}
		if (airSt == AirState::AIRBORN) {
			if (dir == Direction::LEFT) {
				currentAnimation = &airPunchLeft;
			
				idle = false;
		}
			if (dir == Direction::RIGHT) {
				currentAnimation = &airPunchRight;
			
				idle = false;
		}
	}
	}
	if(currentAnimation == &airPunchLeft){ attackCollider->SetPos(position.x + 0, position.y - 60); }
	if (currentAnimation == &airPunchRight) { attackCollider->SetPos(position.x + 26, position.y - 60); }
	if (currentAnimation == &airKickLeft) { attackCollider->SetPos(position.x + 0, position.y - 40); }
	if (currentAnimation == &airKickRight) { attackCollider->SetPos(position.x + 33, position.y - 40); }

	if (App->input->keys[SDL_SCANCODE_X] == KEY_DOWN) {

		if (hitEnemy == false) {
			App->audio->PlayFx(nonLethalAtt);
		}

		if (idle == true && dir == Direction::RIGHT && airSt == AirState::GROUND) {
			kickAnimRight.Reset();
			currentAnimation = &kickAnimRight;
			attackCollider->SetPos(position.x + 43, position.y - 30);
			idle = false;
		}
		if (idle == true && dir == Direction::LEFT && airSt == AirState::GROUND)
		{
			kickAnimLeft.Reset();
			currentAnimation = &kickAnimLeft;
			attackCollider->SetPos(position.x -5 , position.y - 30);
			idle = false;
		}
		if (idle == true && dir == Direction::RIGHT && airSt == AirState::CROUCH) {
			kickCrouchRight.Reset();
			currentAnimation = &kickCrouchRight;
			kickCollider->SetPos(position.x + 37, position.y - 60);
			idle = false;
		}
		if (idle == true && dir == Direction::LEFT && airSt == AirState::CROUCH) {
			kickCrouchLeft.Reset();
			currentAnimation = &kickCrouchLeft;
			kickCollider->SetPos(position.x + 20, position.y - 60);
			idle = false;
		}
		if (airSt == AirState::AIRBORN) {
			if (dir == Direction::LEFT) {
				currentAnimation = &airKickLeft;

				idle = false;
			}
			if (dir == Direction::RIGHT) {
				currentAnimation = &airKickRight;

				idle = false;
			}
		}
	}

		//OUTSIDE THE IF
		if (punchAnimRight.HasFinished() == true) {
			punchAnimRight.loopCount--;   
			idle = true;
			//deactivate punch collider
			attackCollider->SetPos(1000, 1000);
		}
		//OUTSIDE THE IF
		if (punchAnimLeft.HasFinished() == true) {
			punchAnimLeft.loopCount--;   
			idle = true;
			//deactivate punch collider
			attackCollider->SetPos(1000, 1000);
		}

		if (crouchPunchRight.HasFinished() == true) {
			crouchPunchRight.loopCount--;  
			idle = true;
			if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT) {
				airSt = AirState::CROUCH;
			}
			else { airSt = AirState::GROUND; }
			//deactivate punch collider
			attackCollider->SetPos(1000, 1000); //quick fix to make collider disappear from scene by sending it oob, TRY TO CHANG

		}
		if (crouchPunchLeft.HasFinished() == true) {
			crouchPunchLeft.loopCount--;   
			idle = true;
			if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT) {
				airSt = AirState::CROUCH;
			}
			else { airSt = AirState::GROUND; }
			//deactivate punch collider
			attackCollider->SetPos(1000, 1000); //quick fix to make collider disappear from scene by sending it oob, TRY TO CHANGE

		}
		if(kickCrouchRight.HasFinished() == true) {
			kickCrouchRight.loopCount--;
			idle = true;
			if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT) {
				airSt = AirState::CROUCH;
			}
			else { airSt = AirState::GROUND; }
			//deactivate punch collider
			kickCollider->SetPos(1000, 1000); //quick fix to make collider disappear from scene by sending it oob, TRY TO CHANG
		}
		if (kickCrouchLeft.HasFinished() == true) {
			kickCrouchLeft.loopCount--;
			idle = true;
			if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT) {
				airSt = AirState::CROUCH;
			}
			else { airSt = AirState::GROUND; }
			//deactivate punch collider
			kickCollider->SetPos(1000, 1000); //quick fix to make collider disappear from scene by sending it oob, TRY TO CHANG
		}
		
		if (kickAnimRight.HasFinished() == true) {
			kickAnimRight.loopCount--;   //VERY IMPORTANT , since HasFinished checks if the loop count has surpassed 0, after the animation has finished reset loop count
			idle = true;
			//deactivate kick collider
			attackCollider->SetPos(1000, 1000);
		}
		//OUTSIDE THE IF
		if (kickAnimLeft.HasFinished() == true) {
			kickAnimLeft.loopCount--;   //VERY IMPORTANT , since HasFinished checks if the loop count has surpassed 0, after the animation has finished reset loop count
			idle = true;
			//deactivate kick collider
			attackCollider->SetPos(1000, 1000);
		}

		if (kickCrouchLeft.HasFinished() == true) {
			kickCrouchLeft.loopCount--;  
			idle = true;
			kickCollider->SetPos(1000, 1000);
		}

		if (kickCrouchRight.HasFinished() == true) {
			kickCrouchRight.loopCount--;
			idle = true;
			kickCollider->SetPos(1000, 1000);
		}
		

		
		if (App->input->keys[SDL_SCANCODE_S] == KEY_UP && idle == true) {
			
			
			airSt = AirState::GROUND;
			//deactivate punch collider
			kickCollider->SetPos(1000, 1000); //quick fix to make collider disappear from scene by sending it oob, TRY TO CHANGE
		}



		//Player gets killed
		if (destroyed) {

			
			App->audio->PlayFx(playerDeathFX, 0);

			
			if (dir == Direction::RIGHT) {
				if (destroyedCountdown > 117)
				{
					position.x -= 0.7f;
				}
				currentAnimation = &deathAnimRight;

			}
			else if (dir == Direction::LEFT){
				if (destroyedCountdown > 117) {
					position.x += 0.7f;
				}
				currentAnimation = &deathAnimLeft;
			}

			impulse -= Gravity;
			position.y -= impulse;

			if (position.y >= 190) {
				position.x += 0;
				position.y = 190;
				destroyedCountdown-=0.5f;
				if (destroyedCountdown <= 0) {
					return update_status::UPDATE_STOP;
				}

			} 
			
		}
		
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

		if (App->input->keys[SDL_SCANCODE_F1] == KEY_DOWN && GodMode == false) {
			GodMode = true;
		}
		if (App->input->keys[SDL_SCANCODE_F1] == KEY_DOWN && GodMode == true) {
			GodMode = false;
			lives = 3;
		}
		if (GodMode == true) {
			lives++;
		}

		return update_status::UPDATE_CONTINUE;
	
}

update_status ModulePlayer::PostUpdate()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	App->render->Blit(texture, position.x, position.y - rect.h, &rect);

	sprintf_s(scoreText, 10, "%7d", score);

	App->fonts->BlitText(40, 8, scoreFont, scoreText);
	App->fonts->BlitText(230, 8, scoreFont, "50000");



	return update_status::UPDATE_CONTINUE;
}




void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 == attackCollider && c2->type == Collider::Type::ENEMY) {
		hitEnemy = true;
		App->audio->PlayFx(lethalAtt);
		hitEnemy = false;
	} 
	if (c1 == Pcollider && c2->type == Collider::Type::ENEMY && !destroyed && iFrames == false)
	{
		knockImpulse = 1.0f;
		iFrames = true;
		hp--;
		if (hp > 0){
			App->audio->PlayFx(loseHP);
		}

		position.y -= 0.1f;
		if (position.y < 190) {
			KnockBack();
			if(dir == Direction::LEFT){ 
				idle = false;
				currentAnimation = &knockBackLeft;
			}
			if (dir == Direction::RIGHT) {
				currentAnimation = &knockBackRight;
				idle = false;
			}
		}
		
		if (hp <= 0)
		{
			hp = 3;
			lives--;
		}

		if (lives <= 0)
		{
			hp = 0;
			//DEATH
			destroyed = true;

		}
		
		/*App->scene->ScreenScroll = false;*/
	}
	if (c1 == Pcollider &&	c2 == App->scene->backCamLimit) {
		while (position.x < (App->render->camera.x * 0.3333333333f -13.3333333333f)){
			position.x = App->render->camera.x * 0.3333333333f - 13.3333333333f;
		}
		
	}

	if (c1 == Pcollider && c2 == App->scene->frontCamLimit) {
		
		while (position.x > App->scene->aux-33.3333333333f) {
			position.x = App->scene->aux-33.3333333333f;
		}
		
	}

	if (c1->type == Collider::Type::PLAYER_SHOT && c2->type == Collider::Type::ENEMY)
	{
		kickCollider->SetPos(1000, 1000);
		attackCollider->SetPos(1000, 1000);
		score += 100;
	}
	
	

}

void ModulePlayer:: KnockBack() {

	airSt == AirState::AIRBORN;
	knockImpulse -= Gravity;
	position.y -= knockImpulse;

	position.x--;
	if (position.y >= 190) {
		airSt = AirState::GROUND;
		position.y = 190;
		idle = true;
		//jumpRight.Reset();
	}

}