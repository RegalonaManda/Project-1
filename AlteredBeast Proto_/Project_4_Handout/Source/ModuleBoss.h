#ifndef __MODULE_BOSS_H__
#define __MODULE_BOSS_H__

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;
struct Collider;

class Head;

class ModuleBoss : public Module
{
public:
	// Constructor
	ModuleBoss(bool startEnabled);

	// Destructor
	~ModuleBoss();


	bool Start() override;


	update_status Update() override;


	update_status PostUpdate() override;


	void OnCollision(Collider* c1, Collider* c2) override;

public:

	fPoint position;

	
	int speed;

	
	SDL_Texture* texture = nullptr;

	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction

	Animation* currentAnim;

	Animation Anim;

	Animation DisappearAnim;

	fPoint spawnPos;

	Collider* collider = nullptr;

	// A flag to detect when the player has been destroyed
	bool beaten = false;

	// Sound effects indices
	uint welcomeDoom = 0;
	uint headThrow = 0;
	uint deathExpl = 0;

};

class Head {

	fpoint position;














};

#endif 
