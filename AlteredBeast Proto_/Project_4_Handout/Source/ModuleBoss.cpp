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
	//Animations
	Anim.PushBack({ 1,1,31,31 });
	Anim.PushBack({ 33,1,31,31 });
	Anim.PushBack({ 65,1,31,31 });
	Anim.speed = 0.1f;

	DisappearAnim.PushBack({ 91,1,1,1 });
}

ModuleBoss::~ModuleBoss()
{

}

bool ModuleBoss::Start()
{
	LOG("Loading power textures");

	bool ret = true;
	beaten = false;
	texture = App->textures->Load("Assets/PowerUpGrid.png");
	//currentAnimation = &idleAnim;

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