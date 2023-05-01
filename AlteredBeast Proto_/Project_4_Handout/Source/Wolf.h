#ifndef __WOLF_H__
#define __WOLF_H__

#include "Enemy.h"
#include "Path.h"

class Wolf : public Enemy {
public:
	Wolf(int x, int y);

	void Update() override;

	void OnCollision(Collider* collider) override;

	bool idle = true;

	float wolfImpulse = 3.2f;

	

private:

	
	Animation idleAnimRight;
	Animation idleAnimLeft;
	Animation jumpAnimRight;
	Animation jumpAnimLeft;
	Animation deathAnimRight;
	Animation deathAnimLeft;


};



#endif