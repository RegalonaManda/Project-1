#ifndef __MODULE_PLAYER_H__
#define __MODULE_PLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;
struct Collider;

class ModulePlayer : public Module
{
public:
	// Constructor
	ModulePlayer();

	// Destructor
	~ModulePlayer();

	enum class Direction {
		LEFT,
		RIGHT
	};

	enum class AirState {
		CROUCH,
		GROUND,
		AIRBORN
	};

	enum class Transform {
		DEFAULT,
		POWER1,
		POWER2,
		WOLF

	};

	Direction dir;
	AirState airSt;
	Transform tranSt;

	// Called when the module is activated
	// Loads the necessary textures for the player
	bool Start() override;

	// Called at the middle of the application loop
	// Processes new input and handles player movement
	update_status Update() override;

	// Called at the end of the application loop
	// Performs the render call of the player sprite
	update_status PostUpdate() override;

public:
	float impulse = 3.2f;

	// Position of the player in the map
	fPoint position;

	// The speed in which we move the player (pixels per frame)
	int speed = 2;

	float AirSpeed = 1.3f;

	// The player spritesheet loaded into an SDL_Texture
	SDL_Texture* texture = nullptr;
	
	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	Animation* currentAnimation = nullptr;

	// The player's collider
	Collider* Pcollider = nullptr;
	Collider* attackCollider = nullptr;
	Collider* kickCollider = nullptr;

	// A flag to detect when the player has been destroyed
	bool destroyed = false;

	void OnCollision(Collider* c1, Collider* c2) override;

	uint destroyedCountdown = 120;

	// A set of animations
	Animation idleAnimRight;
	Animation idleAnimLeft;
	Animation forwardAnim;
	Animation backAnim;
	Animation punchAnimLeft;
	Animation hitAnimRight;
	Animation hitAnimLeft;
	Animation deathAnimRight;
	Animation deathAnimLeft;
	Animation punchAnimRight;
	Animation crouchAnimLeft;
	Animation crouchAnimRight;
	Animation crouchPunchLeft;
	Animation crouchPunchRight;
	Animation kickAnimRight;
	Animation kickAnimLeft;
	Animation kickCrouchRight;
	Animation kickCrouchLeft;

	//AirBorn animations
	Animation jumpRight;
	Animation jumpLeft;

	// Font score index
	float score = 000;
	int scoreFont = -1;
	char scoreText[10] = { "\0" };
	float deathdist = 2;

};

#endif //!__MODULE_PLAYER_H__