
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

	return ret;
}

update_status ModulePower::Update()
{
	// Moving the power with camera scroll , posx += 1
	
	waveRatio += waveRatioSpeed;

	position.y = spawnPos.y + (waveHeight * sinf(waveRatio));
	position.x -= 1;
	

	//collider->SetPos(position.x, position.y);

	

	return update_status::UPDATE_CONTINUE;
}

update_status ModulePower::PostUpdate()
{
	/*if (!destroyed)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		App->render->Blit(texture, position.x, position.y, &rect);
	}*/
	SDL_Rect PowerRec = Anim.frames[0];
	App->render->Blit(texture, position.x, position.y, &PowerRec);

	return update_status::UPDATE_CONTINUE;
}

void ModulePower::OnCollision(Collider* c1, Collider* c2)
{
	/*if (c1 == collider && destroyed == false)
	{
		App->particles->AddParticle(App->particles->explosion, position.x, position.y, Collider::Type::NONE, 9);
		App->particles->AddParticle(App->particles->explosion, position.x + 8, position.y + 11, Collider::Type::NONE, 14);
		App->particles->AddParticle(App->particles->explosion, position.x - 7, position.y + 12, Collider::Type::NONE, 40);
		App->particles->AddParticle(App->particles->explosion, position.x + 5, position.y - 5, Collider::Type::NONE, 28);
		App->particles->AddParticle(App->particles->explosion, position.x - 4, position.y - 4, Collider::Type::NONE, 21);

		App->audio->PlayFx(explosionFx);

		

		destroyed = true;
	}*/
}