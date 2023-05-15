#ifndef __ZOMBIE_H__
#define __ZOMBIE_H__

#include "Enemy.h"
#include "Path.h"
#include "EnemyDeath.h"
#include "ModuleScene.h"

class Graves : public Enemy {
public:

	Graves(int x, int y);

	void Update() override;

	void OnCollision(Collider* collider) override;

	int CodeN = 5;
	
	
};



#endif