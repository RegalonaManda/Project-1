#include "Zombie.h"
#include "Application.h"
#include "ModuleCollisions.h"
#include "Enemy.h"
#include "ModuleEnemies.h"
#include "ModulePlayer.h"
#include "EnemyDeath.h"
#include "ModuleParticles.h"


//Calls the constructor of enemy class to save spawn position


Zombie::Zombie(int x, int y, bool alignment) : Enemy(x, y) {

	isRising = false;
	Alignment = alignment;
	Spawning = true;
	hp = 2;
	//-------------------------------------------- Left Dir Animations ------------------------------------------------------//

	//------ Walk -------//

	walkAnimL.PushBack({1, 1, 41, 68 });
	walkAnimL.PushBack({43, 1, 41, 68});
	walkAnimL.speed = 0.012f;
	walkAnimL.loop = true;
	

	//------ HeadXplode -------//

	headXplodeL.PushBack({85,1,41,68});
	headXplodeL.PushBack({132,1,41,68});
	headXplodeL.PushBack({127,1,41,68});
	headXplodeL.PushBack({132,1,41,68});
	headXplodeL.loop = false;
	headXplodeL.totalFrames = 4;
	headXplodeL.speed = 0.03f;

	//------ HeadlessWalk -------//

	headlessWalkL.PushBack({ 127,1,41,68 });
	headlessWalkL.PushBack({ 85,121,41,68 });
	headlessWalkL.loop = true;
	headlessWalkL.totalFrames = 2;
	headlessWalkL.speed = 0.008f;

	//------ BodyXplode -------//

	bodyXplodeL.PushBack({ 169,0,41,68 });
	bodyXplodeL.PushBack({ 127,0,41,68 });
	bodyXplodeL.PushBack({ 169,0,41,68 });
	bodyXplodeL.PushBack({ 127,0,41,68 });
	bodyXplodeL.loop = true;
	bodyXplodeL.totalFrames = 4;
	bodyXplodeL.speed = 0.05f;

	//------ DeathAnim -------//

	deathAnimL.PushBack({ 127,1,41,68 });
	deathAnimL.PushBack({ 132,1,41,68 });
	deathAnimL.PushBack({ 127,1,41,68 });
	deathAnimL.PushBack({ 132,1,41,68 });
	deathAnimL.loop = false;
	deathAnimL.totalFrames = 4;
	deathAnimL.speed = 0.03f;

	//------------------------------------------- Right Dir Animations ------------------------------------------------------//

	//------ Walk -------//

	walkAnimR.PushBack({ 587, 4, 41,68 });
	walkAnimR.PushBack({ 547, 4, 41, 68 });
	walkAnimR.speed = 0.008f;
	walkAnimR.loop = true;

	//------ HeadXplode -------//

	headXplodeR.PushBack({496,4,41,68});
	headXplodeR.PushBack({453,4,41,68});
	headXplodeR.PushBack({448,4,41,68});
	headXplodeR.PushBack({453,4,41,68});
	headXplodeR.loop = false;
	headXplodeR.totalFrames = 4;
	headXplodeR.speed = 0.03f;

	//------ HeadlessWalk ------- (notfinished)//

	headlessWalkR.PushBack({ 127,1,41,68 });
	headlessWalkR.PushBack({ 85,121,41,68 });
	headlessWalkR.loop = true;
	headlessWalkR.totalFrames = 2;
	headlessWalkR.speed = 0.008f;

	//------ BodyXplode -------(notfinished)//

	bodyXplodeR.PushBack({ 169,0,41,68 });
	bodyXplodeR.PushBack({ 127,0,41,68 });
	bodyXplodeR.PushBack({ 169,0,41,68 });
	bodyXplodeR.PushBack({ 127,0,41,68 });
	bodyXplodeR.loop = true;
	bodyXplodeR.totalFrames = 4;
	bodyXplodeR.speed = 0.05f;

	//------ DeathAnim -------(notfinished)//

	deathAnimR.PushBack({ 127,1,41,68 });
	deathAnimR.PushBack({ 132,1,41,68 });
	deathAnimR.PushBack({ 127,1,41,68 });
	deathAnimR.PushBack({ 132,1,41,68 });
	deathAnimR.loop = false;
	deathAnimR.totalFrames = 4;
	deathAnimR.speed = 0.03f;



	//Default Direction
	dir = Direction::RIGHT;
	IsExploding = false;
	//at 1 hit, head explodes, continues
	//at 2 hits, disappears
	
	//if enemy gets close enough to player, will explode -> yellow sprite - normal sprite - disappears (no shake)

	Ecollider = App->collisions->AddCollider({ 400, 120, 24, 60 }, Collider::Type::ENEMY, (Module*)App->enemies);
	AttackCollider = App->collisions->AddCollider({ 412,140,50,0 }, Collider::Type::ENEMY_SHOT, (Module*)App->player);
	XplosionTrigger = App->collisions->AddCollider({ 470,150,38,60 }, Collider::ATTACK_XplosionTrigger, (Module*)App->enemies);
	
	SelfDestruct = App->collisions->AddCollider({ 5,5,5,5 }, Collider::ENEMY_SELF_DESTRUCT, (Module*)App->enemies);

	//attack starts oob
	AttackCollider->SetPos(-1000, -1000);
	
	SelfDestruct->SetPos(-1000, -1000);

	lethalAtt = App->audio->LoadFx("Assets/FX/Lethal_Punch.wav");
	//default anim walk left
	currentAnim = &walkAnimL;

}

void Zombie::Update() {
	//Aplicar para el resto de enemigos
	if (App->render->camera.dynamicSpeed == 0.1) {
		if (alive == false) {
			App->render->camera.dynamicSpeed = 0.5;
		}
	}
	//----------------------

	position.x -= Zspeed;
	//the 350 needs reworks

	if (Alignment == 0) {

		if (position.x <= App->player->position.x - 200 && position.y > 120)
		{

			isRising = true;

		}
	}else {
			if (position.x <= App->player->position.x + 150 && position.y > 120)
			{

				isRising = true;

			}
		}
	


	if (isRising == true) {
		position.y -= 1;
	}

	if (position.y <= 120) { Spawning = false; isRising = false; Risen = true; }




	//if zombie is behind the player change the direction

	if (position.x <= App->player->position.x) 
	{
		dir = Direction::RIGHT; 
	}
	else { dir = Direction::LEFT; }

	if (Spawning == false) {
		if (dir == Direction::RIGHT) {
			
			Zspeed = (Zspeed + 0.4f) * -1;
			
		}
	}
	if (dir == Direction::LEFT && currentAnim != &headXplodeL && hp > 1) {
		currentAnim = &walkAnimL;
	}
	if (dir == Direction::RIGHT && currentAnim != &headXplodeR && hp > 1) {
		currentAnim = &walkAnimR;
	}
	if (alive)
	{


		/*if (hp == 2 && attacking == false) { currentAnim = &walkAnimL; }*/
		if (hp == 1 && hitByPlayer)
		{
			hitCountdown--;
			if (hitCountdown <= 0) {
				if (dir == Direction::LEFT) {
					currentAnim = &headXplodeL;
				}
				else if (dir == Direction::RIGHT) {
					currentAnim = &headXplodeR;
				}
				
			}
			if (dir == Direction::LEFT) {
				if (headXplodeL.HasFinished()) {
					currentAnim = &headlessWalkL;
					hitByPlayer = false;
				}
			}
			if (dir == Direction::RIGHT) {
				if (headXplodeR.HasFinished()) {
					currentAnim = &headlessWalkR;
					hitByPlayer = false;
				}
			}
		}
	}


	if (attacking == true) { Attack(); }




	Ecollider->SetPos(2000, 2000);
	// The collider of the enemy changes depending on the position of the direction
	if (attackCnt > 0) {
		if (dir == Direction::LEFT) {
			XplosionTrigger->SetPos(position.x-10, position.y);
			/*XplosionTrigger->SetPos(position.x - 10, position.y);*/
		}
		else if (dir == Direction::RIGHT) {
			XplosionTrigger->SetPos(position.x+6, position.y);
		}
		
	}




	if (falling == true) {
		position.y += 3;
	}






	currentAnim->Update();
	Enemy::Update();
	
}

void Zombie::OnCollision(Collider* collider) {
	//Aplicar para el resto de enemigos
	if (collider == App->scene->backCamLimit || collider == App->scene->frontCamLimit) {

		App->render->camera.dynamicSpeed = 0.1;
	}
	//---------------------------------------------
	if (collider->type == Collider::Type::PLAYER_SHOT) {
		
		
		destroyedCountdown--;
		if (destroyedCountdown <= 0) {
			hp-= App->player->attack;
			App->audio->PlayFx(lethalAtt, 3);
			hitByPlayer = true;
			destroyedCountdown = 20;
			if (hp <= 0) {
				App->player->score += 100;
				App->particles->AddParticle(App->particles->zombie, position.x, position.y-20);
			}

		}

	}
	if (hp <= 0 && attacking == false) {
		Ecollider->SetPos(-1000, -1000);
		alive = false;

		
		App->scene->EnemyCN = 1;
		App->scene->HasEnemyDied = true; 
		App->scene->enemyX = position.x;
		App->scene->enemyY = position.y;
	}

	//make the explosion kill the zombie

	//rise above paltform
	if (collider->type == Collider::Type::PLATFORM) {
		if (collider != App->scene2->Ground) {
			position.y -= 1.8f;
		}
		falling = false;
		idle = true;
	}

	// stop againts wall
	if (collider->type == Collider::Type::WALL) {
		position.x -= 1.1f;
		// we could also stop their movement behaviour
	}
	if (collider->type == Collider::Type::WALL_RIGHT) {
		position.x += 1.1f;
	}

	// fall on border
	if (collider->type == Collider::Type::BORDER) {
		falling = true;
		idle = false;
	}

	
}

void Zombie::Attack() {

	
		attackCnt--;
		if (attackCnt <= 0) {

			//when player is in XplosionTrigger for 50 frames
			currentAnim = &bodyXplodeL;
		
			attacking = true;
			App->scene->EnemyAttacking = true;
			App->scene->enemyX = position.x;
			App->scene->enemyY = position.y;
			App->scene->EnemyCN = 1;
			XplodeCnt--;
			if (dir == Direction::LEFT) {
				XplosionTrigger->SetPos(position.x - 20, position.y);
				XplosionTrigger->SetProportions(60, 60);
			}
			if (dir == Direction::RIGHT) {
				XplosionTrigger->SetPos(position.x + 20, position.y);
				XplosionTrigger->SetProportions(42, 60);
			}
			
			//IMPORTANT the explosionCnt of the collider must be in sync with that of the explosion found in scene.cpp
			Zspeed = 0;
			if (XplodeCnt <= 0) {
				if (exploded == false) {
					hp = 0;
					AttackCollider->SetPos(position.x - 10, position.y);
					SelfDestruct->SetPos(position.x + 3, position.y + 5);
					
					exploded = true;
					

					
				}

			}

		}
	

}

