#ifndef __TOMB_H__
#define __TOMB_H__

#include "Enemy.h"
#include "Path.h"
#include "EnemyDeath.h"
#include "ModuleScene.h"

class Tomb : public Enemy {
public:

	Tomb(int x, int y, bool borderL , bool borderR, bool Zombie);

	void Update() override;

	void OnCollision(Collider* collider) override;

	void Rise();
	int Risecnt = 170;
	int Risecnt2 = 60;
	bool Risen = false;
	bool Rising = false;

	bool TombBorderL = false;
	bool TombBorderR = false;

	bool Zombie = false;

	int Fy;


	uint hitCountdown = 10;
	uint destroyedCountdown = 10;

	uint lethalAtt;


	Animation idle;


	Collider* WallLCollider;
	Collider* WallRCollider;
	Collider* PlatformCollider;

	Collider* LBorder = nullptr;
	Collider* RBorder = nullptr;











};

#endif