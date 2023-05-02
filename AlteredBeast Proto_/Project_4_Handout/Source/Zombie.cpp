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
	IsExploding = false;
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
	bodyXplode.loop = true;
	bodyXplode.totalFrames = 4;
	bodyXplode.speed = 0.05f;

	Ecollider = App->collisions->AddCollider({ 400, 120, 24, 60 }, Collider::Type::ENEMY, (Module*)App->enemies);
	AttackCollider = App->collisions->AddCollider({ 412,140,50,60 }, Collider::Type::ENEMY_SHOT, (Module*)App->player);
	Range = App->collisions->AddCollider({ 450,150,60,60 }, Collider::ATTACK_RANGE, (Module*)App->enemies);
	SelfDestruct = App->collisions->AddCollider({ 5,5,5,5 }, Collider::ENEMY_SELF_DESTRUCT, (Module*)App->enemies);

	//attack starts oob
	AttackCollider->SetPos(-1000, -1000);
	SelfDestruct->SetPos(-1000, -1000);

	lethalAtt = App->audio->LoadFx("Assets/FX/Lethal_Punch");

}

void Zombie::Update() {
	
		position.x -= Zspeed;

		//if zombie is behind the player change the direction
		if (position.x < App->player->position.x) { dir = Direction::RIGHT; }
		else { dir = Direction::LEFT; }
	
		if (alive)
		{
			if (hp == 2 && attacking == false) { currentAnim = &walkAnim; }
			if (hp == 1 && hitByPlayer)
			{
				hitCountdown--;
				if (hitCountdown <= 0) {
					currentAnim = &headXplode;
					App->audio->PlayFx(lethalAtt, 3);
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
	//remove

	if (attacking == true) { Attack(); }
	



	Ecollider->SetPos(2000, 2000);

	Range->SetPos(position.x-10, position.y);

	currentAnim->Update();
	Enemy::Update();
}

void Zombie::OnCollision(Collider* collider) {

	if (collider->Collider::Type::PLAYER_SHOT) {
		
		
		destroyedCountdown--;
		if (destroyedCountdown <= 0) {
			hp-= App->player->attack;
			App->audio->PlayFx(lethalAtt, 5);
			hitByPlayer = true;
			destroyedCountdown = 20;
		}

	}
	if (hp <= 0 && attacking == false) {
		Ecollider->SetPos(-1000, -1000);
		alive = false;


		App->scene->EnemyCN = 1;
		App->scene->HasEnemyDied = true; 
		App->scene->enemyX = position.x;
		App->scene->enemyY = position.y;

		App->player->score += 100;
	}

	//make the explosion kill the zombie
	
	
}

void Zombie::Attack() {

	
		attackCnt--;
		if (attackCnt <= 0) {

			//when player is in range for 50 frames
			currentAnim = &bodyXplode;
		
			attacking = true;
			App->scene->EnemyAttacking = true;
			App->scene->enemyX = position.x;
			App->scene->enemyY = position.y;
			App->scene->EnemyCN = 1;
			XplodeCnt--;
			//IMPORTANT the explosionCnt of the collider must be in sync with that of the explosion found in scene.cpp
			Zspeed = 0;
			if (XplodeCnt <= 0) {
				if (exploded == false) {
					hp = 0;
					AttackCollider->SetPos(position.x - 10, position.y);
					SelfDestruct->SetPos(position.x + 3, position.y + 5);
					exploded = true;
					

					
				}

			}

		}
	

}

