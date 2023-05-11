#include "Application.h"
#include "ModuleCollisions.h"
#include "Enemy.h"
#include "ModuleEnemies.h"
#include "ModulePlayer.h"
#include "EnemyDeath.h"
#include "Tomb.h"
#include "ModuleScene2.h"

//Calls the constructor of enemy class to save spawn position


Tomb::Tomb(int x, int y) : Enemy(x, y) {

	hp = 1;
	Fy = y;

	PlatformCollider = App->collisions->AddCollider({ 100,100,26,5 },Collider::Type::PLATFORM, (Module*)App->player);
	WallLCollider = App->collisions->AddCollider({ 100,121,10,56 }, Collider::Type::WALL, (Module*)App->player);
	WallRCollider = App->collisions->AddCollider({ 100,121,10,56 }, Collider::Type::WALL_RIGHT, (Module*)App->player);


	CodeN = 5;


	position.y += 50;

	lethalAtt = App->audio->LoadFx("Assets/FX/Lethal_Punch");

	idle.PushBack({ 202,272,32,61 });

	idle.loop = true;

}

void Tomb::Update() {



	Tomb::Rise();

	currentAnim = &idle;

	WallLCollider->SetPos(position.x, position.y+4);
	WallRCollider->SetPos(position.x+22, position.y + 4);
	PlatformCollider->SetPos(position.x+4, position.y);

	Enemy::Update();
}

void Tomb::OnCollision(Collider* collider) {

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

		App->scene->EnemyCN = 4;
		App->scene->HasEnemyDied = true;
		App->scene->enemyX = position.x;
		App->scene->enemyY = position.y;

		App->audio->PlayMusic("Assets/Music/Win.ogg", 0.0f);

		App->scene2->killedBoss = true;
		App->player->KilledBoss = true;
	}




}

void Tomb::Rise() {

	if (Risen == false) {
		Risecnt--;
		if (Risecnt <= 0) {
			position.y--;
			if (position.y <= Fy) {
				position.y = Fy;
				Risen = true;
			}
		}
		
	}
}