#include "ModulePlayer.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCollisions.h"

#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib")
#pragma comment( lib, "SDL/libx86/SDL2main.lib")

#include "SDL/include/SDL_scancode.h"

// Street Fighter reference at https://www.youtube.com/watch?v=OEhmUuehGOA

Uint32 startTime = 0;
bool idle = true;



ModulePlayer::ModulePlayer()
{
	position.x = 100;
	position.y = 200;

	//Default direction
	dir = Direction::RIGHT;
	//Default airstate
	airSt = AirState::GROUND;

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
	
	forwardAnim.speed = 0.07f;


	backAnim.PushBack({ 0,76,54,75 });
	backAnim.PushBack({ 56,76,54,75 });
	//backAnim.PushBack({ 111,76,54,75 });
	backAnim.speed = 0.07f;

	//Punch animation
	punchAnimRight.PushBack({ 0,151,54,75 });
	punchAnimRight.PushBack({ 56,151,54,75 });
	punchAnimRight.PushBack({ 110,151,111,75 });//Extra large frame for extended punch
	punchAnimRight.loop = false;
	punchAnimRight.totalFrames = 3;
	
	punchAnimRight.speed = 0.05f;


	punchAnimLeft.PushBack({ 385,227,54,75 });
	punchAnimLeft.PushBack({ 330,227,54,75 });
	punchAnimLeft.PushBack({ 261,227,73,75 });//Extra large frame for extended punch
	punchAnimLeft.loop = false;

	punchAnimLeft.speed = 0.05f;
	punchAnimLeft.totalFrames = 3;


	crouchAnimRight.PushBack({ 0, 226, 54, 75 });
	crouchAnimLeft.PushBack({});


	crouchAnimLeft.speed = 0.05f;
	crouchAnimLeft.totalFrames = 1;


	crouchPunchRight.PushBack({55, 226, 54, 75 });
	crouchPunchRight.PushBack({110, 226, 108, 75 });
	crouchPunchRight.loop = false;
	crouchPunchRight.totalFrames = 2;
	crouchPunchRight.speed = 0.05f;




	deathAnim.PushBack({ 209, 378, 108, 75 });

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

	return ret;
}

update_status ModulePlayer::Update()
{
	//Update Collider to current player pos, change it depending on direction and AirState
	if (dir == Direction::RIGHT && airSt == AirState::GROUND) { Pcollider->SetPos(position.x + 18, position.y - 65); }
	if (dir == Direction::LEFT && airSt == AirState::GROUND) { Pcollider->SetPos(position.x + 20, position.y - 65); }

	//Grounded
	if(dir == Direction::RIGHT && airSt == AirState::CROUCH) { Pcollider->SetPos(position.x + 20, position.y - 40); }

	//Pcollider->SetPos(position.x + App->render->camera.x + 15, position.y + App->render->camera.y - 65);

	//Reset the currentAnimation back to idle, either left ot right before updating the logic
	if (idle == true && dir == Direction::RIGHT && airSt == AirState::GROUND)
	{
		currentAnimation = &idleAnimRight;
	}
	if (idle == true && dir == Direction::LEFT && airSt == AirState::GROUND)
	{
		currentAnimation = &idleAnimLeft;
	}

	if (App->input->keys[SDL_SCANCODE_D] == KEY_REPEAT)
	{
		if (idle == true)/* Can't move if punching */ {
			//change direction
			dir = Direction::RIGHT;

			currentAnimation = &forwardAnim;
			position.x += speed;
		}
	}


	if (App->input->keys[SDL_SCANCODE_A] == KEY_REPEAT) {
		if (idle == true)/* Can't move if punching */ {
			//change direction
			dir = Direction::LEFT;

			currentAnimation = &backAnim;
			position.x -= speed;
		}
	}

	if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT && airSt != AirState::CROUCH) {

		airSt = AirState::CROUCH;

		if (dir == Direction::LEFT) {
			currentAnimation = &crouchAnimLeft;
		}
		if (dir == Direction::RIGHT) {
			currentAnimation = &crouchAnimRight;
		}

	}

	if (App->input->keys[SDL_SCANCODE_Z] == KEY_DOWN) {
		if (airSt == AirState::GROUND) {
			if (dir == Direction::LEFT) {
				punchAnimLeft.Reset();
				currentAnimation = &punchAnimLeft;
				//activate punch collider when player punches
				attackCollider->SetPos(position.x + 38, position.y - 60);

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
				attackCollider->SetPos(position.x + 38, position.y - 40);
				idle = false;
			}
			if (dir == Direction::RIGHT) {
				crouchPunchRight.Reset();
				currentAnimation = &crouchPunchRight;
				attackCollider->SetPos(position.x + 38, position.y - 40);
				idle = false;
			}
		}
	}

		//OUTSIDE THE IF
		if (punchAnimRight.HasFinished() == true) {
			punchAnimRight.loopCount--;   //VERY IMPORTANT , since HasFinished checks if the loop count has surpassed 0, after the animation has finished reset loop count
			idle = true;
			//deactivate punch collider
			attackCollider->SetPos(1000, 1000);
		}
		//OUTSIDE THE IF
		if (punchAnimLeft.HasFinished() == true) {
			punchAnimLeft.loopCount--;   //VERY IMPORTANT , since HasFinished checks if the loop count has surpassed 0, after the animation has finished reset loop count
			idle = true;
			//deactivate punch collider
			attackCollider->SetPos(1000, 1000);
		}

		


		if (App->input->keys[SDL_SCANCODE_S] == KEY_UP /*&& idle == true*/) {
			/*crouchPunchLeft.loopCount--;*/   //VERY IMPORTANT, since HasFinished checks if the loop count has surpassed 0, after the animation has finished, reset loop count
			
			airSt = AirState::GROUND;
			//deactivate punch collider
			attackCollider->SetPos(1000, 1000); //quick fix to make collider disappear from scene by sending it oob, TRY TO CHANGE
		}

		if (crouchPunchRight.HasFinished() == true) {
			crouchPunchRight.loopCount--;   //VERY IMPORTANT, since HasFinished checks if the loop count has surpassed 0, after the animation has finished, reset loop count
			idle = true;
			//deactivate punch collider
			attackCollider->SetPos(1000, 1000); //quick fix to make collider disappear from scene by sending it oob, TRY TO CHANGE
			if (airSt == AirState::CROUCH) {
				if (dir == Direction::LEFT) { currentAnimation = &crouchAnimLeft; }
				if (dir == Direction::RIGHT) { currentAnimation = &crouchAnimRight; }
			}
			
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

	return update_status::UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 == Pcollider && destroyed == false)
	{
		

		destroyed = true;
	}
}