
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"
#include "ModulePower.h"

ModulePower::ModulePower(bool startEnabled) : Module(startEnabled)
{
	//Animations
	Anim.PushBack({ 1,1,31,31 });
	Anim.PushBack({ 33,1,31,31 });
	Anim.PushBack({ 65,1,31,31 });
	Anim.speed = 0.1f;

	DissapearAnim.PushBack({ 91,1,1,1 });
}

ModulePower::~ModulePower()
{

}

bool ModulePower::Start()
{
	LOG("Loading power textures");

	bool ret = true;
	gotten = false;
	texture = App->textures->Load("Assets/PowerUpGrid.png");
	//currentAnimation = &idleAnim;

	/*spawnPos.x = 400;
	spawnPos.y = 110;

	position.x = 400;
	position.y = 110;*/
	

	// Add collider
	SDL_Rect HitBox = { 1,1,20,20 };
	collider = App->collisions->AddCollider(HitBox, Collider::Type::POWER_UP, (Module*)App->player);
	return ret;
}

update_status ModulePower::Update()
{
	// Moving the power with camera scroll , posx += 1

	if (gotten == false) {
		currentAnim = &Anim;
	}
	else {
		currentAnim = &DissapearAnim;
	}
	
	waveRatio += waveRatioSpeed;

	position.x = spawnPos.x + (waveHeight * sinf(waveRatio));
	
	position.y -= 0.4;
	
	collider->SetPos(position.x +6, position.y+6);
	//collider->SetPos(position.x, position.y);

	currentAnim->Update();

	return update_status::UPDATE_CONTINUE;
}

update_status ModulePower::PostUpdate()
{
	/*if (!destroyed)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		App->render->Blit(texture, position.x, position.y, &rect);
	}*/



	SDL_Rect PowerRec = currentAnim->GetCurrentFrame();
	App->render->Blit(texture, position.x, position.y, &PowerRec);

	return update_status::UPDATE_CONTINUE;
}

void ModulePower::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 == collider && gotten == false || c2 == collider)
	{
		

		//destroyed = true;
	}
}

bool ModulePower::CleanUp() {

	/*LOG("Freeing power up texture");
	App->textures->Unload(texture);*/

	collider->SetPos(-300, 4000);

	return true;
}