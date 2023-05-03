#ifndef __BROWNWOLF_H__
#define __BROWNWOLF_H__

#include "Enemy.h"
#include "Path.h"
#include "ModuleEnemies.h"


class BrownWolf : public Enemy {
public:
	BrownWolf(int x, int y);

	float Y0 = 0;

	void Update() override;

	void OnCollision(Collider* collider) override;

	bool idle = true;

	bool jump = false;

	float wolfImpulse = 2.0f;
	float knockImpulse = 4.0f;

	int CodeN = 3;

	int JumpCnt = 200;

	int knockH = 0;

	bool stunt = true;

	bool knocked = false;

private:


	Animation idleAnimRight;
	Animation idleAnimLeft;
	Animation jumpAnimRight;
	Animation jumpAnimLeft;
	Animation deathAnimRight;
	Animation deathAnimLeft;


};



#endif