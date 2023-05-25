#ifndef __DRAGON_H__
#define __DRAGON_H__

#include "Enemy.h"
#include "Path.h"
#include "ModuleScene.h"

class Dragon : public Enemy {
public:

	Dragon(int x, int y);

	void Update() override;

	void OnCollision(Collider* collider) override;


	int CodeN = 6;



	enum class Direction {
		LEFT,
		RIGHT
	};



	uint hitCountdown = 10;
	uint destroyedCountdown = 10;

	uint lethalAtt;


	int followCnt = 600;
	int checkPos = 50;

	int attackCnt = 30;

	const int initialY = position.y;

	int finalY;

	fPoint targetPos;

	void Follow();

	void Attack() override;

	void FlyBack();

	Animation FlyR;


	





};

#endif