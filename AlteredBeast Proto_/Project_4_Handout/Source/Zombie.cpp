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
	walkAnim.loop = false;

	deathAnim.PushBack({ 168, 0, 49, 74 });
	deathAnim.loop = true;

	Ecollider = App->collisions->AddCollider({ 412, 140, 22, 60 }, Collider::Type::ENEMY, (Module*)App->enemies);

}

void Zombie::Update() {

	position.x -= 0.05f;

	if (alive == true) { currentAnim = &walkAnim; }

	if (alive == false) 
	{ 
		currentAnim = &deathAnim;
		destroyedCountdown--;
		if (destroyedCountdown <= 0) { destroyed = true; }
	
	}

	
	
	//if (!alive) {
	//	delete this;
	//}
	currentAnim->Update();
	Enemy::Update();

}

void Zombie::OnCollision(Collider* collider) {
	currentAnim = &deathAnim;
	alive = false;
	
}