#ifndef __ZOMBIE_H__
#define __ZOMBIE_H__

#include "Enemy.h"
#include "Path.h"
#include "EnemyDeath.h"
#include "ModuleScene.h"

class Zombie : public Enemy {
public:

	Zombie(int x, int y, bool alignment);

	void Update() override;

	void OnCollision(Collider* collider) override;

	int XplodeCnt = 40;
	int attackCnt = 50;
	int attackCnt2 = 30;
	bool Alignment;
	bool isRising;
	bool Risen = false;
	

	bool Spawning;
	bool IsExploding;


	int CodeN = 1;
	float Zspeed = 0.12f;
	bool exploded = false;

	enum class Direction {
		LEFT,
		RIGHT
	};

	

	uint hitCountdown = 10;
	uint destroyedCountdown = 10;

	uint lethalAtt;
	

	Direction dir;
	//-------------------------------------------- Left Dir Animations ------------------------------------------------------//
	
	//------ Walk -------//

	Animation walkAnimL;

	//------ HeadXplode -------//

	Animation headXplodeL;

	//------ HeadlessWalk -------//

	Animation headlessWalkL;

	//------ BodyXplode -------//
	
	Animation bodyXplodeL;
	
	//------ DeathAnim -------//

	Animation deathAnimL;

	//------------------------------------------- Right Dir Animations ------------------------------------------------------//
	
	//------ Walk -------//

	Animation walkAnimR;

	//------ HeadXplode -------//

	Animation headXplodeR;

	//------ HeadlessWalk -------//

	Animation headlessWalkR;

	//------ BodyXplode -------//

	Animation bodyXplodeR;

	//------ DeathAnim -------//

	Animation deathAnimR;
	

	

	void Attack() override;

	
};



#endif