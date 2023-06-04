#include "Skull.h"
#include "Application.h"
#include "ModuleCollisions.h"
#include "Enemy.h"
#include "ModuleEnemies.h"
#include "ModulePlayer.h"
#include "ModuleParticles.h"



//Calls the constructor of enemy class to save spawn position


Skull::Skull(int x, int y) : Enemy(x, y) {

	isFalling = false;

	Spawning = true;
	hp = 2;
	hitByPlayer = false;
	//-------------------------------------------- Left Dir Animations ------------------------------------------------------//

	//------- Idle -------//

	idleAnimL.PushBack({ 1, 343, 84, 68 });
	idleAnimL.loop = true;

	//------ Walk -------//

	
	walkAnimL.PushBack({86,343,84, 68 });
	walkAnimL.PushBack({ 1, 343, 84, 68 });
	walkAnimL.speed = 0.012f;
	walkAnimL.loop = true;
	walkAnimL.totalFrames = 2;

	//------ Crouch -------//

	crouchAnimL.PushBack({ 256,343,84, 68 });
	crouchAnimL.PushBack({ 256,343,84, 68 });
	crouchAnimL.speed = 0.05f;
	crouchAnimL.loop = false;
	

	//------- Punch --------//

	punchAnimL.PushBack({171,343,84,68});
	punchAnimL.PushBack({ 1, 343, 84, 68 });
	punchAnimL.PushBack({ 171,343,84,68 });
	punchAnimL.PushBack({ 1, 343, 84, 68 });
	punchAnimL.PushBack({ 1, 343, 84, 68 });
	punchAnimL.PushBack({ 1, 343, 84, 68 });
	punchAnimL.speed = 0.04f;
	punchAnimL.loop = false;

	//------------------------------------------- Right Dir Animations ------------------------------------------------------//

	//------- Idle -------//

	idleAnimR.PushBack({ 1, 412, 84, 68 });
	idleAnimR.loop = true;


	//------ Walk -------//

	
	walkAnimR.PushBack({ 86,412, 84, 68 });
	walkAnimR.PushBack({ 1, 412, 84, 68 });
	walkAnimR.speed = 0.008f;
	walkAnimR.loop = true;

	//------ Punch -------//

	punchAnimR.PushBack({ 171,412,84,68 });
	punchAnimR.PushBack({ 1, 412, 84, 68 });
	punchAnimR.PushBack({ 171,412,84,68 });
	punchAnimR.PushBack({ 1, 412, 84, 68 });
	punchAnimR.PushBack({ 1, 412, 84, 68 });
	punchAnimR.PushBack({ 1, 412, 84, 68 });
	punchAnimR.speed = 0.04f;
	punchAnimR.loop = false;

	//------ Crouch -------//

	crouchAnimR.PushBack({ 256,412,84,68 });
	crouchAnimR.PushBack({ 256,412,84,68 });
	crouchAnimR.speed = 0.05f;
	crouchAnimR.loop = false;

	SkSt = SkullState::STILL;

	Ecollider = App->collisions->AddCollider({ 400, 120, 24, 60 }, Collider::Type::ENEMY, (Module*)App->enemies);

	//change name to range
	Range = App->collisions->AddCollider({0,0, 84, 68}, Collider::Type::RANGE, (Module*)App->enemies);
	
	AttackCollider = App->collisions->AddCollider({ 0,0,30,15 }, Collider::Type::ENEMY_SHOT, (Module*)App->player);

	//attack starts oob
	
	Range->SetPos(position.x, position.y);
	AttackCollider->SetPos(-1200, -1200);

	lethalAtt = App->audio->LoadFx("Assets/FX/Lethal_Punch.wav");
	//default anim walk left
	currentAnim = &walkAnimL;
	destroyed = false;
	position.y -= 50;

}

void Skull::Update() {


	if (!destroyed) {
		if (SkSt != ATTACK) {
			AttackCollider->SetPos(-1500, 1200);
		}


		if (Range->Intersects(App->player->Pcollider->rect) == false) {
			stillCountdown--;
			if (SkSt == STILL && stillCountdown <= 0) {
				SkSt = ADVANCE;
				if (dir == Direction::LEFT)
				{
					currentAnim = &walkAnimL;
					position.x -= 10.0f;
				}
				else if (dir == Direction::RIGHT)
				{
					currentAnim = &walkAnimR;
					position.x += 10.0f;
				}
				walkCountdown = 30;

			}

			walkCountdown--;
			if (SkSt == ADVANCE && walkCountdown <= 0) {
				SkSt = STILL;
				if (dir == Direction::LEFT) { currentAnim = &idleAnimL; }
				else { currentAnim = &idleAnimR; }
				stillCountdown = 20;
			}


		}
		else {
			SkSt = ATTACK;
			if (dir == Direction::LEFT) {
				currentAnim = &punchAnimL;
				AttackCollider->SetPos(position.x, position.y + 6);

			}
			else if (dir == Direction::RIGHT) {
				currentAnim = &punchAnimR;
				AttackCollider->SetPos(position.x + 58, position.y + 6);
			}

		}

		if (punchAnimL.HasFinished() || punchAnimR.HasFinished()) {
			SkSt = BACKTRACK;
			stillCountdown = 20;
			currentAnim->Reset();
			currentAnim->loopCount = 0;
			backtrackCnt = 30;
			AttackCollider->SetPos(-1500, 1200);

			
		}

		if (SkSt == BACKTRACK) {
			BackTrack();
		}
		else {
			if (dir == Direction::LEFT) {
				backTrackDist = position.x + 30;
			}
			else if (dir == Direction::RIGHT) {
				backTrackDist = position.x - 30;
			}
		}

		if (hp == 1 && hitByPlayer) {
			punchAnimL.Reset();
			punchAnimL.loopCount = 0;
			punchAnimR.Reset();
			punchAnimR.loopCount = 0;

			SkSt = HIT;
			if (dir == Direction::LEFT) {
				currentAnim = &crouchAnimL;

			}
			else if (dir == Direction::RIGHT) {
				currentAnim = &crouchAnimR;
			}
		}

		if (SkSt == HIT && (crouchAnimL.HasFinished() || crouchAnimR.HasFinished())) {
			currentAnim->Reset();
			currentAnim->loopCount = 0;
			hitByPlayer = false;
			SkSt = BACKTRACK;
		}

		//if skull is behind the player change the direction

		if (position.x <= App->player->position.x)
		{
			dir = Direction::RIGHT;
		}
		else {
			dir = Direction::LEFT;
		}

		currentAnim->Update();

		Enemy::Update();

		Ecollider->SetPos(position.x + 30, position.y+10);
		Range->SetPos(position.x, position.y);
		if (falling == true) {
			position.y += 3;
		}
	}
	else if (destroyed) {

	   AttackCollider->SetPos(-1500, 1200);
	   Range->SetPos(-1500, 1200);

	}
}

void Skull::BackTrack() {

	backtrackCnt--;
	if (backtrackCnt <= 0) {
		
		if (dir == Direction::LEFT) {
			if (position.x <= backTrackDist) {
				currentAnim = &walkAnimL;
				position.x += 0.5f;
				
			}
			else {
				SkSt = ADVANCE;
				
			}
			
		}
		else if (dir == Direction::RIGHT) {
			if (position.x >= backTrackDist) {
				currentAnim = &walkAnimR;
				position.x -= 0.5f;
			}
			else {
				SkSt = ADVANCE;
			}
		}
	}
}

void Skull::OnCollision(Collider* collider) {
	if (collider->type == Collider::Type::PLAYER_SHOT) {

		hitByPlayer = true;
		SkSt = HIT;
		if (destroyedCountdown == 20) 
		{ 
			hp -= App->player->attack;
		}
		destroyedCountdown--;
		if (destroyedCountdown <= 0) {

			App->audio->PlayFx(lethalAtt, 5);
			
			destroyedCountdown = 20;

		}
	}

	if (hp <= 0) {
		Ecollider->SetPos(-1000, -1000);
		alive = false;
		App->particles->AddParticle(App->particles->Skull, position.x, position.y - 20);
		App->scene->EnemyCN = 6;
		App->scene->HasEnemyDied = true;
		App->scene->enemyX = position.x;
		App->scene->enemyY = position.y;
     	destroyed = true;
		App->player->score += 100;
		AttackCollider->SetPos(-1500, 1200);
		AttackCollider->SetFloatPos(-1500, 1200);
		Range->SetPos(-1500, 1200);
	}

	//rise above paltform
	if (collider->type == Collider::Type::PLATFORM) {
		
			position.y -= 1.8f;
		
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
