#ifndef __WOLF_H__
#define __WOLF_H__

#include "Enemy.h"
#include "Path.h"
#include "ModuleEnemies.h"

class Wolf : public Enemy {
public:
	Wolf(int x, int y);

	void Update() override;

	void OnCollision(Collider* collider) override;

	bool idle = true;

	float wolfImpulse = 3.2f;

	int CodeN = 2;

	int JumpCnt = 90;

	

private:

	
	Animation idleAnimRight;
	Animation idleAnimLeft;
	Animation jumpAnimRight;
	Animation jumpAnimLeft;
	Animation deathAnimRight;
	Animation deathAnimLeft;


};



#endif