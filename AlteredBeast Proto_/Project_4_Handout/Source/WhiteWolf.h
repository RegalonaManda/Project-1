#ifndef __WHITEWOLF_H__
#define __WHITEWOLF_H__

#include "Enemy.h"
#include "Path.h"
#include "ModuleEnemies.h"


class WhiteWolf : public Enemy {
public:
	WhiteWolf(int x, int y);

	float Y0 = 0;

	void Update() override;

	void OnCollision(Collider* collider) override;

	bool idle = true;

	bool jump = false;

	float wolfImpulse = 2.0f;
	float knockImpulse = 4.0f;

	int CodeN = 2;

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