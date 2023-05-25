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


	Ecollider = App->collisions->AddCollider({ 400, 120, 50, 24 }, Collider::Type::ENEMY, (Module*)App->enemies);
	




	lethalAtt = App->audio->LoadFx("Assets/FX/Lethal_Punch");

	FlyR.PushBack({ 399, 545, 73,90 });
	FlyR.PushBack({ 473, 545, 73,90 });
	FlyR.PushBack({ 547, 545, 73,90 });
	FlyR.loop = true;
	FlyR.speed = 0.03f;
	
	

}

void Dragon::Update() {


	Ecollider->SetPos(position.x, position.y+15);

	currentAnim = &FlyR;
	
	if (followCnt >= 0) {
		Dragon::Follow();
		finalY = position.y + 100;

	}
	



	else if ( position.y <finalY && attackCnt == 30) {
		
		Dragon::Attack();
	}
	else {
		attackCnt--;
	}

	if (attackCnt <= 0 && position.y > initialY) {
		position.y--;
		if (position.y <= initialY) {
			attackCnt = 30;
			followCnt = 600;
		}
	}

	followCnt--;


	

	Enemy::Update();
	Ecollider->SetPos(position.x+6, position.y + 20);
}

void Dragon::OnCollision(Collider* collider) {

	if (collider->type == Collider::Type::PLAYER_SHOT) {

		destroyedCountdown--;
		hp -= App->player->attack;
		App->audio->PlayFx(lethalAtt, 5);
		hitByPlayer = true;
		destroyedCountdown = 20;

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


}

void Dragon::Follow() {

	// Aquires targetting position evry 50 frames
	if (checkPos == 50) {
		targetPos = App->player->position;
	}
	checkPos--;
	if (checkPos <= 0) { checkPos = 50; }


	// move towards target pos

	if (targetPos.x < position.x) {
		position.x -= 0.5;
	}
	else if (targetPos.x > position.x) {
		position.x += 0.5;
	}
	else {
		// TODO implement slight random movement pattern
	}

}

void Dragon::FlyBack() {

	if (position.y > initialY) {
		position.y++;
	}



}

