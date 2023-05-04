#include "WhiteWolf.h"
#include "Application.h"
#include "ModuleCollisions.h"
#include "Enemy.h"
#include "ModuleEnemies.h"
#include "ModulePlayer.h"
#include "ModulePower.h"
#include "ModuleScene.h"

//Calls the constructor of enemy class to save spawn position


WhiteWolf::WhiteWolf(int x, int y) : Enemy(x, y) {

	destroyedCountdown = 10;
	hp = 1;

	//Default Direction
	dir = Direction::RIGHT;

	//Animations
	idleAnimRight.PushBack({ 1,70,66,50 });
	idleAnimRight.PushBack({ 1,241,66,50 });
	idleAnimRight.PushBack({ 135,241,66,50 });
	idleAnimRight.PushBack({ 1,241,66,50 });
	idleAnimRight.speed = 0.02f;
	idleAnimRight.loop = true;
	idleAnimRight.totalFrames = 4;

	idleAnimLeft.PushBack({ 547,70,66,50 });
	idleAnimLeft.PushBack({ 68,241,66,50 });
	idleAnimLeft.PushBack({ 1,292,66,50 });
	idleAnimLeft.PushBack({ 68,241,66,50 });
	idleAnimLeft.speed = 0.02f;
	idleAnimLeft.loop = true;
	idleAnimLeft.totalFrames = 4;

	jumpAnimRight.PushBack({ 68,70,66,50 });
	jumpAnimRight.PushBack({ 135,70,67,50 });
	jumpAnimRight.speed = 0.05f;
	jumpAnimRight.loop = false;
	jumpAnimRight.totalFrames = 2;

	jumpAnimLeft.PushBack({ 480,70,66,50 });
	jumpAnimLeft.PushBack({ 412,70,67,50 });
	jumpAnimLeft.speed = 0.05f;
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

void WhiteWolf::Update() {
	if(knocked == true){ knockH = position.y - 150; }

	if (Y0 == 0) {
		Y0 = position.y;
	}

	//if wolf is behind the player change the direction
	if (position.x < App->player->position.x) 
	{ dir = Direction::LEFT; }
	else { dir = Direction::RIGHT; }

	if (alive && knocked == false)
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
				JumpCnt = 150;
			}

			if (idle == false) {
				AttackCollider->SetPos(position.x-5, position.y+10);
				currentAnim = &jumpAnimRight;
				position.x -= 1.2f;
				wolfImpulse -= Gravity;
				position.y -= wolfImpulse;

				if (position.y > Y0) {
					position.y = Y0;
					idle = true;
					wolfImpulse = 2.0f;
				}
			}
			
			
		}

		
		if (dir == Direction::LEFT) 
		{ 
			if (idle == true) {
				currentAnim = &idleAnimLeft;
				JumpCnt--;
				AttackCollider->SetPos(-2000, -2000);
			}

			if (JumpCnt <= 0) {
				idle = false;
				JumpCnt = 150;

			}
			if (idle == false) {
				AttackCollider->SetPos(position.x + 5, position.y + 2);
				currentAnim = &jumpAnimLeft;
				position.x += 1.2f;
				wolfImpulse -= Gravity;
				position.y -= wolfImpulse;
				
				if (position.y > 140) {
					position.y = 140;
					idle = true;
					wolfImpulse = 2.0f;
				}
			}
		}
		

		Enemy::Update();
		

	}
	//Kocked
	if (knocked == true) {
		if (stunt == true) {
			stunt = false;
			knockH = position.y;
		}
		knockImpulse -= Gravity;
		position.y -= knockImpulse;


		if (position.y >= 140) {
			position.y = 140;
			idle = true;
			knocked = false;
			stunt = true;
			knockImpulse = 3.0f;
		}


	}


	
}

void WhiteWolf::OnCollision(Collider* collider) {

	if (collider->type == Collider::Type::PLAYER_SHOT) {

		hp--;

		Ecollider->SetPos(-3000, -3000);
		alive = false;

		App->scene->HasEnemyDied = true;
		App->scene->enemyX = position.x;
		App->scene->enemyY = position.y;
		App->scene->EnemyCN = 2;
		if (App->player->tranSt == ModulePlayer::Transform::DEFAULT) {
			App->powers->Enable();
			App->powers->position.x = position.x;
			App->powers->position.y = position.y;
			App->powers->spawnPos.x = position.x;
			App->powers->spawnPos.y = position.y;
		}
		if (App->player->tranSt == ModulePlayer::Transform::POWER1 && App->player->destroyed) {
			App->powers->Disable();
		}
		

		App->player->score += 1000;
	}

	if (collider->type == Collider::Type::PLAYER) {
		knocked = true;

	}

}
