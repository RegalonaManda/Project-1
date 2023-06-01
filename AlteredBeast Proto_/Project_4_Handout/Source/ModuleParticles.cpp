#include "ModuleParticles.h"

#include "Application.h"

#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCollisions.h"

#include "SDL/include/SDL_timer.h"

ModuleParticles::ModuleParticles(bool startEnabled) : Module(startEnabled)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		particles[i] = nullptr;
}

ModuleParticles::~ModuleParticles()
{

}

bool ModuleParticles::Start()
{
	LOG("Loading particles");
	texture = App->textures->Load("Assets/EnemiesProto.png");

	// Explosion particle
	/*zombieExpl.anim.PushBack({ 274, 296, 33, 30 });
	zombieExpl.anim.PushBack({ 313, 296, 33, 30 });
	zombieExpl.anim.PushBack({ 346, 296, 33, 30 });
	zombieExpl.anim.PushBack({ 382, 296, 33, 30 });
	zombieExpl.anim.PushBack({ 419, 296, 33, 30 });
	zombieExpl.anim.PushBack({ 457, 296, 33, 30 });
	zombieExpl.anim.loop = false;
	zombieExpl.anim.speed = 0.3f;*/

	/*wolfShot.anim.PushBack({ 1,115, 73, 65 });
	wolfShot.anim.PushBack({ 75,115, 73, 65 });
	wolfShot.anim.PushBack({ 149,115, 73, 65 });
	wolfShot.anim.PushBack({ 223,115, 73, 65 });*/

	zombie.anim.PushBack({ 212,1,63,68 });
	zombie.anim.PushBack({ 276,1,63,68 });
	zombie.anim.PushBack({ 340,1,63,68 });

	zombie.anim.PushBack({ 212,70,95,68 });
	zombie.anim.PushBack({ 308,70,95,68 });
	zombie.anim.PushBack({ 212,139,95,68 });
	zombie.anim.PushBack({ 308,139,95,68 });
	zombie.anim.loop = false;
	zombie.anim.speed = 0.18f;

	Skull.anim.PushBack({ 340,247,63,68 });
	Skull.anim.PushBack({ 404,247,63,68 });
	Skull.anim.PushBack({ 468,247,63,68 });
	Skull.anim.PushBack({ 340,316,95,68 });
	Skull.anim.PushBack({ 436,316,95,68 });
	Skull.anim.PushBack({ 340,385,95,68 });
	Skull.anim.PushBack({ 436,385,95,68 });
	Skull.anim.loop = false;
	Skull.anim.speed = 0.18f;


	return true;
}

update_status ModuleParticles::PreUpdate()
{
	// Remove all particles scheduled for deletion
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (particles[i] != nullptr && particles[i]->pendingToDelete)
		{
			delete particles[i];
			particles[i] = nullptr;
		}
	}

	return update_status::UPDATE_CONTINUE;
}

bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");

	// Delete all remaining active particles on application exit 
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (particles[i] != nullptr)
		{
			delete particles[i];
			particles[i] = nullptr;
		}
	}

	return true;
}

void ModuleParticles::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		// Always destroy particles that collide
		if (particles[i] != nullptr && particles[i]->collider == c1)
		{
			particles[i]->pendingToDelete = true;
			particles[i]->collider->pendingToDelete = true;
			break;
		}
	}
}

update_status ModuleParticles::Update()
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* particle = particles[i];

		if (particle == nullptr)	continue;

		// Call particle Update. If it has reached its lifetime, destroy it
		if (particle->Update() == false)
		{
			particles[i]->SetToDelete();
		}
	}

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleParticles::PostUpdate()
{
	//Iterating all particle array and drawing any active particles
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* particle = particles[i];

		if (particle != nullptr && particle->isAlive)
		{
			App->render->Blit(texture, particle->position.x, particle->position.y, &(particle->anim.GetCurrentFrame()));
		}
	}

	return update_status::UPDATE_CONTINUE;
}

Particle* ModuleParticles::AddParticle(const Particle& particle, int x, int y, Collider::Type colliderType, uint delay)
{
	Particle* newParticle = nullptr;

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		//Finding an empty slot for a new particle
		if (particles[i] == nullptr)
		{
			newParticle = new Particle(particle);
			newParticle->frameCount = -(int)delay;			// We start the frameCount as the negative delay
			newParticle->position.x = x;						// so when frameCount reaches 0 the particle will be activated
			newParticle->position.y = y;

			//Adding the particle's collider
			if (colliderType != Collider::Type::NONE)
				newParticle->collider = App->collisions->AddCollider(newParticle->anim.GetCurrentFrame(), colliderType, this);

			particles[i] = newParticle;
			break;
		}
	}

	return newParticle;
}