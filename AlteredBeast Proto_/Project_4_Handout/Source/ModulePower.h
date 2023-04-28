#ifndef __MODULE_POWER_H__
#define __MODULE_POWER_H__

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;
struct Collider;

class ModulePower : public Module
{
public:
	// Constructor
	ModulePower(bool startEnabled);

	// Destructor
	~ModulePower();

	
	bool Start() override;

	
	update_status Update() override;

	
	update_status PostUpdate() override;


	void OnCollision(Collider* c1, Collider* c2) override;

public:
	
	fPoint position;

	// The speed in which we move the player (pixels per frame)
	int speed = 1;

	// The player spritesheet loaded into an SDL_Texture
	SDL_Texture* texture = nullptr;

	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	

	// A set of animations
	Animation idleAnim;
	Animation upAnim;
	Animation downAnim;

	// The player's collider
	Collider* collider = nullptr;

	// A flag to detect when the player has been destroyed
	bool destroyed = false;

	// Sound effects indices
	uint laserFx = 0;
	uint explosionFx = 0;

};

#endif //!__MODULE_PLAYER_H__