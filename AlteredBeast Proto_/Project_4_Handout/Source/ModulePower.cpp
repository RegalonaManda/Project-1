
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
}

ModulePower::~ModulePower()
{

}

bool ModulePower::Start()
{
	LOG("Loading power textures");

	bool ret = true;

	texture = App->textures->Load("Assets/PowerUpGrid.png");
	//currentAnimation = &idleAnim;

	spawnPos.x = 400;
	spawnPos.y = 110;

	position.x = 400;
	position.y = 110;
	

	// Add collider
	SDL_Rect HitBox = { 1,1,20,20 };
	collider = App->collisions->AddCollider(HitBox, Collider::Type::POWER_UP, (Module*)App->player);
	return ret;
}

update_status ModulePower::Update()
{
	// Moving the power with camera scroll , posx += 1
	
	waveRatio += waveRatioSpeed;

	position.y = spawnPos.y + (waveHeight * sinf(waveRatio));
	
	position.x -= 1;
	
	collider->SetPos(position.x +6, position.y+6);
	//collider->SetPos(position.x, position.y);

	Anim.Update();

	return update_status::UPDATE_CONTINUE;
}

update_status ModulePower::PostUpdate()
{
	/*if (!destroyed)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		App->render->Blit(texture, position.x, position.y, &rect);
	}*/
	SDL_Rect PowerRec = Anim.GetCurrentFrame();
	App->render->Blit(texture, position.x, position.y, &PowerRec);

	return update_status::UPDATE_CONTINUE;
}

void ModulePower::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 == collider && gotten == false || c2 == collider)
	{
		

		//Kai pon aqui el power up sound

		this->Disable();
		//destroyed = true;
	}
}