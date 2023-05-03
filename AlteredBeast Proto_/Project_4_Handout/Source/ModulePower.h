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
	
	Animation* currentAnim;
	
	Animation Anim;

	Animation DissapearAnim;

	float waveRatio = 0.0f;
	float waveRatioSpeed = 0.05f;
	int waveHeight = 20;

	int spawn_y = 0;
	fPoint spawnPos;

	Collider* collider = nullptr;

	// A flag to detect when the player has been destroyed
	bool gotten = false;

	// Sound effects indices
	uint laserFx = 0;
	uint explosionFx = 0;

};

#endif 