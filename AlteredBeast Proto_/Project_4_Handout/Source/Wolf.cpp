#include "Wolf.h"
#include "Application.h"
#include "ModuleCollisions.h"
#include "Enemy.h"
#include "ModuleEnemies.h"
#include "ModulePlayer.h"
#include "ModulePower.h"

//Calls the constructor of enemy class to save spawn position


Wolf::Wolf(int x, int y) : Enemy(x, y) {

	destroyedCountdown = 10;
	hp = 1;

	//Default Direction
	dir = Direction::RIGHT;

	//Animations
	idleAnimRight.PushBack({ 1,70,66,50 });
	idleAnimRight.speed = 0.08f;
	idleAnimRight.loop = true;
	idleAnimRight.totalFrames = 1;

	idleAnimLeft.PushBack({ 547,70,66,50 });
	idleAnimLeft.speed = 0.08f;
	idleAnimLeft.loop = true;
	idleAnimLeft.totalFrames = 1;

	jumpAnimRight.PushBack({ 68,70,66,50 });
	jumpAnimRight.PushBack({ 135,70,67,50 });
	jumpAnimRight.speed = 0.05f;
	jumpAnimRight.loop = false;
	jumpAnimRight.totalFrames = 2;

	jumpAnimLeft.PushBack({ 480,70,66,50 });
	jumpAnimLeft.PushBack({ 412,70,67,50 });
	jumpAnimLeft.speed = 0.008f;
	jumpAnimLeft.loop = false;
	jumpAnimLeft.totalFrames = 2;

	
	deathAnimRight.PushBack({ 1,121,33,50 });
	deathAnimRight.PushBack({ 34,121,33,50 });
	deathAnimRight.speed = 0.008f;
	deathAnimRight.loop = false;
	deathAnimRight.totalFrames = 2;

	deathAnimLeft.PushBack({ 446,121,33,50 });
	deathAnimLeft.PushBack({ 413,121,33,50 });
	deathAnimLeft.speed = 0.008f;
	deathAnimLeft.loop = false;
	deathAnimLeft.totalFrames = 2;

	Ecollider = App->collisions->AddCollider({ 600, 190, 68, 70 }, Collider::Type::ENEMY, (Module*)App->enemies);
	AttackCollider = App->collisions->AddCollider({ 600, 200, 34, 30 }, Collider::Type::ENEMY_SHOT, (Module*)App->player);
}

void Wolf::Update() {


	//if wolf is behind the player change the direction
	if (position.x < App->player->position.x) 
	{ dir = Direction::LEFT; }
	else { dir = Direction::RIGHT; }

	if (alive)
	{
		
		if (dir == Direction::RIGHT ) 
		{
			if (idle == true) { 
				currentAnim = &idleAnimRight;
			    JumpCnt--;
				AttackCollider->SetPos(-2000, -2000);
			}
			
			if (JumpCnt <= 0) {
				idle = false;
				JumpCnt = 100;
			}

			if (idle == false) {
				AttackCollider->SetPos(position.x-5, position.y+10);
				currentAnim = &jumpAnimRight;
				position.x -= 1.0f;
				wolfImpulse -= Gravity;
				position.y -= wolfImpulse;

				if (position.y > 140) {
					position.y = 140;
					idle = true;
					wolfImpulse = 2.0f;
				}
			}
			
			/*if (JumpCnt <= 0) {

				idle = false;
				currentAnim = &jumpAnimRight;
				jump = true;
				JumpCnt = 300;
			}

			if (jumpAnimRight.HasFinished() == true && idle == false) {
				jumpAnimRight.Reset();
				jump = false;
				jumpAnimRight.loopCount = 0;
				idle = true;
			}

			if (jump == true) {
				wolfImpulse -= Gravity;
				position.y -= wolfImpulse;

			}

			if (position.y > 140 && jump == true) {
				position.y = 140;
				wolfImpulse = 3.2f;
				idle = true;
				jump = false;
			}*/
		}

		
		

		/*if (idle == false) {
			position.x += 0.7f;
			wolfImpulse -= Gravity;
			position.y -= wolfImpulse;
			if (position.y >= 140) {
				position.y = 140;

			}
		}*/
		 
		
		

		if (dir == Direction::LEFT) 
		{ 
			currentAnim = &idleAnimLeft; 
			/*if (idleAnimLeft.HasFinished()) {
				
				currentAnim = &jumpAnimLeft;

				position.x -= 0.7f;
				wolfImpulse -= Gravity;
				position.y -= wolfImpulse;

				if (position.y >= 140) {
					position.y = 140;
					
				}
			}*/
		}

		Enemy::Update();
		/*Enemy::Draw();*/

	}

	if (!alive) {
		/*App->powers->Enable();
		App->powers->position.x = position.x;
		App->powers->position.y = position.y;
		destroyed = true;*/

		if (dir == Direction::RIGHT) { currentAnim = &deathAnimRight; }
		if (dir == Direction::LEFT) { currentAnim = &deathAnimLeft; }

		if (currentAnim->HasFinished() == false) {
			destroyed = false;
		}
	}


	
}

void Wolf::OnCollision(Collider* collider) {

	if (collider->Collider::Type::PLAYER_SHOT) {


		//destroyedCountdown--;
		//if (destroyedCountdown <= 0) {
		//	hp -= App->player->attack;
		//	hitByPlayer = true;
		//	destroyedCountdown = 20;
		//}

		//if (hp <= 0) {

		    hp--;

			Ecollider->SetPos(-3000, -3000);
			alive = false;

			App->scene->HasEnemyDied = true;
			App->scene->enemyX = position.x;
			App->scene->enemyY = position.y;

			App->powers->Enable();
			App->powers->position.x = position.x;
			App->powers->position.y = position.y;
			App->powers->spawnPos.x = position.x;
			App->powers->spawnPos.y = position.y;
		

			App->player->score += 1000;
	}

}
