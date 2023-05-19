#include "Skull.h"
#include "Application.h"
#include "ModuleCollisions.h"
#include "Enemy.h"
#include "ModuleEnemies.h"
#include "ModulePlayer.h"
#include "EnemyDeath.h"



//Calls the constructor of enemy class to save spawn position


Skull::Skull(int x, int y) : Enemy(x, y) {

	isFalling = false;

	Spawning = true;
	hp = 2;
	//-------------------------------------------- Left Dir Animations ------------------------------------------------------//

	//------ Walk -------//

	walkAnimL.PushBack({1, 343, 816, 692});
	walkAnimL.PushBack({ 86,343,816,692 });
	walkAnimL.speed = 0.012f;
	walkAnimL.loop = true;


	//------ Crouch -------//

	crouchAnimL.PushBack({ 256,343,816,692 });
	crouchAnimL.speed = 0.01f;
	crouchAnimL.loop = false;
	
	

	//------ DeathAnim -------//

	//deathAnimL.PushBack({ 127,1,41,68 });
	//deathAnimL.PushBack({ 132,1,41,68 });
	//deathAnimL.PushBack({ 127,1,41,68 });
	//deathAnimL.PushBack({ 132,1,41,68 });
	deathAnimL.loop = false;
	deathAnimL.totalFrames = 4;
	deathAnimL.speed = 0.03f;

	//------------------------------------------- Right Dir Animations ------------------------------------------------------//

	//------ Walk -------//

	walkAnimR.PushBack({ 1, 412, 816,692 });
	walkAnimR.PushBack({ 86,412, 816,692 });
	walkAnimR.speed = 0.008f;
	walkAnimR.loop = true;

	//------ BodyXplode -------(notfinished)//

	

	//------ DeathAnim -------(notfinished)//

	deathAnimR.PushBack({ 127,1,41,68 });
	deathAnimR.PushBack({ 132,1,41,68 });
	deathAnimR.PushBack({ 127,1,41,68 });
	deathAnimR.PushBack({ 132,1,41,68 });
	deathAnimR.loop = false;
	deathAnimR.totalFrames = 4;
	deathAnimR.speed = 0.03f;



	//Default Direction
	dir = Direction::LEFT;
	SkSt = SkullState::ADVANCE;

	IsExploding = false;
	//at 1 hit, head explodes, continues
	//at 2 hits, disappears

	//if enemy gets close enough to player, will explode -> yellow sprite - normal sprite - disappears (no shake)

	Ecollider = App->collisions->AddCollider({ 400, 120, 24, 60 }, Collider::Type::ENEMY, (Module*)App->enemies);
	AttackCollider = App->collisions->AddCollider({ 412,140,50,0 }, Collider::Type::ENEMY_SHOT, (Module*)App->player);
	

	SelfDestruct = App->collisions->AddCollider({ 5,5,5,5 }, Collider::ENEMY_SELF_DESTRUCT, (Module*)App->enemies);

	//attack starts oob
	AttackCollider->SetPos(-1000, -1000);

	SelfDestruct->SetPos(-1000, -1000);

	lethalAtt = App->audio->LoadFx("Assets/FX/Lethal_Punch");
	//default anim walk left
	currentAnim = &walkAnimL;

}

void Skull::Update() {

	

	while (Ecollider->Intersects(App->player->Pcollider->rect) == false) {
		stillCountdown--;
		if (SkSt == STILL && stillCountdown == 0) {
			SkSt = ADVANCE;
			if (dir == Direction::LEFT) 
			{ 
				currentAnim = &walkAnimL; 
				position.x -= 10.0f;
			}
			else 
			{ 
				currentAnim = &walkAnimR; 
				position.x += 10.0f;
			}
			walkCountdown = 30;
			
		}

		walkCountdown--;
		if (SkSt == ADVANCE && walkCountdown == 0) {
			SkSt = STILL;
			if (dir == Direction::LEFT) { currentAnim = &idleAnimL; }
			else { currentAnim = &idleAnimR; }
			stillCountdown = 20;
		}
	}

	//if skull is behind the player change the direction

	if (position.x <= App->player->position.x)
	{
		dir = Direction::RIGHT;
	}
	else { dir = Direction::LEFT; }

	//if (Spawning == false) {
	//	if (dir == Direction::RIGHT) {

	//		Zspeed = (Zspeed + 0.4f) * -1;

	//	}
	//}
	//if (dir == Direction::LEFT) {
	//	currentAnim = &walkAnimL;
	//}
	//if (dir == Direction::RIGHT) {
	//	currentAnim = &walkAnimR;
	//}

}
	//if (alive)
	//{


//		/*if (hp == 2 && attacking == false) { currentAnim = &walkAnimL; }*/
//		if (hp == 1 && hitByPlayer)
//		{
//			hitCountdown--;
//			if (hitCountdown <= 0) {
//				if (dir == Direction::LEFT) {
//					currentAnim = &headXplodeL;
//				}
//				else if (dir == Direction::RIGHT) {
//					currentAnim = &headXplodeR;
//				}
//				App->audio->PlayFx(lethalAtt, 3);
//			}
//			if (dir == Direction::LEFT) {
//				if (headXplodeL.HasFinished()) {
//					currentAnim = &headlessWalkL;
//					hitByPlayer = false;
//				}
//			}
//			if (dir == Direction::RIGHT) {
//				if (headXplodeR.HasFinished()) {
//					currentAnim = &headlessWalkR;
//					hitByPlayer = false;
//				}
//			}
//		}
//	}
//
//
//	if (attacking == true) { Attack(); }
//
//
//
//
//	Ecollider->SetPos(2000, 2000);
//	// The collider of the enemy changes depending on the position of the direction
//	if (attackCnt > 0) {
//		if (dir == Direction::LEFT) {
//			XplosionTrigger->SetPos(position.x - 10, position.y);
//			/*XplosionTrigger->SetPos(position.x - 10, position.y);*/
//		}
//		else if (dir == Direction::RIGHT) {
//			XplosionTrigger->SetPos(position.x + 6, position.y);
//		}
//
//	}
//
//
//
//
//	if (falling == true) {
//		position.y += 3;
//	}
//
//
//
//
//
//
//	currentAnim->Update();
//	Enemy::Update();
//
//}
//
//void Zombie::OnCollision(Collider* collider) {
//
//	if (collider->type == Collider::Type::PLAYER_SHOT) {
//
//
//		destroyedCountdown--;
//		if (destroyedCountdown <= 0) {
//			hp -= App->player->attack;
//			App->audio->PlayFx(lethalAtt, 5);
//			hitByPlayer = true;
//			destroyedCountdown = 20;
//			if (hp <= 0) {
//				App->player->score += 100;
//			}
//
//		}
//
//	}
//	if (hp <= 0 && attacking == false) {
//		Ecollider->SetPos(-1000, -1000);
//		alive = false;
//
//
//		App->scene->EnemyCN = 1;
//		App->scene->HasEnemyDied = true;
//		App->scene->enemyX = position.x;
//		App->scene->enemyY = position.y;
//	}
//
//	//make the explosion kill the zombie
//
//	//rise above paltform
//	if (collider->type == Collider::Type::PLATFORM) {
//		if (collider != App->scene2->Ground) {
//			position.y -= 1.8f;
//		}
//		falling = false;
//		idle = true;
//	}
//
//	// stop againts wall
//	if (collider->type == Collider::Type::WALL) {
//		position.x -= 1.1f;
//		// we could also stop their movement behaviour
//	}
//	if (collider->type == Collider::Type::WALL_RIGHT) {
//		position.x += 1.1f;
//	}
//
//	// fall on border
//	if (collider->type == Collider::Type::BORDER) {
//		falling = true;
//		idle = false;
//	}
//
//}
//
//void Zombie::Attack() {
//
//
//	attackCnt--;
//	if (attackCnt <= 0) {
//
//		//when player is in XplosionTrigger for 50 frames
//		currentAnim = &bodyXplodeL;
//
//		attacking = true;
//		App->scene->EnemyAttacking = true;
//		App->scene->enemyX = position.x;
//		App->scene->enemyY = position.y;
//		App->scene->EnemyCN = 1;
//		XplodeCnt--;
//		if (dir == Direction::LEFT) {
//			XplosionTrigger->SetPos(position.x - 20, position.y);
//			XplosionTrigger->SetProportions(60, 60);
//		}
//		if (dir == Direction::RIGHT) {
//			XplosionTrigger->SetPos(position.x + 20, position.y);
//			XplosionTrigger->SetProportions(42, 60);
//		}
//
//		//IMPORTANT the explosionCnt of the collider must be in sync with that of the explosion found in scene.cpp
//		Zspeed = 0;
//		if (XplodeCnt <= 0) {
//			if (exploded == false) {
//				hp = 0;
//				AttackCollider->SetPos(position.x - 10, position.y);
//				SelfDestruct->SetPos(position.x + 3, position.y + 5);
//
//				exploded = true;
//
//
//
//			}
//
//		}
//
//	}
//
//

