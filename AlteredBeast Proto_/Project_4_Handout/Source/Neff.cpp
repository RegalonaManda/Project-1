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

	RayAtt.PushBack({ 701,906,114,87 });
	RayAtt.PushBack({ 701,818,114,87 });
	RayAtt.PushBack({ 586,818,114,87 });
	RayAtt.PushBack({ 471,818,114,87 });
	RayAtt.PushBack({ 356,818,114,87 });
	RayAtt.PushBack({ 241,818,114,87 });
	RayAtt.PushBack({ 126,818,114,87 });
	RayAtt.PushBack({ 11,818,114,87 });
	RayAtt.PushBack({ 701,818,114,87 });
	RayAtt.PushBack({ 701,906,114,87 });
	RayAtt.PushBack({ 701,906,114,87 });
		
	RayAtt.speed = 0.05f;
	RayAtt.loop = true;

	reachedBoss = false;
	scroll = 1200;
	
}

void Neff::Update() {
	
	currentAnim = &RayAtt;
	scroll--;

	if (reachedBoss) {
		bossCountdown--;
	}
	
	Enemy::Update();

	if (scroll <= 0) {
		App->scene->ScreenScroll = false;

		Ecollider->SetPos(position.x + 10, position.y + 15);
	}
	else {
		Ecollider->SetPos(-200, -2000);

	}
	
	if (run) {
		runCnt--;
		Neff::HeadOut();
	}

	
	
	
	
	ActivateBoss();
	
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

		App->player->Neff_pushBack--;
		


		if (last == false) {
			if (App->player->tranSt != Transform::WOLF) {
				run = true;

			}
			else {
				reachedBoss = true;
				
			}
		}
		else {
			reachedBoss = true;
		}
	}
	
	
	
	
}

void Neff::Attack() {

	

}

void Neff::HeadOut() {

	App->scene->ScreenScroll = true;
	
	if (run && runCnt <= 0) {
		
		position.x += 2;
	}


}

void Neff::ActivateBoss(){
	
	if (bossCountdown <= 0) {
		/*App->bossfight->Enable();*/

		App->bossfight->Neff_activ = true;
		App->bossfight->hp = 100;
		App->bossfight->beaten = false;
		App->bossfight->initilized = false;
		//App->bossfight->selected = false;
		App->bossfight->position.x = position.x + 70;

		App->bossfight->position.y = position.y - 60;
		App->enemies->Disable();
		hp = 0;
	}
}
