#include "BrownWolf.h"
#include "Application.h"
#include "ModuleCollisions.h"
#include "Enemy.h"
#include "ModuleEnemies.h"
#include "ModulePlayer.h"
#include "ModulePower.h"
#include "ModuleScene.h"
#include "ModuleScene2.h"

//Calls the constructor of enemy class to save spawn position

int MAX_HEIGHT = 0;

BrownWolf::BrownWolf(int x, int y) : Enemy(x, y) {

	destroyedCountdown = 10;
	hp = 1;

	//Default Direction
	dir = Direction::RIGHT;

	//Animations
	idleAnimRight.PushBack({ 1,190,66,50 });
	idleAnimRight.speed = 0.02f;
	idleAnimRight.loop = true;
	idleAnimRight.totalFrames = 1;

	idleAnimLeft.PushBack({ 547,190,66,50 });
	idleAnimLeft.speed = 0.02f;
	idleAnimLeft.loop = true;
	idleAnimLeft.totalFrames = 1;

	jumpAnimRight.PushBack({ 68,190,66,50 });
	jumpAnimRight.PushBack({ 134,190,67,50 });
	jumpAnimRight.speed = 0.05f;
	jumpAnimRight.loop = false;
	jumpAnimRight.totalFrames = 2;

	jumpAnimLeft.PushBack({ 480,190,66,50 });
	jumpAnimLeft.PushBack({ 413,190,67,50 });
	jumpAnimLeft.speed = 0.05f;
	jumpAnimLeft.loop = false;
	jumpAnimLeft.totalFrames = 2;


	deathAnimRight.PushBack({ 135,121,33,50 });
	deathAnimRight.PushBack({ 169,121,33,50 });
	deathAnimRight.speed = 0.008f;
	deathAnimRight.loop = false;
	deathAnimRight.totalFrames = 2;

	deathAnimLeft.PushBack({ 580,121,33,50 });
	deathAnimLeft.PushBack({ 546,121,33,50 });
	deathAnimLeft.speed = 0.008f;
	deathAnimLeft.loop = false;
	deathAnimLeft.totalFrames = 2;

	Ecollider = App->collisions->AddCollider({ 600, 190, 55, 22 }, Collider::Type::ENEMY, (Module*)App->enemies);
	AttackCollider = App->collisions->AddCollider({ 600, 200, 34, 30 }, Collider::Type::ENEMY_SHOT, (Module*)App->player);

	position.y = 210;
}

void BrownWolf::Update() {
	if (knocked == true) { knockH = position.y - 150; }


	Ecollider->SetPos(position.x, position.y+20 );

	if (idle == true) {
		Y0 = position.y;
	}

	//if wolf is behind the player change the direction
	if (position.x < App->player->position.x)
	{
		dir = Direction::LEFT;
	}
	else { dir = Direction::RIGHT; }

	if (alive && knocked == false)
	{

		if (dir == Direction::RIGHT)
		{
			if (idle == true) {
				currentAnim = &idleAnimRight;
				JumpCnt--;
				AttackCollider->SetPos(-2000, -2000);
			}

			if (JumpCnt <= 0) {
				position.y -= 10;
				idle = false;
				JumpCnt = 150;
			}

			if (idle == false) {
				AttackCollider->SetPos(position.x - 5, position.y + 10);
				currentAnim = &jumpAnimRight;
				position.x -= 1.2f;
				wolfImpulse -= Gravity;
				position.y -= wolfImpulse;

				/*if (position.y > Y0) {
					position.y = Y0;
					idle = true;
					wolfImpulse = 2.0f;
				}*/
			}


		}

		// Working on this... change like right direction
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
	//Knocked
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

		// calculate the max h for the current ground lvl
		if (knocked == false) {
			MAX_HEIGHT = position.y - 56;
		}
	}
	//if (!alive) {
	//	

	//	if (dir == Direction::RIGHT) { currentAnim = &deathAnimRight; }
	//	if (dir == Direction::LEFT) { currentAnim = &deathAnimLeft; }

	//	if (currentAnim->HasFinished() == false) {
	//		destroyed = false;
	//	}
	//}



}

void BrownWolf::OnCollision(Collider* collider) {

	if (collider->type == Collider::Type::PLAYER_SHOT) {

		hp--;

		Ecollider->SetPos(-3000, -3000);
		alive = false;

		App->scene->HasEnemyDied = true;
		App->scene->enemyX = position.x;
		App->scene->enemyY = position.y;
		App->scene->EnemyCN = 3;

		/*App->powers->Enable();
		App->powers->position.x = position.x;
		App->powers->position.y = position.y;
		App->powers->spawnPos.x = position.x;
		App->powers->spawnPos.y = position.y;*/


		App->player->score += 1000;
	}

	if (collider->type == Collider::Type::PLAYER) {
		knocked = true;

	}

	if(collider == App->scene2->Ground || collider->type == Collider::Type::PLATFORM) {
		position.y-=25;
		idle = true;
		knocked = false;
		stunt = true;
		knockImpulse = 3.0f;
		wolfImpulse = 2.0f;
	}

}
