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

	Animation walkAnimR;
	Animation walkAnimL;

	Animation headXplodeR;
	Animation headXplodeL;
	Animation headlessWalk;
	
	Animation bodyXplode;
	Animation deathAnim;

	

	void Attack() override;

	
};



#endif