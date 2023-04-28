
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
	// idle animation - just one sprite
	idleAnim.PushBack({ 66, 1, 32, 14 });

	// move upwards
	upAnim.PushBack({ 100, 1, 32, 14 });
	upAnim.PushBack({ 132, 0, 32, 14 });
	upAnim.loop = false;
	upAnim.speed = 0.1f;

	// Move down
	downAnim.PushBack({ 33, 1, 32, 14 });
	downAnim.PushBack({ 0, 1, 32, 14 });
	downAnim.loop = false;
	downAnim.speed = 0.1f;
}

ModulePower::~ModulePower()
{

}

bool ModulePower::Start()
{
	LOG("Loading player textures");

	bool ret = true;

	texture = App->textures->Load("Assets/Sprites/ship.png");
	//currentAnimation = &idleAnim;

	//Load asset

	position.x = 150;
	position.y = 120;

	

	// Add collider

	return ret;
}

update_status ModulePower::Update()
{
	// Moving the power with camera scroll , posx += 1
	

	

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