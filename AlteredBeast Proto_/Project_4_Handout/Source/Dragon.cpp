#include "Application.h"
#include "ModuleCollisions.h"
#include "Enemy.h"
#include "ModuleEnemies.h"
#include "ModulePlayer.h"
#include "EnemyDeath.h"
#include "Dragon.h"
#include "ModuleScene2.h"
#include "ModulePlayer.h"
#include "time.h"


Dragon::Dragon(int x, int y) : Enemy(x, y) {

	hp = 1;
	attacking = false;

	Ecollider = App->collisions->AddCollider({ 400, 120, 50, 24 }, Collider::Type::ENEMY, (Module*)App->enemies);
	
	AttackCollider = App->collisions->AddCollider({ 0,0, 17,21 }, Collider::Type::ENEMY_SHOT, (Module*)App->player);

	destroyedCountdown = 20;


	lethalAtt = App->audio->LoadFx("Assets/FX/Lethal_Punch");

	FlyL.PushBack({ 399, 545, 73,90 });
	FlyL.PushBack({ 473, 545, 73,90 });
	FlyL.PushBack({ 547, 545, 73,90 });
	FlyL.loop = true;
	FlyL.speed = 0.03f;
	
	FlyR.PushBack({ 695, 636, 73,90 });
	FlyR.PushBack({ 621, 636, 73,90 });
	FlyR.PushBack({ 547, 636, 73,90 });
	FlyR.loop = true;
	FlyR.speed = 0.03f;

	AttackL.PushBack({ 621,545, 73,90 });
	AttackL.PushBack({ 695,545, 73,90 });
	AttackL.PushBack({ 621,545, 73,90 });
	AttackL.PushBack({ 695,545, 73,90 });
	AttackL.loop = false;
	AttackL.speed = 0.04f;

	AttackR.PushBack({ 399,636, 73,90 });
	AttackR.PushBack({ 473,636, 73,90 });
	AttackR.PushBack({ 399,636, 73,90 });
	AttackR.PushBack({ 473,636, 73,90 });
	AttackR.loop = false;
	AttackR.speed = 0.04f;

}

void Dragon::Update() {


	

	if (dir == Direction::RIGHT) {
		currentAnim = &FlyR;
	}
	if (dir == Direction::LEFT) {
		currentAnim = &FlyL;
	}
	
	if (followCnt >= 0) {
		

		// Aquires targetting position evry 50 frames
		if (checkPos == 50) {
			targetPos = App->player->position;

			if (targetPos.x < position.x && attacking == false) {
				dir = Direction::RIGHT;
			}
			if (targetPos.x >= position.x && attacking == false) {
				dir = Direction::LEFT;
			}
		}
		checkPos--;
		if (checkPos <= 0) { checkPos = 50; }


		// move towards target pos

		if (targetPos.x < position.x) {
			position.x -= 0.5;

		}
		else if (targetPos.x >= position.x) {
			position.x += 0.5;

		}
		else {
			// TODO implement slight random movement pattern
		}

		finalY = position.y + 70;

	}
	



	else if ( position.y <finalY && attackCnt == 30) {
		
		Dragon::Attack();
	}
	else {
		attackCnt--;
	}

	if (attackCnt <= 0 && attacking == true) {

	
		if (dir == Direction::LEFT) {
			currentAnim = &AttackL;
			AttackCollider->SetPos(position.x + 41, position.y + 68);
		}
		if (dir == Direction::RIGHT) {
			currentAnim = &AttackR;
			AttackCollider->SetPos(position.x + 18, position.y + 68);
		}
	}

	if (AttackL.HasFinished() == true || AttackR.HasFinished() == true) {
		currentAnim->Reset();
		currentAnim->loopCount = 0;


		AttackCollider->SetPos(-2000, -2000);

		attacking = false;
	}

	if (attacking == false) {
		// FlyBack()
		if (attackCnt <= 0 && position.y > initialY) {
			position.y--;
			if (position.y <= initialY) {
				attackCnt = 30;
				followCnt = 400;
			}
		}
	}

	followCnt--;


	

	Enemy::Update();
	Ecollider->SetPos(position.x+6, position.y + 40);
}

void Dragon::OnCollision(Collider* collider) {

	if (collider->type == Collider::Type::PLAYER_SHOT) {

		if (destroyedCountdown <= 0) {
			destroyedCountdown--;
			
			App->audio->PlayFx(lethalAtt, 5);
			
			
		}
		hitByPlayer = true;
		hp -= App->player->attack;

	}
	if (hp <= 0) {
		Ecollider->SetPos(-1000, -1000);
		alive = false;

		App->scene->EnemyCN = 7;
		App->scene->HasEnemyDied = true;
		App->scene->enemyX = position.x;
		App->scene->enemyY = position.y;

		
	}

	if (collider == App->scene->backCamLimit) {
		position.x += 3;
	}
	if (collider == App->scene->frontCamLimit) {
		position.x -= 3;
	}
 



}

void Dragon::Attack() {

	attacking = true;

	int distanceX = (App->player->position.x + 45) - (position.x);
	int distanceY = App->player->position.y - finalY;
	double distance = sqrt(distanceX * distanceX + distanceY * distanceY);
	double directionX = distanceX / distance;
	double directionY = distanceY / distance;
	int speed = 1.4;

	float VelocityX = directionX * speed;
	float VelocityY = directionY * speed;

	position.x += VelocityX;
	position.y += VelocityY;

	if (position.y == finalY) {

		attacking = true;
		if (dir == Direction::LEFT) {
			currentAnim = &AttackL;
		}
		if (dir == Direction::RIGHT) {
			currentAnim = &AttackR;
		}
	}

}

void Dragon::Follow() {

	// Aquires targetting position evry 50 frames
	if (checkPos == 50) {
		targetPos = App->player->position;

		if (targetPos.x < position.x) {
			dir = Direction::RIGHT;
		}
		else{
			dir = Direction::LEFT;
		}
	}
	checkPos--;
	if (checkPos <= 0) { checkPos = 50; }


	// move towards target pos

	if (targetPos.x < position.x) {
		position.x -= 0.5;
		
	}
	else if (targetPos.x >= position.x) {
		position.x += 0.5;
		
	}
	else {
		// TODO implement slight random movement pattern
	}

}

void Dragon::FlyBack() {

	if (attackCnt <= 0 && position.y > initialY) {
		position.y--;
		if (position.y <= initialY) {
			attackCnt = 30;
			followCnt = 600;
		}
	}


}

