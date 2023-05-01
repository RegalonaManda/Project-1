#ifndef __NEFF_H__
#define __NEFF_H__

#include "Enemy.h"
#include "Path.h"
#include "EnemyDeath.h"
#include "ModuleScene.h"

class Neff : public Enemy {
public:

	Neff(int x, int y);

	void Update() override;

	void OnCollision(Collider* collider) override;

	int XplodeCnt = 40;
	int attackCnt = 50;
	int attackCnt2 = 30;

	int CodeN = 1;

	bool exploded = false;

	enum class Direction {
		LEFT,
		RIGHT
	};



	uint hitCountdown = 10;
	uint destroyedCountdown = 10;

	uint lethalAtt;


	Animation CapeAnimation;
	



	void Attack() override;












};

#endif