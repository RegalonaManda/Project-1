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
Tomb::Tomb(int x, int y, bool borderL, bool borderR) : Enemy(x, y) {



	TombBorderL = borderL;
	TombBorderR = borderR;

	hp = 1;
	Fy = y;

	PlatformCollider = App->collisions->AddCollider({ 100,100,26,5 },Collider::Type::PLATFORM, (Module*)App->player);
	WallLCollider = App->collisions->AddCollider({ 100,121,10,56 }, Collider::Type::WALL, (Module*)App->player);
	WallRCollider = App->collisions->AddCollider({ 100,121,10,56 }, Collider::Type::WALL_RIGHT, (Module*)App->player);

	//Ecollider = App->collisions->AddCollider({ 100,121,22,56 }, Collider::Type::ENEMY, (Module*)App->player);

	CodeN = 5;

	if (borderL == true) {
		LBorder = App->collisions->AddCollider({ 100,700,5,5 }, Collider::Type::BORDER, (Module*)App->player);
	}
	if (borderR == true) {
		RBorder = App->collisions->AddCollider({ 100,700,5,5 }, Collider::Type::BORDER, (Module*)App->player);
	}


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

	//Ecollider->SetPos(position.x + 5, position.y);

	if (TombBorderL == true && Rising == true || Risen == true && TombBorderL == true) {
		LBorder->SetPos(position.x - 20, position.y -40);
	}

	if (TombBorderL == true && Rising == true || Risen == true && TombBorderR == true) {
		
		RBorder->SetPos(position.x +52, position.y -40);
	}

	Enemy::Update();
}

void Tomb::OnCollision(Collider* collider) {


	//if (collider->type == Collider::Type::PLAYER_SHOT) {

	//	hp--;

	//	Ecollider->SetPos(-3000, -3000);
	//	alive = false;


	//	
	//}



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