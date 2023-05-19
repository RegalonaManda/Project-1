#ifndef __MODULE_PLAYER_H__
#define __MODULE_PLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "ModuleAudio.h"
#include "Globals.h"
#include "PlayerAnimations.h"
#include "Enemy.h"
#include "WereWolf.h"

#define Gravity 0.07f;

struct SDL_Texture;
struct Collider;

enum class Direction {
	LEFT,
	RIGHT
};

class WolfShot {


public:

	Animation* CurrentShot = nullptr;

	Animation despawned;
	Animation GrowAnim;
	Animation TravelAnim;
	Animation ExplodeAnim;

	Animation GrowAnimL;
	Animation TravelAnimL;

	SDL_Texture* ShotText = nullptr;
	Collider* collider = nullptr;

	fPoint ShotPosition;

	SDL_Rect wolfRec;

	Direction dir;

	bool destroyed = true;
	bool exploded = false;


};

class ModulePlayer : public Module
{
public:

	// Constructor
	ModulePlayer(bool startEnabled);

	// Destructor
	~ModulePlayer();
	int attack = 1;
	bool idle = true;

	

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

	//----------Control---Functions-------------

	void DefaultMovement();

	void Power1Movement();

	void WereWolfMovement();

	void WolfKick();

	void FireBallMovement();

	WolfShot FBall;

	Direction dir;
	AirState airSt;
	Transform tranSt;
	bool transforming = false;
	bool wolfPunch = false;

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
	int exTimer = 40;

	bool startExTimer = false;
	bool GodMode = false;
	bool start = false;
	float knockImpulse = 2;


	float impulse = 3.5f;
	float MAX_HEIGHT;

	// Position of the player in the map
	fPoint position;

	// The speed in which we move the player (pixels per frame)
	int speed = 2;

	float AirSpeed = 1.3f;

	bool inAir = false;
	// The player spritesheet loaded into an SDL_Texture
	SDL_Texture* texture = nullptr;
	
	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	Animation* currentAnimation = nullptr;

	// The player's collider
	Collider* Pcollider = nullptr;
	Collider* Crouchcollider = nullptr;
	Collider* attackCollider = nullptr;
	Collider* kickCollider = nullptr;
	Collider* Deathcollider = nullptr;

	Collider* WolfKickCollider = nullptr;

	// A flag to detect when the player has been destroyed
	bool destroyed = false;

	int landing = 30;
	bool landed = false;

	void OnCollision(Collider* c1, Collider* c2) override;

	void KnockBack(float x);

	void KnockBackL(float x);

	void PlayerHit(Collider* c2);


	void Gravity_();

	

	void PlayerBump();

	void playerDamaged();

	bool jumped = false;

	float destroyedCountdown = 300;
	

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
	int FadeCnt = 200;
	bool playerKnocked = false;

	bool Platform = false;
};


#endif //!__MODULE_PLAYER_H__