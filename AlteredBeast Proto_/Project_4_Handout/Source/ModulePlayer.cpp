#include "ModulePlayer.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCollisions.h"
#include "ModuleFonts.h"

#include <stdio.h>

#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib")
#pragma comment( lib, "SDL/libx86/SDL2main.lib")

#include "SDL/include/SDL_scancode.h"
//the lower the higher
#define MAX_HEIGHT 147
#define WALKANIMSPEED 0.08f
#define PUNCHANIMSPEED 0.1f
#define Gravity 0.07f;


//deberia estar en el .h
Uint32 startTime = 0;
bool idle = true;



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
	crouchAnimLeft.PushBack({386,303,54,75});


	crouchAnimLeft.speed = 0.05f;
	crouchAnimLeft.totalFrames = 1;


	crouchPunchRight.PushBack({55, 226, 54, 75 });
	crouchPunchRight.PushBack({110, 226, 108, 75 });
	crouchPunchRight.loop = false;
	crouchPunchRight.totalFrames = 2;
	crouchPunchRight.speed = 0.05f;

	crouchPunchLeft.PushBack({ 331, 303, 54, 75 });
	crouchPunchLeft.PushBack({ 253, 303, 63, 75 });
	crouchPunchLeft.loop = false;
	crouchPunchLeft.totalFrames = 2;
	crouchPunchLeft.speed = 0.05f;


	kickAnimRight.PushBack({221, 379, 87, 75});
	kickAnimRight.PushBack({309, 379, 87, 75 });
	kickAnimRight.PushBack({397, 379, 87, 75 });
	kickAnimRight.PushBack({309, 379, 87, 75 });

	kickAnimRight.speed = 0.1f;
	kickAnimRight.loop = false;
	kickAnimRight.totalFrames = 4;


	kickAnimLeft.PushBack({ 221, 455, 87, 75 });
	kickAnimLeft.PushBack({ 309, 455, 87, 75 });
	kickAnimLeft.PushBack({ 397, 455, 87, 75 });
	kickAnimLeft.PushBack({ 309, 455, 87, 75 });

	kickAnimLeft.speed = 0.1f;
	kickAnimLeft.loop = false;
	kickAnimLeft.totalFrames = 4;

	kickCrouchRight.PushBack({ 1,455,72,75 });
	kickCrouchRight.PushBack({ 74,455,72,75 });
	kickCrouchRight.PushBack({ 1,455,72,75 });
	kickCrouchRight.speed = 0.05f;
	kickCrouchRight.totalFrames = 3;
	kickCrouchRight.loop = false;

	kickCrouchLeft.PushBack({ 1,531,72,75 });
	kickCrouchLeft.PushBack({ 74,531,72,75 });
	kickCrouchLeft.PushBack({ 1,531,72,75 });
	kickCrouchLeft.speed = 0.05f;
	kickCrouchLeft.totalFrames = 3;
	kickCrouchLeft.loop = false;


	deathAnim.PushBack({ 266,531,72,75 });
	deathAnim.PushBack({ 339,531,72,75 });
	deathAnim.PushBack({ 412,531,72,75 });
	deathAnim.speed = 0.05f;
	deathAnim.totalFrames = 3;
	deathAnim.loop = false;
	
	jumpAnim.PushBack({ 0,303,54,75 });
	jumpAnim.speed = 0.05f;
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

	// TODO 0: Notice how a font is loaded and the meaning of all its arguments 
	char lookupTable[] = { "abcdefghijklmnopqrstuvwxyz0123456789.,ç'?!*$%&()+-/<=>© " };
	scoreFont = App->fonts->Load("Assets/font_spritesheet.png", lookupTable, 1);

	return ret;
}

update_status ModulePlayer::Update()
{
	//Update Collider to current player pos, change it depending on direction and AirState
	if (dir == Direction::RIGHT && airSt == AirState::GROUND) { Pcollider->SetPos(position.x + 18, position.y - 65); }
	if (dir == Direction::LEFT && airSt == AirState::GROUND) { Pcollider->SetPos(position.x + 20, position.y - 65); }

	//Grounded
	if(dir == Direction::RIGHT && airSt == AirState::CROUCH) { Pcollider->SetPos(position.x + 20, position.y - 40); }

	//Jump function
	//impulse bigger, the stronger it jumps
	if (idle == true && airSt == AirState::GROUND && App->input->keys[SDL_SCANCODE_SPACE] == KEY_DOWN) {
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
	if (position.y >= 190) {
		airSt = AirState::GROUND;
		position.y = 190;
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
		currentAnimation = &jumpAnim;
	}
	if (App->input->keys[SDL_SCANCODE_D] == KEY_REPEAT )
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


	if (App->input->keys[SDL_SCANCODE_A] == KEY_REPEAT) {
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

	if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT && airSt != AirState::CROUCH && idle == true) {

		airSt = AirState::CROUCH;

		if (dir == Direction::LEFT) {
			currentAnimation = &crouchAnimLeft;
		}
		if (dir == Direction::RIGHT) {
			currentAnimation = &crouchAnimRight;
		}

	}

	if (App->input->keys[SDL_SCANCODE_Z] == KEY_DOWN) {
		if (airSt == AirState::GROUND || airSt== AirState::AIRBORN) {
			if (dir == Direction::LEFT) {
				punchAnimLeft.Reset();
				currentAnimation = &punchAnimLeft;
				//activate punch collider when player punches
				attackCollider->SetPos(position.x + 0, position.y - 60);

				idle = false;

			}
			if (dir == Direction::RIGHT) {
				punchAnimRight.Reset();
				currentAnimation = &punchAnimRight;
				//activate punch collider when player punches
				attackCollider->SetPos(position.x + 38, position.y - 60);

				idle = false;

			}
		}
		if (airSt == AirState::CROUCH) {
			if (dir == Direction::LEFT) {
				crouchPunchLeft.Reset();
				currentAnimation = &crouchPunchLeft;
				//CHANGE x
				attackCollider->SetPos(position.x + 0, position.y - 40);
				idle = false;
			}
			if (dir == Direction::RIGHT) {
				crouchPunchRight.Reset();
				currentAnimation = &crouchPunchRight;
				attackCollider->SetPos(position.x + 43, position.y - 40);
				idle = false;
			}
		}
	}

	if (App->input->keys[SDL_SCANCODE_X] == KEY_DOWN) {
		if (idle == true && dir == Direction::RIGHT && airSt == AirState::GROUND) {
			kickAnimRight.Reset();
			currentAnimation = &kickAnimRight;
			idle = false;
		}
		if (idle == true && dir == Direction::LEFT && airSt == AirState::GROUND)
		{
			kickAnimLeft.Reset();
			currentAnimation = &kickAnimLeft;
			idle = false;
		}
		if (idle == true && dir == Direction::RIGHT && airSt == AirState::CROUCH) {
			kickCrouchRight.Reset();
			currentAnimation = &kickCrouchRight;
			idle = false;
		}
		if (idle == true && dir == Direction::LEFT && airSt == AirState::CROUCH) {
			kickCrouchLeft.Reset();
			currentAnimation = &kickCrouchLeft;
			idle = false;
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
			//deactivate kick collider
			attackCollider->SetPos(1000, 1000);
		}

		if (kickCrouchRight.HasFinished() == true) {
			kickCrouchRight.loopCount--;
			idle = true;
			//deactivate kick collider
			attackCollider->SetPos(1000, 1000);
		}


		if (App->input->keys[SDL_SCANCODE_S] == KEY_UP && idle == true) {
			/*crouchPunchLeft.loopCount--;*/   //VERY IMPORTANT, since HasFinished checks if the loop count has surpassed 0, after the animation has finished, reset loop count
			
			airSt = AirState::GROUND;
			//deactivate punch collider
			attackCollider->SetPos(1000, 1000); //quick fix to make collider disappear from scene by sending it oob, TRY TO CHANGE
		}

		

		//Player gets killed
		if (destroyed) {
			idle = false;
			currentAnimation = &deathAnim;
			destroyedCountdown--;
			if (destroyedCountdown <= 0) { return update_status::UPDATE_STOP; }
		}


		currentAnimation->Update();

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
	if (c1 == Pcollider && destroyed == false)
	{
		destroyed = true;
	}

	if (c1->type == Collider::Type::PLAYER_SHOT && c2->type == Collider::Type::ENEMY)
	{
		score += 100;
	}

}