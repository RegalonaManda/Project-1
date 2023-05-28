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
	LOG("Loading Boss textures");

	App->audio->PlayMusic("Gaum_Boss.ogg", 0.0f);

	bool ret = true;
	beaten = false;
	texture = App->textures->Load("Assets/Boss spsheet Proto.png");

	deathExpl = App->audio->LoadFx("Boss_Death");
	welcomeDoom = App->audio->LoadFx("Welcome_To_Your_Doom");

	

	idleAnim.PushBack({ 19,1190, 131,156 });
	idleAnim.loop = true;



	//pattern[0].headAttack[0].finalX = 

	return ret;
}

update_status ModuleBoss::Update()
{
	currentAnim = &idleAnim;
	currentAnim->Update();

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleBoss::PostUpdate()
{
	SDL_Rect BossRec = currentAnim->GetCurrentFrame();
	App->render->Blit(texture, position.x, position.y, &BossRec);


	return update_status::UPDATE_CONTINUE;
}

void ModuleBoss::OnCollision(Collider* c1, Collider* c2)
{

}