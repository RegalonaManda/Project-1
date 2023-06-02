#include "Application.h"
#include "ModuleCollisions.h"
#include "Enemy.h"
#include "ModuleEnemies.h"
#include "ModulePlayer.h"
#include "EnemyDeath.h"
#include "Tomb.h"
#include "ModuleScene2.h"

//Calls the constructor of enemy class to save spawn position

// the border boolean marks if the toomb is the last, if it needs a border
Tomb::Tomb(int x, int y, bool borderL, bool borderR, bool Zombie) : Enemy(x, y) {



	TombBorderL = borderL;
	TombBorderR = borderR;
	this->Zombie = Zombie;
	hp = 2;
	Fy = y;

	PlatformCollider = App->collisions->AddCollider({ 100,100,26,5 },Collider::Type::PLATFORM, (Module*)App->player);
	WallLCollider = App->collisions->AddCollider({ 100,121,10,56 }, Collider::Type::WALL, (Module*)App->player);
	WallRCollider = App->collisions->AddCollider({ 100,121,10,56 }, Collider::Type::WALL_RIGHT, (Module*)App->player);
	SelfDestruct = App->collisions->AddCollider({ -7000,-7000,5,5 }, Collider::Type::PLAYER_SHOT, (Module*)App->enemies);

	Ecollider = App->collisions->AddCollider({ 100,114,22,60 }, Collider::Type::ENEMY, (Module*)App->enemies);

	CodeN = 5;

	
		LBorder = App->collisions->AddCollider({ 100,700,5,5 }, Collider::Type::BORDER, (Module*)App->player);
	
	
		RBorder = App->collisions->AddCollider({ 100,700,5,5 }, Collider::Type::BORDER, (Module*)App->player);
	


	position.y += 70;

	lethalAtt = App->audio->LoadFx("Assets/FX/Lethal_Punch.wav");

	idle.PushBack({ 202,272,32,61 });

	shake.PushBack({ 201,272,32,61 });
	shake.PushBack({ 203,272,32,61 });
	shake.loop = true;
	shake.speed = 0.1f;

	idle.loop = true;

}

void Tomb::Update() {



	Tomb::Rise();
	if (zombieTimer > 200 && shakeB == false) {
		currentAnim = &idle;
	}

	if (shakeB == true) {
		currentAnim = &shake;
		shaketimer--;
		if (shaketimer <= 0) {
			shakeB = false;
			shaketimer = 20;
		}

	}

	WallLCollider->SetPos(position.x, position.y+4);
	WallRCollider->SetPos(position.x+22, position.y + 4);
	PlatformCollider->SetPos(position.x+4, position.y);

	//Ecollider->SetPos(position.x + 5, position.y-200);

	if (TombBorderL == true && Rising == true || Risen == true && TombBorderL == true) {
		LBorder->SetPos(position.x - 20, position.y -40);
	}

	if (TombBorderL == true && Rising == true || Risen == true && TombBorderR == true) {
		
		RBorder->SetPos(position.x +52, position.y -40);
	}


	// Release zombie if has zombie inside
	if (Zombie == true && Risen == true) {
		zombieTimer--;

		if (zombieTimer <= 200) {
			currentAnim = &shake;
		}
		if (zombieTimer <= 0) {
			App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, position.x, position.y, true);
			Zombie = false;
			hp = 0;
			SelfDestruct->SetPos(position.x + 10, position.y + 10);
			WallLCollider->SetPos(-650, -650);
			WallRCollider->SetPos(-650, -650);
			PlatformCollider->SetPos(-650, -650);
		}
	}


	Enemy::Update();
}

void Tomb::OnCollision(Collider* collider) {


	

	if (collider->type == Collider::Type::PLAYER_SHOT) {
		
		shakeB = true;
		if(destroyedCountdown == 20){ hp -= App->player->attack; }
		destroyedCountdown--;
		if (destroyedCountdown <= 0) {
			
			App->audio->PlayFx(lethalAtt, 5);
			destroyedCountdown = 20;

		}



		WallLCollider->SetPos(-650, -650);
		WallRCollider->SetPos(-650,-650);
		PlatformCollider->SetPos(-650, -650);
		// Add enemy?
		if (Zombie == true) {
			App->enemies->AddEnemy(ENEMY_TYPE::ZOMBIE, position.x, position.y, true);
			Zombie = false;
		}


		if (hp <= 0) {
			LBorder->SetPos(position.x + 20, position.y - 40);
		}
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