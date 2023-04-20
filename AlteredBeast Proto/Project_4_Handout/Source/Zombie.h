#ifndef __ZOMBIE_H__
#define __ZOMBIE_H__

#include "Enemy.h"

class Zombie : public Enemy {

	Zombie(int x, int y);

	void Update() override;

	
};



#endif