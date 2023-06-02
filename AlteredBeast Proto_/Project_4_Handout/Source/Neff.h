#ifndef __NEFF_H__
#define __NEFF_H__

#include "Enemy.h"
#include "Path.h"
#include "EnemyDeath.h"
#include "ModuleScene.h"

class Neff : public Enemy {
public:

	Neff(int x, int y, bool last);

	void Update() override;

	void OnCollision(Collider* collider) override;

	int XplodeCnt = 40;
	int attackCnt = 50;
	int attackCnt2 = 30;

	int CodeN = 4;

	bool exploded = false;

	int scroll = 1200;

	int bossCountdown = 200;

	bool last;

	enum class Direction {
		LEFT,
		RIGHT
	};

	Collider* rangeCollider = nullptr;
	

	uint hitCountdown = 10;
	uint destroyedCountdown = 10;

	uint lethalAtt;

	bool reachedBoss;

	
	Animation RayAtt;

	Animation escaped;

	void Attack() override;

	bool run = false;
	int runCnt = 180;

	void HeadOut();

	void ActivateBoss();








};

#endif