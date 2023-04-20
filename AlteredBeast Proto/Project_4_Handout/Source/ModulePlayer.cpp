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
	punchAnim.PushBack({ 0,151,54,75 });
	punchAnim.PushBack({ 56,151,54,75 });
	punchAnim.PushBack({ 110,151,111,75 });//Extra large frame for extended punch
	punchAnim.loop = false;
	punchAnim.totalFrames = 3;
	
	
	punchAnim.speed = 0.05f;

}

ModulePlayer::~ModulePlayer()
{

}

bool ModulePlayer::Start()
{
	LOG("Loading player textures");

	bool ret = true;

	texture = App->textures->Load("Assets/ABfullsprites.png"); // arcade version

	//Initialize collider
	Pcollider = App->collisions->AddCollider({ 100,300,20,68 }, Collider::Type::PLAYER, this);
	//CHANGE listener of attack to enemy
	attackCollider = App->collisions->AddCollider({ 100,300,33,19 }, Collider::Type::PLAYER_SHOT, this);
	return ret;
}

update_status ModulePlayer::Update()
{
	//Update Collider to current player pos, change it depending on direction
	if(dir == Direction::RIGHT){ Pcollider->SetPos(position.x + 18, position.y - 65); }
	if (dir == Direction::LEFT) { Pcollider->SetPos(position.x + 20, position.y - 65); }
	
	//Pcollider->SetPos(position.x + App->render->camera.x + 15, position.y + App->render->camera.y - 65);

	//Reset the currentAnimation back to idle, either left ot right before updating the logic
	if (idle == true && dir == Direction::RIGHT)
	{ currentAnimation = &idleAnimRight; }
	if (idle == true && dir == Direction::LEFT) 
	{ currentAnimation = &idleAnimLeft; }

	if(App->input->keys[SDL_SCANCODE_D] == KEY_REPEAT)
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

	//Punch
	if (App->input->keys[SDL_SCANCODE_Z] == KEY_DOWN) {
		punchAnim.Reset();
		currentAnimation = &punchAnim;
		//activate punch collider when player punches
		attackCollider->SetPos(position.x+38, position.y-60);
		
		idle = false;
		
	}
	//OUTSIDE THE IF
	if ( punchAnim.HasFinished() == true) {
		punchAnim.loopCount--;   //VERY IMPORTANT , since HasFinished checks if the loop count has surpassed 0, after the animation has finished reset loop count
		idle = true;
		//deactivate punch collider
		attackCollider->SetPos(1000, 1000);
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