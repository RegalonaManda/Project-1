#ifndef __ZOMBIE_H__
#define __ZOMBIE_H__

#include "Enemy.h"
#include "Path.h"

class Zombie : public Enemy {
public:


	Zombie(int x, int y);

	void Update() override;

	void OnCollision(Collider* collider) override;



	enum class Direction {
		LEFT,
		RIGHT
	};

	

	uint hitCountdown = 10;
	uint destroyedCountdown = 10;
	

private:

	Direction dir;
	Animation walkAnim;
	Animation headXplode;
	Animation headlessWalk;
	Animation bodyXplode;
	Animation deathAnim;

	
};



#endif