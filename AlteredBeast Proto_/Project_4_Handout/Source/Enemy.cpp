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

void Enemy::Update()
{
	if (currentAnim != nullptr)
		currentAnim->Update();

	if (Ecollider != nullptr)
		Ecollider->SetPos(position.x, position.y);
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