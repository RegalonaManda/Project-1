#ifndef __MODULE_PLAYER_H__
#define __MODULE_PLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "ModuleAudio.h"
#include "Globals.h"

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
		AIRBORN,
		LANDING
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

	//HP And lives
	int hp = 3;
	int lives = 3;
	bool iFrames = false;
	int iTimer = 30;

	bool GodMode = false;

	float knockImpulse = 0;


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

	int landing = 30;
	bool landed = false;

	void OnCollision(Collider* c1, Collider* c2) override;

	void KnockBack();

	float destroyedCountdown = 120;

	// attack animations
	Animation idleAnimRight;
	Animation idleAnimLeft;
	Animation forwardAnim;
	Animation backAnim;
	Animation punchAnimLeft;
	
	Animation punchAnimRight;
	Animation crouchAnimLeft;
	Animation crouchAnimRight;
	Animation crouchPunchLeft;
	Animation crouchPunchRight;
	Animation kickAnimRight;
	Animation kickAnimLeft;
	Animation kickCrouchRight;
	Animation kickCrouchLeft;

	//damage animations
	Animation knockBackLeft;
	Animation knockBackRight;
	Animation hitAnimRight;
	Animation hitAnimLeft;
	Animation HitContinueLeft;
	Animation HitContinueRight;
	Animation deathAnimRight;
	Animation deathAnimLeft;

	//AirBorn animations
	Animation jumpRight;
	Animation jumpLeft;
	Animation airPunchLeft;
	Animation airPunchRight;
	Animation airKickLeft;
	Animation airKickRight;

	Animation LandingRight;
	Animation LandingLeft;



	// Font score index
	uint score = 0;
	int scoreFont = 9;
	char scoreText[10] = { "\0" };
	float deathdist = 2;

	bool hitEnemy = false;

	//Sound FX
	uint playerDeathFX;
	uint nonLethalAtt;
	uint lethalAtt;
	uint loseHP;
	uint powerUp;

};

#endif //!__MODULE_PLAYER_H__