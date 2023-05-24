#include "Enemy.h"
#include "ModuleEnemies.h"
#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleRender.h"

Enemy::Enemy(int x, int y) : position(x, y)
{
	
	spawnPos = position;
	
}

Enemy::~Enemy()
{
	if (Ecollider != nullptr)
		Ecollider->pendingToDelete = true;
}

const Collider* Enemy::GetCollider() const
{
	return Ecollider;
}

const Collider* Enemy::GetXplosionTriggerCollider() const {
	return XplosionTrigger;
}

const Collider* Enemy::GetDestructCollider() const {
	return SelfDestruct;
}

void Enemy::Update()
{
	if (currentAnim != nullptr)
		currentAnim->Update();

	// why do we move the collider of all anemies ???
	if (Ecollider != nullptr )
		//CHANGE
		Ecollider->SetPos(position.x+7, position.y+7);
	//CHANGE acticate attack collider when exploding
	if (AttackCollider != nullptr) {
		/*AttackCollider->SetPos(position.x-10, position.y);*/
	}
}

void Enemy::Draw()
{
	
	if (currentAnim != nullptr)
		App->render->Blit(App->enemies->texture, position.x, position.y, &(currentAnim->GetCurrentFrame()));

}

void Enemy::OnCollision(Collider* collider)
{
	/*App->particles->AddParticle(App->particles->explosion, position.x, position.y);
	App->audio->PlayFx(destroyedFx);*/
}

void Enemy::Attack() {
	//This function will be overwritten
}

void Enemy::KnockBack() {

	/*if (knocked == false) {
		knocked = true;
		initialH;
		idle = false;
	}
	
		knockH -= Gravity;
		position.y -= knockH;

		
		if (position.y >= initialH) {
			position.y = initialH;
			idle = true;
			
		}*/
	
}