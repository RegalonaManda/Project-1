#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "p2Point.h"
#include "Animation.h"
#include "ModulePlayer.h"

struct SDL_Texture;
struct Collider;

class Enemy
{
public:
	// Constructor
	// Saves the spawn position for later movement calculations
	Enemy(int x, int y);

	// Destructor
	virtual ~Enemy();

	// Returns the enemy's collider
	const Collider* GetCollider() const;

	const Collider* GetRangeCollider() const;

	const Collider* GetDestructCollider() const;

	// Called from inhering enemies' Udpate
	// Updates animation and collider position
	virtual void Update();

	// Called from ModuleEnemies' Update
	virtual void Draw();

	// Collision response
	// Triggers an animation and a sound fx
	virtual void OnCollision(Collider* collider);

	virtual void Attack();

public:

	enum class Direction {
		LEFT,
		RIGHT
	};
	Direction dir;

	// The current position in the world
	fPoint position;

	

	// The enemy's texture
	SDL_Texture* texture = nullptr;

	bool attacking = false;
	bool destroyed = false;
	bool alive = true;
	bool hitByPlayer = false;
	int hp;
	uint hitCountdown;
	uint destroyedCountdown;

	Collider* AttackCollider = nullptr;

protected:
	// A ptr to the current animation
	Animation* currentAnim = nullptr;
	Collider* Ecollider = nullptr;
	
	Collider* Range = nullptr;
	Collider* SelfDestruct = nullptr;
	// Original spawn position. Stored for movement calculations
	fPoint spawnPos;
	
	
};

#endif // __ENEMY_H__