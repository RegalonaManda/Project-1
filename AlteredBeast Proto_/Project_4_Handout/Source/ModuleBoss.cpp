#include "ModuleBoss.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"
#include "ModuleBoss.h"
#include "ModuleScene.h"

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

	return true;

}

update_status ModuleBoss::Update()
{
	if (initilized == false) {
		initilized = ModuleBoss::Initialize();
	}
	

	if (transform.currentFrame == 0.0f) {
		App->audio->PlayFx(welcomeDoom, -1);
	}

	if (transform.HasFinished()) {
		currentAnim = &idleAnim;
		App->audio->PlayMusic("Assets/Music/Gaum_Boss.ogg", 0.0f);
		transform.loopCount = 0;
		transformed = true;
	}
	colliderBoss->SetPos(position.x +20, position.y);

	if (transformed == true){

		attackCnt--;


		if (attackCnt <= 0) {

			ModuleBoss::Attack(pattern[0]);






		}





	}











	currentAnim->Update();

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleBoss::PostUpdate()
{
	
	SDL_Rect BossRec = currentAnim->GetCurrentFrame();
	App->render->Blit(texture, position.x, position.y, &BossRec);

	SDL_Rect Head = pattern[currentPattern_].headAttack[currentHead].current->GetCurrentFrame();
	App->render->Blit(texture, pattern[currentPattern_].headAttack[currentHead].positionX, pattern[currentPattern_].headAttack[currentHead].positionY, &Head);

	return update_status::UPDATE_CONTINUE;
}

void ModuleBoss::OnCollision(Collider* c1, Collider* c2)
{

}

void ModuleBoss::Attack(AttackPattern& Pattern) {

	if (currentHead < 6) {

		

		Pattern.headAttack[currentHead].Trajectory();

		if (Pattern.headAttack[currentHead].fallen == true) {
			currentHead++;
			attackCnt = 40;
		}
	}
}