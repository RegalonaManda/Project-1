#include "Zombie.h"
#include "Application.h"
#include "ModuleCollisions.h"
#include "Enemy.h"
#include "ModuleEnemies.h"
#include "ModulePlayer.h"

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
	headXplode.totalFrames;
	headXplode.speed = 0.05f;

	headlessWalk.PushBack({ 127,1,41,68 });
	headlessWalk.PushBack({ 85,121,41,68 });
	headlessWalk.loop = true;
	headlessWalk.totalFrames;
	headlessWalk.speed = 0.008f;

	deathAnim.PushBack({ 127,1,41,68 });
	deathAnim.PushBack({ 132,1,41,68 });
	deathAnim.PushBack({ 127,1,41,68 });
	deathAnim.PushBack({ 132,1,41,68 });
	deathAnim.loop = true;
	deathAnim.totalFrames;
	deathAnim.speed = 0.008f;
	

	bodyXplode.PushBack({ 169,0,41,68 });
	bodyXplode.PushBack({ 127,0,41,68 });
	bodyXplode.PushBack({ 169,0,41,68 });
	bodyXplode.PushBack({ 127,0,41,68 });
	bodyXplode.loop = false;
	bodyXplode.totalFrames;
	bodyXplode.speed = 0.05f;

	Ecollider = App->collisions->AddCollider({ 400, 120, 24, 60 }, Collider::Type::ENEMY, (Module*)App->enemies);
	AttackCollider = App->collisions->AddCollider({ 412,140,42,60 }, Collider::Type::ENEMY_SHOT, (Module*)App->player);
}

void Zombie::Update() {

	position.x -= 0.05f;

	//if zombie is behind the player change the direction
	if (position.x < App->player->position.x) { dir = Direction::RIGHT; }
	else { dir = Direction::LEFT; }
	//life 
	/*if (hp <= 0) {
		currentAnim = &deathAnim;
		alive = false;
	}*/

	if (alive) 
	{ 
		/*destroyedCountdown = 20;*/

		if(hp == 2 && !hitByPlayer)
		{ currentAnim = &walkAnim; }
		if (hp == 2 && hitByPlayer)
		{
			
			destroyedCountdown--;
			if (destroyedCountdown <= 0) {
				currentAnim = &headXplode;
				hitByPlayer = false;
				destroyedCountdown = 10;
				//currentAnim = &headlessWalk;
			}
			
			
		}
		if (hp == 1 && !hitByPlayer) 
		{ currentAnim = &headlessWalk; }
		if (hp == 1 && hitByPlayer) 
		{ 
			currentAnim = &deathAnim; 
			destroyedCountdown--;
			if (destroyedCountdown <= 0) {
				hitByPlayer = false;
				destroyedCountdown = 10;

			}
			
		}
		
		/*destroyedCountdown = 20;*/
	}

	if (!alive) 
	{ 
		destroyedCountdown--;
		if (destroyedCountdown <= 0) { destroyed = true; }
	
	}

	
	currentAnim->Update();
	Enemy::Update();

}

void Zombie::OnCollision(Collider* collider) {

	if (collider->Collider::Type::PLAYER_SHOT) {
		

		destroyedCountdown--;
		if (destroyedCountdown <= 0) {
			--hp;
			hitByPlayer = true;
			destroyedCountdown = 50;
		}
	
		
	
		if (hp <= 0) {
			Ecollider->SetPos(-1000, -1000);
			alive = false;
		}
		
		
		/*hitByPlayer = false;*/
		
	}

}

