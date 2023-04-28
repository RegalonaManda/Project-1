#ifndef __ZOMBIE_H__
#define __ZOMBIE_H__

#include "Enemy.h"
#include "Path.h"

class Zombie : public Enemy {
public:
	Zombie(int x, int y);

	void Update() override;

	void OnCollision(Collider* collider) override;

	bool alive = true;
	bool destroyed = false;

	enum class Direction {
		LEFT,
		RIGHT
	};

	int hp = 10;

	uint destroyedCountdown = 50;
	

private:

	Direction dir;
	Animation walkAnim;
	Animation deathAnim;

	
};



#endif