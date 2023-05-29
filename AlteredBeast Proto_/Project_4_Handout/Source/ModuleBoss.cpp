#include "ModuleBoss.h"
#include <time.h>
#include <stdlib.h>
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"
#include "ModuleScene.h"
#include "ModuleGreyScene.h"
#include "ModulePlayer.h"

ModuleBoss::ModuleBoss(bool startEnabled) : Module(startEnabled)
{
	
	
}

ModuleBoss::~ModuleBoss()
{

}

int  i = 0;
bool initilized = false;

bool ModuleBoss::Start()
{
	LOG("Loading Boss textures");

	

	bool ret = true;
	beaten = false;
	texture = App->textures->Load("Assets/Boss spsheet Proto.png");

	deathExpl = App->audio->LoadFx("Assets/FX/Boss_Death.wav");
	welcomeDoom = App->audio->LoadFx("Assets/FX/Welcome_To_Your_Doom.wav");

	//SDL_Rect initialframe = { 1185,744,62,144 };

	transform.PushBack({1185,744,62,144});
	transform.PushBack({ 996,744,62,144 });
	transform.PushBack({ 807,744,62,144 });
	transform.PushBack({ 618,744,62,144 });
	transform.PushBack({ 429,744,62,144 });
	transform.PushBack({ 240,744,62,144 });
	transform.PushBack({1185,889,62,144 });
	transform.PushBack({ 996,889,62,144 });
	transform.PushBack({ 807,889,62,144 });
	transform.PushBack({ 618,889,62,144 });
	transform.PushBack({ 429,889,62,144 });
	transform.PushBack({ 240,889,62,144 });
	transform.speed = 0.12f;
	transform.loop = false;

	idleAnim.PushBack({ 19,1190, 131,156 });
	idleAnim.PushBack({ 151,1190, 131,156 });
	idleAnim.speed = 0.035f;

	idleAnim.loop = true;

	attackAnim.PushBack({307,1190, 131, 156});
	attackAnim.PushBack({ 439,1190, 131,156 });
	attackAnim.PushBack({ 571,1190, 131,156 });
	attackAnim.PushBack({ 703,1190, 131,156 });
	attackAnim.PushBack({ 835,1190, 131,156 });
	attackAnim.PushBack({ 967,1190, 131,156 });
	attackAnim.PushBack({ 1099,1190, 131,156 });
	attackAnim.speed = 0.3f;
	attackAnim.loop = false;

	App->scene->ScreenScroll = false;

	colliderBoss = App->collisions->AddCollider({ 0,0,85,150 }, Collider::Type::ENEMY, (Module*)App->bossfight);

	//pattern[0].headAttack[0].finalX = 

	currentAnim = &transform;






	return ret;
}


bool ModuleBoss::Initialize() {

	// Define attack Patterns
	for (int i = 0; i < 4; ++i) {
 		for (int j = 0; j < 6; ++j) {
			pattern[i].headAttack[j].Start();
			pattern[i].headAttack[j].positionX = position.x;
			pattern[i].headAttack[j].positionY = position.y;
		}
	}


	pattern[0].headAttack[0].FinalX = position.x - 85;
	pattern[0].headAttack[1].FinalX = position.x - 196;
	pattern[0].headAttack[2].FinalX = position.x - 19;
	pattern[0].headAttack[3].FinalX = position.x - 148;
	pattern[0].headAttack[4].FinalX = position.x - 70;
	pattern[0].headAttack[5].FinalX = position.x - 220;


	pattern[1].headAttack[0].FinalX = position.x - 81;
	pattern[1].headAttack[1].FinalX = position.x - 14;
	pattern[1].headAttack[2].FinalX = position.x - 163;
	pattern[1].headAttack[3].FinalX = position.x - 56;
	pattern[1].headAttack[4].FinalX = position.x - 211;
	pattern[1].headAttack[5].FinalX = position.x - 121;

	pattern[2].headAttack[0].FinalX = position.x - 9;
	pattern[2].headAttack[1].FinalX = position.x - 109;
	pattern[2].headAttack[2].FinalX = position.x - 204;
	pattern[2].headAttack[3].FinalX = position.x - 31;
	pattern[2].headAttack[4].FinalX = position.x - 148;
	pattern[2].headAttack[5].FinalX = position.x - 60;


	pattern[3].headAttack[0].FinalX = position.x - 52;
	pattern[3].headAttack[1].FinalX = position.x - 8;
	pattern[3].headAttack[2].FinalX = position.x - 142;
	pattern[3].headAttack[3].FinalX = position.x - 60;
	pattern[3].headAttack[4].FinalX = position.x - 200;
	pattern[3].headAttack[5].FinalX = position.x - 98;

	srand(time(NULL));

	return true;

}

update_status ModuleBoss::Update()
{
	if (initilized == false) {
		initilized = ModuleBoss::Initialize();
	}
	
	if (!beaten) {
		if (transform.currentFrame == 0.0f) {
			App->audio->PlayFx(welcomeDoom, -1);
		}

		if (transform.HasFinished()) {
			App->grey_scene->Grey = true;
			currentAnim = &idleAnim;
			App->audio->PlayMusic("Assets/Music/Gaum_Boss.ogg", 0.0f);
			transform.loopCount = 0;
			transformed = true;
		}
		colliderBoss->SetPos(position.x + 20, position.y);

		if (attackAnim.HasFinished()) {
			currentAnim->Reset();
			currentAnim->loopCount = 0;
			currentAnim = &idleAnim;
		}

		if (transformed == true) {

			attackCnt--;


			if (attackCnt <= 0) {
				if (selected == false) {
					RandID = rand() % 4;
					selected = true;
				}
				attackFinished = ModuleBoss::Attack(pattern[RandID]);
				if (attackFinished == true) {
					attackCnt = 1;

					//Reset Heads
					for (int i = 0; i < 6; ++i) {
						pattern[RandID].headAttack[i].positionX = position.x;
						pattern[RandID].headAttack[i].positionY = position.y;
						pattern[RandID].headAttack[i].hurtCollider->SetPos(-7500, -7500);
						pattern[RandID].headAttack[i].headCollider->SetPos(-7500, -7500);

						pattern[RandID].headAttack[i].current->Reset();
						pattern[RandID].headAttack[i].current->loopCount = 0;
						pattern[RandID].headAttack[i].fallen = false;

						pattern[RandID].headAttack[i].acceleration = 0.1f;
						pattern[RandID].headAttack[i].deacceleration = 1.5f;

					}

					pattern[RandID].activeHeads = 0;
					attackFinished = false;
					selected = false;
				}
			}

		}
	}

	else if (beaten) {


	}

	currentAnim->Update();

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleBoss::PostUpdate()
{
	
	SDL_Rect BossRec = currentAnim->GetCurrentFrame();
	App->render->Blit(texture, position.x, position.y, &BossRec);


	/*SDL_Rect Head = pattern[currentPattern_].headAttack[currentHead].current->GetCurrentFrame();
	App->render->Blit(texture, pattern[currentPattern_].headAttack[currentHead].positionX, pattern[currentPattern_].headAttack[currentHead].positionY, &Head);*/
	for (int i = 0; i < pattern[RandID].activeHeads; ++i) {
		SDL_Rect Head = pattern[RandID].headAttack[i].current->GetCurrentFrame();
		App->render->Blit(texture, pattern[RandID].headAttack[i].positionX, pattern[RandID].headAttack[i].positionY, &Head);
	}

	return update_status::UPDATE_CONTINUE;
}

void ModuleBoss::OnCollision(Collider* c1, Collider* c2)
{
	for (int i = 0; i < pattern[RandID].activeHeads; ++i) {
		if (c1 == pattern[RandID].headAttack[i].headCollider && c2->type == Collider::Type::PLATFORM) {

			if (pattern[RandID].headAttack[i].fallen == false) {
				pattern[RandID].headAttack[i].fallen = true;
				// Make head Explode
				c1->SetPos(-7500, -7500);
				pattern[RandID].headAttack[i].hurtCollider->SetPos(-7500, -7500);
				pattern[RandID].headAttack[i].current = &pattern[RandID].headAttack[i].XplodeAnim;
				currentHead++;
			}

		}
		if (c1 == pattern[RandID].headAttack[i].hurtCollider && c2->type == Collider::Type::PLAYER_SHOT) {
			pattern[RandID].headAttack[i].fallen = true;
			pattern[RandID].headAttack[i].headCollider->SetPos(-7500, -7500);
			c1->SetPos(-7500, -7500);
			pattern[RandID].headAttack[i].current = &pattern[RandID].headAttack[i].XplodeAnim;
			currentHead++;


		}
	}

	if (c1 == colliderBoss && c2->type == Collider::Type::PLAYER_SHOT) {
		if (destroyedCountdown == 20) { 
			hp -= App->player->attack;
			if (hp <= 0) { beaten = true; }
		}
		destroyedCountdown--;
		if (destroyedCountdown <= 0) {

			// play sound effect
			destroyedCountdown = 20;

		}
	}

}

bool ModuleBoss::Attack(AttackPattern& Pattern) {

	
	if (Pattern.activeHeads < 6) {
		cooldown--;
		if (cooldown <= 10) {
			currentAnim = &attackAnim;
		}
		if (cooldown <= 0) {
			cooldown = 20;
			// throw new head
			LOG("Spawning Head");
			Pattern.activeHeads++;
		}
	}
	
	else if (Pattern.headAttack[5].fallen == true && Pattern.headAttack[5].XplodeAnim.HasFinished()) {
		/*Pattern.headAttack[5].XplodeAnim.Reset();
		Pattern.headAttack[5].XplodeAnim.loopCount = 0;*/
		return true;
	}
	
		for (int i = 0; i < Pattern.activeHeads; ++i) {
			Pattern.headAttack[i].Trajectory();
			Pattern.headAttack[i].current->Update();
			//Pattern.headAttack[i].Draw();
			
		}

		/*if (Pattern.headAttack[currentHead].fallen == true) {
			currentHead++;
			attackCnt = 40;
		}*/
		return false;
}