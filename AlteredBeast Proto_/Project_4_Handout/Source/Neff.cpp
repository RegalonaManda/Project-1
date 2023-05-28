#include "Application.h"
#include "ModuleCollisions.h"
#include "Enemy.h"
#include "ModuleEnemies.h"
#include "ModulePlayer.h"
#include "EnemyDeath.h"
#include "Neff.h"
#include "ModuleScene2.h"
#include "ModuleBoss.h"

//Calls the constructor of enemy class to save spawn position


Neff::Neff(int x, int y, bool last) : Enemy(x, y) {
	

	this->last = last;
	hp = 1;


	Ecollider = App->collisions->AddCollider({ 400, 120, 50, 60 }, Collider::Type::RANGE, (Module*)App->enemies);
	CodeN = 4;

	//rangeCollider = App->collisions->AddCollider({0,0,50,73}, Collider::Type::RANGE, (Module*)App->enemies);
	

	lethalAtt = App->audio->LoadFx("Assets/FX/Lethal_Punch");

	RayAtt.PushBack({ 1125,12,114,87 });
	RayAtt.PushBack({ 1010,12,114,87 });
	RayAtt.PushBack({ 895,12,114,87 });
	RayAtt.PushBack({ 780,12,114,87 });
	RayAtt.PushBack({ 665,12,114,87 });
	RayAtt.PushBack({ 550,12,114,87 });
	RayAtt.PushBack({ 435,12,114,87 });
	RayAtt.PushBack({ 320,12,114,87 });
		
	RayAtt.speed = 0.01;
	RayAtt.pingpong = true;
	
	
}

void Neff::Update() {
	
	currentAnim = &RayAtt;
	
	//rangeCollider->SetPos(position.x - 50, position.y);

	if (run) {
		runCnt--;
		Neff::HeadOut();
	}

	
	
	Enemy::Update();

	Ecollider->SetPos(position.x - 30, position.y + 10);
}

void Neff::OnCollision(Collider* collider) {

	/*if (collider->type == Collider::Type::PLAYER_SHOT) {

		destroyedCountdown--;
		hp -= App->player->attack;
		App->audio->PlayFx(lethalAtt, 5);
		hitByPlayer = true;
		destroyedCountdown = 20;

	}
	if (hp <= 0) {
		Ecollider->SetPos(-1000, -1000);
		alive = false;
		
		App->scene->EnemyCN = 4;
		App->scene->HasEnemyDied = true; 
		App->scene->enemyX = position.x;
		App->scene->enemyY = position.y;

		App->audio->PlayMusic("Assets/Music/Win.ogg", 0.0f);

		App->scene2->killedBoss = true;
		App->player->KilledBoss = true;
	}*/

	if (collider->type == Collider::Type::PLAYER) {
		if (last == false) {
			if (App->player->tranSt != Transform::WOLF) {
				run = true;

			}
			else {
				App->bossfight->Enable();

				App->bossfight->position.x = position.x;
				App->bossfight->position.y = position.y - 80;
				hp = 0;
			}
		}
		else {
			App->bossfight->Enable();

			App->bossfight->position.x = position.x;
			App->bossfight->position.y = position.y - 80;
			hp = 0;
		}
	}
	
	
	
	
}

void Neff::Attack() {

	

}

void Neff::HeadOut() {

	if (run && runCnt <= 0) {
		position.x += 2;
	}


}
