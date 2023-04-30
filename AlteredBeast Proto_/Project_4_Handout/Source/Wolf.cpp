#include "Wolf.h"
#include "Application.h"
#include "ModuleCollisions.h"
#include "Enemy.h"
#include "ModuleEnemies.h"
#include "ModulePlayer.h"

//Calls the constructor of enemy class to save spawn position


Wolf::Wolf(int x, int y) : Enemy(x, y) {

	destroyedCountdown = 10;
	/*hp = 1;*/

	//Default Direction
	dir = Direction::RIGHT;

	//Animations
	idleAnimRight.PushBack({ 1,70,66,50 });
	idleAnimRight.PushBack({ 1,70,66,50 });
	idleAnimRight.PushBack({ 1,70,66,50 });
	idleAnimRight.PushBack({ 1,70,66,50 });
	idleAnimRight.PushBack({ 1,70,66,50 });
	idleAnimRight.speed = 0.08f;
	idleAnimRight.loop = false;
	idleAnimRight.totalFrames = 5;

	idleAnimLeft.PushBack({ 547,70,66,50 });
	idleAnimLeft.PushBack({ 547,70,66,50 });
	idleAnimLeft.PushBack({ 547,70,66,50 });
	idleAnimLeft.PushBack({ 547,70,66,50 });
	idleAnimLeft.PushBack({ 547,70,66,50 });
	idleAnimLeft.speed = 0.08f;
	idleAnimLeft.loop = false;
	idleAnimLeft.totalFrames = 5;

	jumpAnimRight.PushBack({ 68,70,66,50 });
	jumpAnimRight.PushBack({ 135,70,67,50 });
	jumpAnimRight.speed = 0.008f;
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
	/*AttackCollider = App->collisions->AddCollider({ 600, 200, 34, 70 }, Collider::Type::ENEMY_SHOT, (Module*)App->player);*/
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
			currentAnim = &idleAnimRight;

			/*if (idleAnimRight.HasFinished()) {
				
				currentAnim = &jumpAnimRight;

				position.x += 0.7f;
				wolfImpulse -= Gravity;
				position.y -= wolfImpulse;

				if (position.y >= 140) {
					position.y = 140;
					
				}
			}*/
		}
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
	}

	if (!alive) {

		destroyed = true;

		if (dir == Direction::RIGHT) { currentAnim = &deathAnimRight; }
		if (dir == Direction::LEFT) { currentAnim = &deathAnimLeft; }

		if (currentAnim->HasFinished() == false) {
			destroyed = false;
		}
	}


	currentAnim->Update();
	Enemy::Update();
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
			Ecollider->SetPos(-3000, -3000);
			alive = false;

			App->scene->HasEnemyDied = true;
			App->scene->enemyX = position.x;
			App->scene->enemyY = position.y;


		

			App->player->score += 1000;
	}

}
