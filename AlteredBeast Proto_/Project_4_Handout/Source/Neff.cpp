#include "Application.h"
#include "ModuleCollisions.h"
#include "Enemy.h"
#include "ModuleEnemies.h"
#include "ModulePlayer.h"
#include "EnemyDeath.h"
#include "Neff.h"
#include "ModuleScene2.h"

//Calls the constructor of enemy class to save spawn position


Neff::Neff(int x, int y) : Enemy(x, y) {
	
	hp = 1;


	Ecollider = App->collisions->AddCollider({ 400, 120, 24, 60 }, Collider::Type::ENEMY, (Module*)App->enemies);
	CodeN = 3;

	
	

	lethalAtt = App->audio->LoadFx("Assets/FX/Lethal_Punch");

	CapeAnimation.PushBack({ 1,600,57,83 });
	CapeAnimation.PushBack({ 59,600,57,83 });
	CapeAnimation.PushBack({ 117,600,57,83 });
	CapeAnimation.speed = 0.01;
	CapeAnimation.loop = true;

}

void Neff::Update() {
	
	currentAnim = &CapeAnimation;

	

	
	Enemy::Update();
}

void Neff::OnCollision(Collider* collider) {

	if (collider->Collider::Type::PLAYER_SHOT) {
		
		
		destroyedCountdown--;
		if (destroyedCountdown <= 0) {
			hp-= App->player->attack;
			App->audio->PlayFx(lethalAtt, 5);
			hitByPlayer = true;
			destroyedCountdown = 20;
		}

	}
	if (hp <= 0) {
		Ecollider->SetPos(-1000, -1000);
		alive = false;

		App->scene->HasEnemyDied = true; 
		App->scene->enemyX = position.x;
		App->scene->enemyY = position.y;

		App->audio->PlayMusic("Assets/Music/Win.ogg", 3.0f);

		App->scene2->killedBoss = true;
		App->player->KilledBoss = true;
	}

	
	
	
}

void Neff::Attack() {

	
		
	

}
