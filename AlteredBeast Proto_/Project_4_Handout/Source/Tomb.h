#ifndef __TOMB_H__
#define __TOMB_H__

#include "Enemy.h"
#include "Path.h"
#include "ModuleScene.h"

class Tomb : public Enemy {
public:

	Tomb(int x, int y, bool borderL , bool borderR, bool Zombie, float RiseTimer);

	void Update() override;

	void OnCollision(Collider* collider) override;

	void Rise();
	int Risecnt = 900;
	int Risecnt2 = 60;

	int zombieTimer = 600;
	int shaketimer = 20;
	bool shakeB = false;

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
	Animation shake;


	










};

#endif