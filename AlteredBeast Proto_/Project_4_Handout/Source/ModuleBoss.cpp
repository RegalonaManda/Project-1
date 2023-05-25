#include "ModuleBoss.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"
#include "ModuleBoss.h"

ModuleBoss::ModuleBoss(bool startEnabled) : Module(startEnabled)
{
	
	
}

ModuleBoss::~ModuleBoss()
{

}

bool ModuleBoss::Start()
{
	LOG("Loading power textures");

	App->audio->PlayMusic("Gaum_Boss.ogg", 0.0f);

	bool ret = true;
	beaten = false;
	texture = App->textures->Load("Assets/PowerUpGrid.png");

	deathExpl = App->audio->LoadFx("Boss_Death");
	welcomeDoom = App->audio->LoadFx("Welcome_To_Your_Doom");

	currentAnim = &idleAnim;

	//pattern[0].headAttack[0].finalX = 

	return ret;
}

update_status ModuleBoss::Update()
{
	currentAnim->Update();

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleBoss::PostUpdate()
{


	return update_status::UPDATE_CONTINUE;
}

void ModuleBoss::OnCollision(Collider* c1, Collider* c2)
{

}