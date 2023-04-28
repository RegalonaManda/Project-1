#include "Zombie.h"
#include "Application.h"
#include "ModuleCollisions.h"
#include "Enemy.h"
#include "ModuleEnemies.h"
#include "ModulePlayer.h"

//Calls the constructor of enemy class to save spawn position


Zombie::Zombie(int x, int y) : Enemy(x, y) {

	walkAnim.PushBack({11, 8, 27, 61 });
	walkAnim.PushBack({55, 9, 27, 61});
	walkAnim.speed = 0.008f;
	walkAnim.loop = true;

	//Default Direction
	dir = Direction::RIGHT;

	deathAnim.PushBack({ 168, 0, 49, 74 });

	deathAnim.loop = true;

	Ecollider = App->collisions->AddCollider({ 412, 140, 22, 60 }, Collider::Type::ENEMY, (Module*)App->enemies);
	AttackCollider = App->collisions->AddCollider({ 412,140,42,60 }, Collider::Type::ENEMY_SHOT, (Module*)App->player);
}

void Zombie::Update() {

	position.x -= 0.05f;

	//if zombie is behind the player change the direction
	if (position.x < App->player->position.x) { dir = Direction::RIGHT; }
	else { dir = Direction::LEFT; }
	//life 
	if (hp <= 0) {
		currentAnim = &deathAnim;
		alive = false;
	}

	if (alive == true) { currentAnim = &walkAnim; }

	if (alive == false) 
	{ 
		currentAnim = &deathAnim;
		destroyedCountdown--;
		if (destroyedCountdown <= 0) { destroyed = true; }
	
	}

	
	currentAnim->Update();
	Enemy::Update();

}

void Zombie::OnCollision(Collider* collider) {
	--hp;
	//currentAnim = &deathAnim;
	//alive = false;
	//
}