#include "ModulePlayer.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCollisions.h"
#include "ModuleFonts.h"
#include "ModuleAudio.h"
#include "ModuleScene.h"
#include "Colliders.h"
#include "ModuleScene2.h"
#include "ModuleEnemies.h"
#include "Enemy.h"

#include <stdio.h>

#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib")
#pragma comment( lib, "SDL/libx86/SDL2main.lib")

#include "SDL/include/SDL_scancode.h"
#include "Animation.h"
#include "Module.h"
#include <stdio.h>

#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib")
#pragma comment( lib, "SDL/libx86/SDL2main.lib")

#include "SDL/include/SDL_scancode.h"

//------- Animation Speeds ----------- //
#define WALKANIMSPEED 0.08f
#define PUNCHANIMSPEED 0.13f
#define CROUCHANIMSPEED 0.05f
#define CROUCHPUNCHSPEED 0.1f
#define KICKANIMSPEED 0.13f
#define CROUCHKICKSPEED 0.1f
#define DEATHANIMSPEED 0.05f
#define HITANIMSPEED 0.05f
#define JUMPANIMSPEED 0.025f
#define AIRKICKSPEED 0.06f



// idle animation (arcade sprite sheet)
	// x,y (top of rect in sprite sheet) , width, height of rect

class PlayerAnim {
public:
	
	



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

	Animation powerUp1;

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

	//Power1 animations
	Animation P1IdleRight;
	Animation P1IdleLeft;

	Animation P1walkRight;
	Animation P1walkLeft;
	Animation P1punchRight;
	Animation P1punchLeft;
	Animation P1KickLeft;
	Animation P1KickRight;

	PlayerAnim::PlayerAnim() {


		idleAnimRight.PushBack({ 0, 0, 54, 75 });
		idleAnimRight.speed = 0.1f;

		idleAnimLeft.PushBack({ 165,75, 54,75 });
		idleAnimLeft.speed = 0.1f;

		// walk forward animation (arcade sprite sheet)
		forwardAnim.PushBack({ 56, 0, 54, 75 });
		forwardAnim.PushBack({ 112, 0, 54, 75 });
		forwardAnim.PushBack({ 168, 0, 54, 75 });

		forwardAnim.loop = true;

		forwardAnim.speed = WALKANIMSPEED;


		backAnim.PushBack({ 0,76,54,75 });
		backAnim.PushBack({ 56,76,54,75 });
		backAnim.PushBack({ 111,76,54,75 });
		backAnim.speed = WALKANIMSPEED;

		//Punch animation
		punchAnimRight.PushBack({ 0,151,54,75 });
		punchAnimRight.PushBack({ 56,151,54,75 });
		punchAnimRight.PushBack({ 110,151,111,75 });//Extra large frame for extended punch
		punchAnimRight.loop = false;
		punchAnimRight.totalFrames = 3;

		punchAnimRight.speed = PUNCHANIMSPEED;


		punchAnimLeft.PushBack({ 385,227,54,75 });
		punchAnimLeft.PushBack({ 330,227,54,75 });
		punchAnimLeft.PushBack({ 261,227,73,75 });//Extra large frame for extended punch
		punchAnimLeft.loop = false;

		punchAnimLeft.speed = PUNCHANIMSPEED;
		punchAnimLeft.totalFrames = 3;


		crouchAnimRight.PushBack({ 0, 226, 54, 75 });
		crouchAnimRight.speed = CROUCHANIMSPEED;
		crouchAnimRight.totalFrames = 1;

		crouchAnimLeft.PushBack({ 386,303,54,75 });
		crouchAnimLeft.speed = CROUCHANIMSPEED;
		crouchAnimLeft.totalFrames = 1;


		crouchPunchRight.PushBack({ 55, 226, 54, 75 });
		crouchPunchRight.PushBack({ 110, 226, 108, 75 });
		crouchPunchRight.loop = false;
		crouchPunchRight.totalFrames = 2;
		crouchPunchRight.speed = CROUCHPUNCHSPEED;

		crouchPunchLeft.PushBack({ 331, 303, 54, 75 });
		crouchPunchLeft.PushBack({ 253, 303, 63, 75 });
		crouchPunchLeft.loop = false;
		crouchPunchLeft.totalFrames = 2;
		crouchPunchLeft.speed = CROUCHPUNCHSPEED;


		kickAnimRight.PushBack({ 221, 379, 87, 75 });
		kickAnimRight.PushBack({ 309, 379, 87, 75 });
		kickAnimRight.PushBack({ 397, 379, 87, 75 });
		kickAnimRight.PushBack({ 309, 379, 87, 75 });

		kickAnimRight.speed = KICKANIMSPEED;
		kickAnimRight.loop = false;
		kickAnimRight.totalFrames = 4;


		kickAnimLeft.PushBack({ 221, 455, 87, 75 });
		kickAnimLeft.PushBack({ 309, 455, 87, 75 });
		kickAnimLeft.PushBack({ 397, 455, 87, 75 });
		kickAnimLeft.PushBack({ 309, 455, 87, 75 });

		kickAnimLeft.speed = KICKANIMSPEED;
		kickAnimLeft.loop = false;
		kickAnimLeft.totalFrames = 4;

		kickCrouchRight.PushBack({ 1,455,72,75 });
		kickCrouchRight.PushBack({ 74,455,72,75 });
		kickCrouchRight.PushBack({ 1,455,72,75 });
		kickCrouchRight.speed = CROUCHKICKSPEED;
		kickCrouchRight.totalFrames = 3;
		kickCrouchRight.loop = false;

		kickCrouchLeft.PushBack({ 1,531,72,75 });
		kickCrouchLeft.PushBack({ 74,531,72,75 });
		kickCrouchLeft.PushBack({ 1,531,72,75 });
		kickCrouchLeft.speed = CROUCHKICKSPEED;
		kickCrouchLeft.totalFrames = 3;
		kickCrouchLeft.loop = false;


		deathAnimRight.PushBack({ 266,531,72,75 });
		deathAnimRight.PushBack({ 339,531,72,75 });
		deathAnimRight.PushBack({ 412,531,72,75 });
		deathAnimRight.speed = DEATHANIMSPEED;
		deathAnimRight.totalFrames = 3;
		deathAnimRight.loop = false;

		hitAnimRight.PushBack({ 1,607,68,70 });
		hitAnimRight.PushBack({ 70,607,68,70 });
		hitAnimRight.totalFrames = 2;
		hitAnimRight.loop = false;
		hitAnimRight.speed = HITANIMSPEED;

		hitAnimLeft.PushBack({ 1,678,68,70 });
		hitAnimLeft.PushBack({ 70,678,68,70 });
		hitAnimLeft.totalFrames = 2;
		hitAnimLeft.loop = false;
		hitAnimLeft.speed = HITANIMSPEED;

		knockBackRight.PushBack({ 266, 531, 72, 75 });
		knockBackRight.totalFrames = 1;
		knockBackRight.loop = false;
		knockBackRight.speed = 0.07f;

		knockBackLeft.PushBack({ 266,607,72,75 });
		knockBackLeft.totalFrames = 1;
		knockBackLeft.loop = false;
		knockBackLeft.speed = 0.07f;

		HitContinueRight.PushBack({ 266,531,72,75 });
		HitContinueRight.PushBack({ 339,531,72,75 });
		HitContinueRight.PushBack({ 412,531,72,75 });
		HitContinueRight.PushBack({ 139, 607, 72, 75 });
		HitContinueRight.PushBack({ 208, 607, 72, 75 });
		HitContinueRight.totalFrames = 5;
		HitContinueRight.loop = false;
		HitContinueRight.speed = 0.07f;

		HitContinueLeft.PushBack({ 266,531,72,75 });
		HitContinueLeft.PushBack({ 339,531,72,75 });
		HitContinueLeft.PushBack({ 412,531,72,75 });
		HitContinueLeft.PushBack({ 139, 678, 72, 75 });
		HitContinueLeft.PushBack({ 208, 678, 72, 75 });
		HitContinueLeft.totalFrames = 5;
		HitContinueLeft.loop = false;
		HitContinueLeft.speed = 0.07f;

		deathAnimLeft.PushBack({ 266,607,72,75 });
		deathAnimLeft.PushBack({ 339,607,72,75 });
		deathAnimLeft.PushBack({ 412,607,72,75 });
		deathAnimLeft.speed = DEATHANIMSPEED;
		deathAnimLeft.totalFrames = 3;
		deathAnimLeft.loop = false;

		jumpRight.PushBack({ 0,303,54,75 });
		jumpRight.PushBack({ 56,303,54,75 });
		jumpRight.speed = JUMPANIMSPEED;

		jumpLeft.PushBack({ 0,379,54,75 });
		jumpLeft.PushBack({ 56,379,54,75 });
		jumpLeft.speed = JUMPANIMSPEED;

		airPunchLeft.PushBack({ 354,749,54,75 });
		airPunchLeft.loop = true;
		airPunchRight.PushBack({ 0,749,60,75 });
		airPunchRight.loop = true;

		airKickRight.PushBack({ 70,749,68,75 });
		airKickRight.PushBack({ 140,749,68,75 });
		airKickRight.speed = AIRKICKSPEED;

		airKickLeft.PushBack({ 277,749,68,75 });
		airKickLeft.PushBack({ 208,749,68,75 });
		airKickLeft.speed = AIRKICKSPEED;
		airKickLeft.totalFrames = 2;

		LandingLeft.PushBack({ 111,379,54,75 });
		LandingRight.PushBack({ 111,303,54,75 });

		//Time for power1 animations ;)

		powerUp1.PushBack({ 221, 0, 54, 75 });
		powerUp1.PushBack({ 221, 0, 54, 75 });
		powerUp1.PushBack({ 276, 0, 54, 75 });
		powerUp1.PushBack({ 331, 0, 54, 75 });
		powerUp1.PushBack({ 331, 0, 54, 75 });
		powerUp1.loop = false;
		powerUp1.totalFrames = 5;
		powerUp1.speed = 0.07;

		P1IdleRight.PushBack({ 743,5,88,69 });
		
		P1IdleLeft.PushBack({ 832,5,88,69 });

		P1walkRight.PushBack({ 476,5,88,69 });
		P1walkRight.PushBack({ 565,5,88,69 });
		P1walkRight.PushBack({ 654,5,88,69 });
		P1walkRight.speed = WALKANIMSPEED;

		P1walkLeft.PushBack({ 654,81,88,69 });
		P1walkLeft.PushBack({ 565,81,88,69 });
		P1walkLeft.PushBack({ 476,81,88,69 });
		P1walkLeft.speed = WALKANIMSPEED;

		P1punchRight.PushBack({ 476,225,88,69 });
		P1punchRight.PushBack({ 565,225,88,69 });
		P1punchRight.PushBack({ 654,225,88,69 });
		P1punchRight.totalFrames = 3;
		P1punchRight.speed = PUNCHANIMSPEED;
		P1punchRight.loop = false;

		P1punchLeft.PushBack({ 654,295,88,69 });
		P1punchLeft.PushBack({ 565,295,88,69 });
		P1punchLeft.PushBack({ 476,295,88,69 });
		P1punchLeft.totalFrames = 3;
		P1punchLeft.speed = PUNCHANIMSPEED;
		P1punchLeft.loop = false;

		P1KickRight.PushBack({ 476,155,88,69 });
		P1KickRight.PushBack({ 565,155,88,69 });
		P1KickRight.PushBack({ 654,155,88,69 });
		P1KickRight.totalFrames = 3;
		P1KickRight.speed = KICKANIMSPEED;
		P1KickRight.loop = false;

		P1KickLeft.PushBack({ 663,365,88,69 });
		P1KickLeft.PushBack({ 574,365,88,69 });
		P1KickLeft.PushBack({ 485,365,88,69 });
		P1KickLeft.totalFrames = 3;
		P1KickLeft.speed = KICKANIMSPEED;
		P1KickLeft.loop = false;

	}



};

