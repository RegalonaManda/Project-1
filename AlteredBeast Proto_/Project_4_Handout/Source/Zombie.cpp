#include "Zombie.h"
#include "Application.h"
#include "ModuleCollisions.h"
#include "Enemy.h"
#include "ModuleEnemies.h"
#include "ModulePlayer.h"
#include "EnemyDeath.h"

//Calls the constructor of enemy class to save spawn position


Zombie::Zombie(int x, int y) : Enemy(x, y) {
	hp = 2;
	walkAnim.PushBack({1, 1, 41, 68 });
	walkAnim.PushBack({43, 1, 41, 68});
	walkAnim.speed = 0.008f;
	walkAnim.loop = true;

	//Default Direction
	dir = Direction::RIGHT;

	//at 1 hit, head explodes, continues
	//at 2 hits, disappears
	
	//if enemy gets close enough to player, will explode -> yellow sprite - normal sprite - disappears (no shake)

	headXplode.PushBack({85,1,41,68});
	headXplode.PushBack({132,1,41,68});
	headXplode.PushBack({127,1,41,68});
	headXplode.PushBack({132,1,41,68});
	headXplode.loop = false;
	headXplode.totalFrames = 4;
	headXplode.speed = 0.03f;

	headlessWalk.PushBack({ 127,1,41,68 });
	headlessWalk.PushBack({ 85,121,41,68 });
	headlessWalk.loop = true;
	headlessWalk.totalFrames = 2;
	headlessWalk.speed = 0.008f;

	deathAnim.PushBack({ 127,1,41,68 });
	deathAnim.PushBack({ 132,1,41,68 });
	deathAnim.PushBack({ 127,1,41,68 });
	deathAnim.PushBack({ 132,1,41,68 });
	deathAnim.loop = false;
	deathAnim.totalFrames = 4;
	deathAnim.speed = 0.03f;
	

	bodyXplode.PushBack({ 169,0,41,68 });
	bodyXplode.PushBack({ 127,0,41,68 });
	bodyXplode.PushBack({ 169,0,41,68 });
	bodyXplode.PushBack({ 127,0,41,68 });
	bodyXplode.loop = false;
	bodyXplode.totalFrames = 4;
	bodyXplode.speed = 0.05f;

	Ecollider = App->collisions->AddCollider({ 400, 120, 24, 60 }, Collider::Type::ENEMY, (Module*)App->enemies);
	AttackCollider = App->collisions->AddCollider({ 412,140,42,60 }, Collider::Type::ENEMY_SHOT, (Module*)App->player);
}

void Zombie::Update() {

	position.x -= 0.05f;

	//if zombie is behind the player change the direction
	if (position.x < App->player->position.x) { dir = Direction::RIGHT; }
	else { dir = Direction::LEFT; }

	if (alive) 
	{ 
		if(hp == 2){ currentAnim = &walkAnim; }
		if (hp == 1 && hitByPlayer)
		{
			hitCountdown--;
			if (hitCountdown <= 0) {
				currentAnim = &headXplode;
			}

			if (headXplode.HasFinished()) {
				currentAnim = &headlessWalk;
				hitByPlayer = false;

			}
		}
	}

	/*if (!alive) {
		currentAnim = &deathAnim;
		if (currentAnim->HasFinished()) {
			destroyed = true;
		}
	}*/


	currentAnim->Update();
	Enemy::Update();
}

void Zombie::OnCollision(Collider* collider) {

	if (collider->Collider::Type::PLAYER_SHOT) {
		
		
		destroyedCountdown--;
		if (destroyedCountdown <= 0) {
			hp-= App->player->attack;
			hitByPlayer = true;
			destroyedCountdown = 20;
		}
	
		if (hp <= 0) {
			Ecollider->SetPos(-1000, -1000);
			alive = false;

			App->scene->HasEnemyDied = true;
			App->scene->enemyX = position.x;
			App->scene->enemyY = position.y;
			
			
		}	
	}
}

void Zombie::Attack() {

	



}

