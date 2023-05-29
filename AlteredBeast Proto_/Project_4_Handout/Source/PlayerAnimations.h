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
	Animation powerUp2;

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

	Animation RiseR;
	Animation RiseL;

	//Power1 animations
	Animation P1IdleRight;
	Animation P1IdleLeft;

	Animation P1walkRight;
	Animation P1walkLeft;
	Animation P1punchRight;
	Animation P1punchLeft;
	Animation P1KickLeft;
	Animation P1KickRight;

	Animation P1CrouchRight;
	Animation P1CrouchLeft;

	Animation P1CrouchPunchR;
	Animation P1CrouchPunchL;

	Animation P1CrouchKickR;
	Animation P1CrouchKickL;

	Animation P1JumpR;
	Animation P1JumpL;

	Animation P1JumpPunchR;
	Animation P1JumpPunchL;

	Animation P1JumpKickR;
	Animation P1JumpKickL;

	Animation P1LandingR;
	Animation P1LandingL;

	//Power2 animations
	Animation P2IdleRight;
	Animation P2IdleLeft;

	Animation P2walkRight;
	Animation P2walkLeft;
	Animation P2punchRight;
	Animation P2punchLeft;
	Animation P2KickLeft;
	Animation P2KickRight;

	Animation P2CrouchRight;
	Animation P2CrouchLeft;
				
	Animation P2CrouchPunchR;
	Animation P2CrouchPunchL;

	Animation P2CrouchKickR;
	Animation P2CrouchKickL;

	Animation P2JumpR;
	Animation P2JumpL;

	Animation P2JumpPunchR;
	Animation P2JumpPunchL;

	Animation P2JumpKickR;
	Animation P2JumpKickL;

	Animation P2LandingR;
	Animation P2LandingL;

	//------WereWolf Animations--------

	Animation W_IdleR;
	Animation W_IdleL;

	Animation W_walkR;
	Animation W_walkL;

	Animation W_KickR;
	Animation W_KickL;

	Animation W_punchR;
	Animation W_punchL;

	Animation W_punchR_depleted;
	Animation W_punchL_depleted;

	Animation W_crouchR;
	Animation W_crouchL;

	Animation W_crouch_punchR;
	Animation W_crouch_punchL;

	Animation W_jumpR;
	Animation W_jumpL;

	Animation W_jump_punchR;
	Animation W_jump_punchL;

	Animation W_knockedR;
	Animation W_knockedL;

	PlayerAnim::PlayerAnim() {


		idleAnimRight.PushBack({ 1, 1, 89, 65 });
		idleAnimRight.speed = 0.1f;

		idleAnimLeft.PushBack({ 631,1, 89,65 });
		idleAnimLeft.speed = 0.1f;

		// walk forward animation (arcade sprite sheet)
		forwardAnim.PushBack({ 91, 1, 89,65 });
		forwardAnim.PushBack({ 181, 1, 89,65 });
		forwardAnim.PushBack({ 271, 1, 89,65 });

		forwardAnim.loop = true;

		forwardAnim.speed = WALKANIMSPEED;


		backAnim.PushBack({ 361,1,89,65 });
		backAnim.PushBack({ 451,1,89,65 });
		backAnim.PushBack({ 541,1,89,65 });
		backAnim.speed = WALKANIMSPEED;

		//Punch animation
		punchAnimRight.PushBack({ 91, 67, 89, 65 });
		punchAnimRight.PushBack({ 181, 67, 89, 65 });
		punchAnimRight.PushBack({ 271, 67, 89, 65 });
		punchAnimRight.loop = false;
		punchAnimRight.totalFrames = 3;

		punchAnimRight.speed = PUNCHANIMSPEED;


		punchAnimLeft.PushBack({ 541,67,89,65 });
		punchAnimLeft.PushBack({ 451,67,89,65 });
		punchAnimLeft.PushBack({ 361,67,89,65 });
		punchAnimLeft.loop = false;

		punchAnimLeft.speed = PUNCHANIMSPEED;
		punchAnimLeft.totalFrames = 3;


		crouchAnimRight.PushBack({ 91, 133, 89, 65 });
		crouchAnimRight.speed = CROUCHANIMSPEED;
		crouchAnimRight.totalFrames = 1;
		crouchAnimRight.loop = false;

		crouchAnimLeft.PushBack({ 541, 133,89,65 });
		crouchAnimLeft.speed = CROUCHANIMSPEED;
		crouchAnimLeft.totalFrames = 1;
		crouchAnimLeft.loop = false;


		crouchPunchRight.PushBack({ 181, 133, 89,65 });
		crouchPunchRight.PushBack({ 271, 133, 89,65 });
		crouchPunchRight.loop = false;
		crouchPunchRight.totalFrames = 2;
		crouchPunchRight.speed = CROUCHPUNCHSPEED;

		crouchPunchLeft.PushBack({ 451, 133, 89,65 });
		crouchPunchLeft.PushBack({ 361, 133, 89,65 });
		crouchPunchLeft.loop = false;
		crouchPunchLeft.totalFrames = 2;
		crouchPunchLeft.speed = CROUCHPUNCHSPEED;


		kickAnimRight.PushBack({ 91, 265, 89,65 });
		kickAnimRight.PushBack({ 181, 265, 89,65 });
		kickAnimRight.PushBack({ 271, 265, 89,65 });
		kickAnimRight.PushBack({ 181, 265, 89,65 });

		kickAnimRight.speed = KICKANIMSPEED;
		kickAnimRight.loop = false;
		kickAnimRight.totalFrames = 4;


		kickAnimLeft.PushBack({ 541, 265, 89,65 });
		kickAnimLeft.PushBack({ 451, 265,89,65 });
		kickAnimLeft.PushBack({ 361, 265, 89,65 });
		kickAnimLeft.PushBack({ 451, 265, 89,65 });

		kickAnimLeft.speed = KICKANIMSPEED;
		kickAnimLeft.loop = false;
		kickAnimLeft.totalFrames = 4;

		kickCrouchRight.PushBack({ 91, 331 ,89,65 });
		kickCrouchRight.PushBack({ 181,331,89,65 });
		kickCrouchRight.PushBack({ 91,331,89,65 });
		kickCrouchRight.speed = CROUCHKICKSPEED;
		kickCrouchRight.totalFrames = 3;
		kickCrouchRight.loop = false;

		kickCrouchLeft.PushBack({ 361,331,89,65 });
		kickCrouchLeft.PushBack({ 451,331,89,65 });
		kickCrouchLeft.PushBack({ 361,331,89,65 });
		kickCrouchLeft.speed = CROUCHKICKSPEED;
		kickCrouchLeft.totalFrames = 3;
		kickCrouchLeft.loop = false;


		deathAnimRight.PushBack({ 91,397,89,65 });
		deathAnimRight.PushBack({ 181,397,89,65 });
		deathAnimRight.PushBack({ 271,397,89,65 });
		deathAnimRight.speed = DEATHANIMSPEED;
		deathAnimRight.totalFrames = 3;
		deathAnimRight.loop = false;

		deathAnimLeft.PushBack({ 91, 397, 89, 65 });
		deathAnimLeft.PushBack({ 181, 397, 89, 65 });
		deathAnimLeft.PushBack({ 271, 397, 89, 65 });
		deathAnimLeft.speed = DEATHANIMSPEED;
		deathAnimLeft.totalFrames = 3;
		deathAnimLeft.loop = false;

		hitAnimRight.PushBack({ 1,463,89,65 });
		hitAnimRight.PushBack({ 91,463,89,65 });
		hitAnimRight.totalFrames = 2;
		hitAnimRight.loop = false;
		hitAnimRight.speed = HITANIMSPEED;

		hitAnimLeft.PushBack({ 361,463,89,65 });
		hitAnimLeft.PushBack({ 541,463,89,65 });
		hitAnimLeft.totalFrames = 2;
		hitAnimLeft.loop = false;
		hitAnimLeft.speed = HITANIMSPEED;

		knockBackRight.PushBack({ 91, 397, 89,65 });
		knockBackRight.totalFrames = 1;
		knockBackRight.loop = false;
		knockBackRight.speed = 0.07f;

		knockBackLeft.PushBack({ 541,397,89,65 });
		knockBackLeft.totalFrames = 1;
		knockBackLeft.loop = false;
		knockBackLeft.speed = 0.07f;

		HitContinueRight.PushBack({  91, 397, 89, 65 });
		HitContinueRight.PushBack({ 181, 397, 89, 65 });
		HitContinueRight.PushBack({ 271, 397, 89, 65 });
		HitContinueRight.PushBack({ 181, 463, 89, 65 });
		HitContinueRight.PushBack({ 271, 463, 89, 65 });
		HitContinueRight.totalFrames = 5;
		HitContinueRight.loop = false;
		HitContinueRight.speed = 0.07f;

		HitContinueLeft.PushBack({ 541, 397, 89, 65 });
		HitContinueLeft.PushBack({ 451, 397, 89, 65 });
		HitContinueLeft.PushBack({ 361, 397, 89, 65 });
		HitContinueLeft.PushBack({ 451, 463, 89, 65 });
		HitContinueLeft.PushBack({ 361, 463, 89, 65 });
		HitContinueLeft.totalFrames = 5;
		HitContinueLeft.loop = false;
		HitContinueLeft.speed = 0.07f;

		jumpRight.PushBack({  91, 199, 89, 65 });
		jumpRight.PushBack({ 181, 199, 89, 65 });
		jumpRight.speed = JUMPANIMSPEED;

		jumpLeft.PushBack({ 541, 199, 89, 65 });
		jumpLeft.PushBack({ 451, 199, 89, 65 });
		jumpLeft.speed = JUMPANIMSPEED;

		airPunchRight.PushBack({ 91, 529, 89, 65 });
		airPunchRight.loop = true;
		airPunchLeft.PushBack({ 541, 529, 89, 65 });
		airPunchLeft.loop = true;


		airKickRight.PushBack({ 271, 529, 89, 65 });
		airKickRight.PushBack({ 181, 529, 89, 65 });
		airKickRight.speed = AIRKICKSPEED;

		airKickLeft.PushBack({ 361, 529, 89, 65 });
		airKickLeft.PushBack({ 451, 529, 89, 65 });
		airKickLeft.speed = AIRKICKSPEED;
		airKickLeft.totalFrames = 2;

		LandingRight.PushBack({ 271, 199, 89, 65 });
		LandingLeft.PushBack({  361, 199, 89, 65 });

		RiseR.PushBack({ 181, 463, 89, 65});
		RiseR.PushBack({ 271, 463, 89, 65});
		RiseR.loop = false;
		RiseR.speed = 0.07f;
		RiseR.totalFrames = 2;

		RiseL.PushBack({ 451, 463, 89, 65 });
		RiseL.PushBack({ 361, 463, 89, 65 });
		RiseL.loop = false;
		RiseL.speed = 0.07f;
		RiseL.totalFrames = 2;


		//Time for power1 animations ;)----------------------------------------------------------------------------------------------------
		//orb taken
		powerUp1.PushBack({ 542, 1104, 54, 75 });
		powerUp1.PushBack({ 542, 1104, 54, 75 });
		powerUp1.PushBack({ 597, 1104, 54, 75 });
		powerUp1.PushBack({ 652, 1104, 54, 75 });
		powerUp1.PushBack({ 652, 1104, 54, 75 });
		powerUp1.loop = false;
		powerUp1.totalFrames = 5;
		powerUp1.speed = 0.07;

		P1IdleRight.PushBack({ 402,1253,88,69 });
		
		P1IdleLeft.PushBack({ 491,1253,88,69 });

		P1walkRight.PushBack({ 135,1253,88,69 });
		P1walkRight.PushBack({ 224,1253,88,69 });
		P1walkRight.PushBack({ 313,1253,88,69 });
		P1walkRight.speed = WALKANIMSPEED;

		P1walkLeft.PushBack({ 313,1329,88,69 });
		P1walkLeft.PushBack({ 224,1329,88,69 });
		P1walkLeft.PushBack({ 135,1329,88,69 });
		P1walkLeft.speed = WALKANIMSPEED;

		P1punchRight.PushBack({ 135,1473,88,69 });
		P1punchRight.PushBack({ 224,1473,88,69 });
		P1punchRight.PushBack({ 313,1473,88,69 });
		P1punchRight.totalFrames = 3;
		P1punchRight.speed = PUNCHANIMSPEED;
		P1punchRight.loop = false;

		P1punchLeft.PushBack({ 313,1543,88,69 });
		P1punchLeft.PushBack({ 224,1543,88,69 });
		P1punchLeft.PushBack({ 135,1543,88,69 });
		P1punchLeft.totalFrames = 3;
		P1punchLeft.speed = PUNCHANIMSPEED;
		P1punchLeft.loop = false;

		P1KickRight.PushBack({ 135,1403,88,69 });
		P1KickRight.PushBack({ 224,1403,88,69 });
		P1KickRight.PushBack({ 313,1403,88,69 });
		P1KickRight.totalFrames = 3;
		P1KickRight.speed = KICKANIMSPEED;
		P1KickRight.loop = false;

		P1KickLeft.PushBack({ 322,1613,88,69 });
		P1KickLeft.PushBack({ 233,1613,88,69 });
		P1KickLeft.PushBack({ 144,1613,88,69 });
		P1KickLeft.totalFrames = 3;
		P1KickLeft.speed = KICKANIMSPEED;
		P1KickLeft.loop = false;

		P1CrouchRight.PushBack({ 144,1683,88,69 });

		P1CrouchLeft.PushBack({ 313,1753, 88,69 });

		P1CrouchPunchR.PushBack({ 233,1683,88,69 });
		P1CrouchPunchR.PushBack({ 322,1683,88,69 });
		P1CrouchPunchR.totalFrames = 2;
		P1CrouchPunchR.speed = CROUCHPUNCHSPEED;
		P1CrouchPunchR.loop = false;

		P1CrouchPunchL.PushBack({ 233,1753,88,69 });
		P1CrouchPunchL.PushBack({ 144,1753,88,69 });
		P1CrouchPunchL.totalFrames = 2;
		P1CrouchPunchL.speed = CROUCHPUNCHSPEED;
		P1CrouchPunchL.loop = false;

		P1CrouchKickR.PushBack({ 411, 1683,88,69 });
		P1CrouchKickR.PushBack({ 500, 1683, 88 , 69 });
		P1CrouchKickR.totalFrames = 2;
		P1CrouchKickR.speed = CROUCHKICKSPEED;
		P1CrouchKickR.loop = false;

		P1CrouchKickL.PushBack({ 500,1753 ,88,69 });
		P1CrouchKickL.PushBack({ 411,1753 , 88 , 69 });
		P1CrouchKickL.totalFrames = 2;
		P1CrouchKickL.speed = CROUCHKICKSPEED;
		P1CrouchKickL.loop = false;

		P1JumpR.PushBack({ 144, 1823, 88, 69 });
		P1JumpR.PushBack({ 233, 1823, 88, 69 });
		P1JumpR.speed = JUMPANIMSPEED;

		P1JumpL.PushBack({ 322, 1893, 88, 69 });
		P1JumpL.PushBack({ 233, 1893, 88, 69 });
		P1JumpL.speed = JUMPANIMSPEED;

		P1JumpPunchR.PushBack({ 144, 1963, 91, 70});
		P1JumpPunchR.loop = true;

		P1JumpPunchL.PushBack({ 317,2033, 91, 70 });
		P1JumpPunchL.loop = true;

		
		P1JumpKickR.PushBack({ 322,1963, 91,69 });
		
		
		P1JumpKickL.PushBack({ 140,2033,94,69 });

		P1LandingR.PushBack({ 322, 1823, 88, 69 });

		P1LandingL.PushBack({ 144,1893,88,69 });

		//power2---------------------------------------------------------------------------------------------------------------

		powerUp2.PushBack({ 813, 2558, 54, 75 });
		powerUp2.PushBack({ 813, 2558, 54, 75 });
		powerUp2.PushBack({ 868, 2558, 54, 75 });
		powerUp2.PushBack({ 923, 2558, 54, 75 });
		powerUp2.PushBack({ 923, 2558, 54, 75 });
		powerUp2.loop = false;
		powerUp2.totalFrames = 5;
		powerUp2.speed = 0.07;

		P2IdleRight.PushBack({ 1,2123,99,72 });

		P2IdleLeft.PushBack({ 401,2123,99,72 });

		P2walkRight.PushBack({ 101, 2123, 99, 72});
		P2walkRight.PushBack({ 201, 2123, 99, 72});
		P2walkRight.PushBack({ 301, 2123, 99, 72});
		P2walkRight.speed = WALKANIMSPEED;

		P2walkLeft.PushBack({ 501, 2123, 99, 72 });
		P2walkLeft.PushBack({ 601, 2123, 99, 72 });
		P2walkLeft.PushBack({ 701, 2123, 99, 72 });
		P2walkLeft.speed = WALKANIMSPEED;

		P2punchRight.PushBack({ 101, 2195, 99, 72 });
		P2punchRight.PushBack({ 201, 2195, 99, 72 });
		P2punchRight.PushBack({ 301, 2195, 99, 72 });
		P2punchRight.totalFrames = 3;
		P2punchRight.speed = PUNCHANIMSPEED;
		P2punchRight.loop = false;

		P2punchLeft.PushBack({ 501, 2195, 99, 72 });
		P2punchLeft.PushBack({ 601, 2195, 99, 72 });
		P2punchLeft.PushBack({ 701, 2195, 99, 72 });
		P2punchLeft.totalFrames = 3;
		P2punchLeft.speed = PUNCHANIMSPEED;
		P2punchLeft.loop = false;

		P2KickRight.PushBack({ 101, 2269, 99, 72 });
		P2KickRight.PushBack({ 201, 2269, 99, 72 });
		P2KickRight.PushBack({ 301, 2269, 99, 72 });
		P2KickRight.totalFrames = 3;
		P2KickRight.speed = KICKANIMSPEED;
		P2KickRight.loop = false;

		P2KickLeft.PushBack({ 501, 2269, 99, 72 });
		P2KickLeft.PushBack({ 601, 2269, 99, 72 });
		P2KickLeft.PushBack({ 701, 2269, 99, 72 });
		P2KickLeft.totalFrames = 3;
		P2KickLeft.speed = KICKANIMSPEED;
		P2KickLeft.loop = false;

		P2CrouchRight.PushBack({ 001, 2342, 99, 72 });

		P2CrouchLeft.PushBack({ 401, 2342, 99, 72 });

		P2CrouchPunchR.PushBack({ 101, 2342, 99, 72 });
		P2CrouchPunchR.PushBack({ 201, 2342, 99, 72 });
		P2CrouchPunchR.PushBack({ 301, 2342, 99, 72 });
		P2CrouchPunchR.totalFrames = 3;
		P2CrouchPunchR.speed = CROUCHPUNCHSPEED;
		P2CrouchPunchR.loop = false;

		P2CrouchPunchL.PushBack({ 501, 2342, 99, 72 });
		P2CrouchPunchL.PushBack({ 601, 2342, 99, 72 });
		P2CrouchPunchL.PushBack({ 701, 2342, 99, 72 });
		P2CrouchPunchL.totalFrames = 3;
		P2CrouchPunchL.speed = CROUCHPUNCHSPEED;
		P2CrouchPunchL.loop = false;

		P2CrouchKickR.PushBack({ 201, 2415, 99, 72 });
		P2CrouchKickR.PushBack({ 301, 2415, 99, 72 });
		P2CrouchKickR.totalFrames = 2;
		P2CrouchKickR.speed = CROUCHKICKSPEED;
		P2CrouchKickR.loop = false;

		P2CrouchKickL.PushBack({ 601, 2415, 99, 72 });
		P2CrouchKickL.PushBack({ 701, 2415, 99, 72 });
		P2CrouchKickL.totalFrames = 2;
		P2CrouchKickL.speed = CROUCHKICKSPEED;
		P2CrouchKickL.loop = false;

		P2JumpR.PushBack({ 001, 2269, 99, 72 });
		P2JumpR.PushBack({ 101, 2415, 99, 72 });
		P2JumpR.speed = JUMPANIMSPEED;

		P2JumpL.PushBack({ 401, 2269, 99, 72});
		P2JumpL.PushBack({ 501, 2415, 99, 72});
		P2JumpL.speed = JUMPANIMSPEED;

		P2JumpPunchR.PushBack({ 301, 2488, 99, 72 });
		P2JumpPunchR.loop = true;

		P2JumpPunchL.PushBack({ 701, 2488, 99, 72});
		P2JumpPunchL.loop = true;


		P2JumpKickR.PushBack({ 201, 2488, 99, 72});


		P2JumpKickL.PushBack({ 601, 2488, 99, 72 });

		P2LandingR.PushBack({ 001, 2415, 99, 72});//NEED CHANGE

		P2LandingL.PushBack({ 401, 2415, 99, 72});//NEED CHANGE

		//----WereWolf--PushBack------------------------------------------------------------------------------------------------------------------

		W_IdleR.PushBack({  64, 659,148,92 });

		W_IdleL.PushBack({ 512, 1190 - 438, 148, 92 });

		W_walkR.PushBack({ 213, 659, 148, 92 });
		W_walkR.PushBack({ 362, 659, 148, 92 });
		W_walkR.PushBack({ 511, 659, 148, 92 });
		W_walkR.speed = WALKANIMSPEED;
		W_walkR.loop = true;

		W_walkL.PushBack({ 362, 752, 148, 92 });
		W_walkL.PushBack({ 213, 752, 148, 92 });
		W_walkL.PushBack({  64, 752, 148, 92 });
		W_walkL.speed = WALKANIMSPEED;
		W_walkL.loop = true;

		W_KickR.PushBack({  64, 1031, 148, 92 });
		W_KickR.PushBack({ 213, 1031, 148, 92 });

		W_KickR.PushBack({  64, 1031, 148, 92 });
		W_KickR.PushBack({ 213, 1031, 148, 92 });
		W_KickR.PushBack({  64, 1031, 148, 92 });
		W_KickR.PushBack({ 213, 1031, 148, 92 });
		W_KickR.PushBack({  64, 1031, 148, 92 });
		W_KickR.PushBack({ 213, 1031, 148, 92 });
		W_KickR.loop = false;
		W_KickR.speed = KICKANIMSPEED + 0.1;
		// loop 4 times

		W_KickL.PushBack({ 362, 1124, 148, 92 });
		W_KickL.PushBack({ 213, 1124, 148, 92 });

		W_KickL.PushBack({ 362, 1124, 148, 92 });
		W_KickL.PushBack({ 213, 1124, 148, 92 });
		W_KickL.PushBack({ 362, 1124, 148, 92 });
		W_KickL.PushBack({ 213, 1124, 148, 92 });
		W_KickL.PushBack({ 362, 1124, 148, 92 });
		W_KickL.PushBack({ 213, 1124, 148, 92 });
		W_KickL.loop = false;
		W_KickL.speed = KICKANIMSPEED + 0.1;

		W_punchR.PushBack({  64, 845, 148, 92 });
		W_punchR.PushBack({ 213, 845, 148, 92 });
		W_punchR.PushBack({ 362, 845, 148, 92 });
		W_punchR.loop = false;
		W_punchR.speed = PUNCHANIMSPEED;



		W_punchL.PushBack({ 362,938, 148, 92 });
		W_punchL.PushBack({ 213,938, 148, 92 });
		W_punchL.PushBack({ 64 ,938, 148, 92 });

		W_punchL.loop = false;
		W_punchL.speed = PUNCHANIMSPEED;


		W_punchR_depleted.PushBack({ 511,845, 148,92 });
		W_punchR_depleted.speed = PUNCHANIMSPEED/2;
		W_punchR_depleted.loop = false;

		W_punchL_depleted.PushBack({ 64,938, 148, 92 });
		W_punchL_depleted.speed = PUNCHANIMSPEED/2;
		W_punchL_depleted.loop = false;


		W_crouchR.PushBack({ 660,845,148,92 });

		W_crouchL.PushBack({ 958,938,148,92 });

		
		W_jumpR.PushBack({ 660,659,148,92 });
		W_jumpR.PushBack({ 809,659,148,92 });
		W_jumpR.speed = JUMPANIMSPEED;

		W_jumpL.PushBack({ 958,752,148,92 });
		W_jumpL.PushBack({ 809,752,148,92 });
		W_jumpL.speed = JUMPANIMSPEED;

		W_crouch_punchR.PushBack({ 809,845,148,92 });
		W_crouch_punchR.PushBack({ 958,845,148,92 });
		W_crouch_punchR.speed = CROUCHPUNCHSPEED;
		W_crouch_punchR.loop = false;


		W_crouch_punchL.PushBack({ 809,938,148,92 });
		W_crouch_punchL.PushBack({ 660,938,148,92 });
		W_crouch_punchL.speed = CROUCHPUNCHSPEED;
		W_crouch_punchL.loop = false;

		W_jump_punchR.PushBack({ 959,660,148,92 });

		W_jump_punchL.PushBack({ 660,752,148,92 });

		W_knockedR.PushBack({ 809,1031,148,92 });

		W_knockedL.PushBack({ 511,1124,148,92 });
	}
};

