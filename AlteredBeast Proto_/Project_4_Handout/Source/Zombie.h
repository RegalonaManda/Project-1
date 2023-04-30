#ifndef __ZOMBIE_H__
#define __ZOMBIE_H__

#include "Enemy.h"
#include "Path.h"
#include "EnemyDeath.h"
#include "ModuleScene.h"

class Zombie : public Enemy {
public:


	Zombie(int x, int y);

	void Update() override;

	void OnCollision(Collider* collider) override;

	int XplodeCnt = 40;
	int attackCnt = 50;
	int attackCnt2 = 30;


	enum class Direction {
		LEFT,
		RIGHT
	};

	

	uint hitCountdown = 10;
	uint destroyedCountdown = 10;
	

	Direction dir;

	Animation walkAnim;
	Animation headXplode;
	Animation headlessWalk;
	Animation bodyXplode;
	Animation deathAnim;

	

	void Attack() override;

	
};



#endif