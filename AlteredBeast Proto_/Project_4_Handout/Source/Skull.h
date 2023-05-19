#ifndef __SKULL_H__
#define __SKULL_H__

#include "Enemy.h"
#include "Path.h"
#include "EnemyDeath.h"
#include "ModuleScene.h"

class Skull : public Enemy {
public:

	Skull(int x, int y);

	void Update() override;

	//void OnCollision(Collider* collider) override;

	int XplodeCnt = 40;
	int attackCnt = 50;
	int attackCnt2 = 30;
	bool Alignment;
	bool isFalling;
	bool onGround = false;

	enum SkullState {
		STILL,
		ADVANCE,
		BACKTRACK,
		ATTACK,
		HIT
	};

	SkullState SkSt;

	enum SkullSpawn {
		GROUNDLEFT,
		GROUNDRIGHT,
		SKYLEFT,
		SKYRIGHT
	};
	
	SkullSpawn SkSpwn;

	enum class Direction {
		LEFT,
		RIGHT
	};

	Direction dir;

	bool Spawning;
	bool IsExploding;


	int CodeN = 6;
	float Zspeed = 0.12f;
	bool exploded = false;




	uint walkCountdown = 30;
	uint stillCountdown = 20;

	uint hitCountdown = 10;
	uint destroyedCountdown = 10;

	uint lethalAtt;


	
	//-------------------------------------------- Left Dir Animations ------------------------------------------------------//
	//------ Idle --------//

	Animation idleAnimL;

	//------ Walk -------//

	Animation walkAnimL;

	//------ Punch ------//
	Animation punchAnimL;

	//------ Crouch ------//

	Animation crouchAnimL;

	//------ DeathAnim -------//

	Animation deathAnimL;

	//------------------------------------------- Right Dir Animations ------------------------------------------------------//

	//------ Idle --------//

	Animation idleAnimR;

	//------ Walk -------//

	Animation walkAnimR;

	//------ Punch ------//

	Animation punchAnimR;

	//------ Crouch -------//

	Animation crouchAnimR;

	//------ DeathAnim -------//

	Animation deathAnimR;




	/*void Attack() override;*/


};

#endif