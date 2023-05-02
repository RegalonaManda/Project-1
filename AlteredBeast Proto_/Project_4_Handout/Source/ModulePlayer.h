#ifndef __MODULE_PLAYER_H__
#define __MODULE_PLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "ModuleAudio.h"
#include "Globals.h"
#include "PlayerAnimations.h"
#include "Enemy.h"

#define Gravity 0.07f;

struct SDL_Texture;
struct Collider;

class ModulePlayer : public Module
{
public:

	// Constructor
	ModulePlayer(bool startEnabled);

	// Destructor
	~ModulePlayer();
	int attack = 1;

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
	bool transforming = false;

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
	bool start = false;
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
	Collider* Deathcollider = nullptr;
	// A flag to detect when the player has been destroyed
	bool destroyed = false;

	int landing = 30;
	bool landed = false;

	void OnCollision(Collider* c1, Collider* c2) override;

	void KnockBack();

	void playerDamaged();

	float destroyedCountdown = 120;

	Enemy* knocked = nullptr;

public:

	PlayerAnim AllAnimations;

	// Font score index
	uint score = 0;
	int scoreFont = 9;
	char scoreText[10] = { "\0" };
	

	bool hitEnemy = false;

	//Sound FX
	uint playerDeathFX;
	uint nonLethalAtt;
	uint lethalAtt;
	uint loseHP;
	uint powerUp;

	friend class PlayerAnim; 

	bool firstHit = false;
	bool KilledBoss = false;
	int FadeCnt = 120;
};


#endif //!__MODULE_PLAYER_H__