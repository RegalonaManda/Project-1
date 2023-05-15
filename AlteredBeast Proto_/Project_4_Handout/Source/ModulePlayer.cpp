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
#include "ModulePower.h"
#include "ModuleFadeToBlack.h"
#include <stdio.h>
#include "SceneIntro.h"


#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib")
#pragma comment( lib, "SDL/libx86/SDL2main.lib")

#include "SDL/include/SDL_scancode.h"
//the lower the higher
//#define MAX_HEIGHT 147
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





Uint32 startTime = 0;
bool idle = true;
int knockPos;


ModulePlayer::ModulePlayer(bool startEnabled) : Module(startEnabled)
{
	
	startEnabled = true;
	
	//Default direction
	dir = Direction::RIGHT;
	//Default airstate
	airSt = AirState::GROUND;
	//Default transformation
	tranSt = Transform::DEFAULT;
	//default attack 
	attack = 1;


	
	
}


ModulePlayer::~ModulePlayer()
{

}

bool ModulePlayer::Start()
{
	LOG("Loading player textures");

	bool ret = true;

	destroyed = false;
	
	texture = App->textures->Load("Assets/ABfullspritesProto.png"); // arcade version

	//Initialize collider
	Pcollider = App->collisions->AddCollider({ 100,300,20,64 }, Collider::Type::PLAYER, this);
	Crouchcollider = App->collisions->AddCollider({ 100,300,20,34 }, Collider::Type::PLAYER, this);
	//CHANGE listener of attack to enemy
	attackCollider = App->collisions->AddCollider({ 100,300,33,19 }, Collider::Type::PLAYER_SHOT, this);
	kickCollider = App->collisions->AddCollider({ 100,300,19,33 }, Collider::Type::PLAYER_SHOT, this);
	Deathcollider = App->collisions->AddCollider({ 100,300,19,33 }, Collider::Type::ENEMY_SHOT, this);
	Deathcollider->SetPos(1300, 1200);
	//font table
	char lookupTable[] = { "abcdefghijklmnopqrstuvwxyz0123456789.,ç'?!*$%&()+-/<=>© " };
	scoreFont = App->fonts->Load("Assets/font_spritesheet.png", lookupTable, 1);

	//player Sound FX
	currentAnimation = &AllAnimations.idleAnimRight;
	nonLethalAtt = App->audio->LoadFx("Assets/FX/Non-Lethal_Punch.wav");
	lethalAtt = App->audio->LoadFx("Assets/FX/Lethal_Punch.wav");
	loseHP = App->audio->LoadFx("Assets/FX/lose_1_hp.wav");
	powerUp = App->audio->LoadFx("Assets/FX/Power_Up.wav");
	playerDeathFX = App->audio->LoadFx("Assets/FX/Player_Death.wav");

	lives = 3;
	hp = 4;
	idle = true;
	dir = Direction::RIGHT;
	start = false;
	attack = 1;
	tranSt = Transform::DEFAULT;
	
	

	return ret;
}

update_status ModulePlayer::Update()
{

	

	// calculate the max h for the current ground lvl
	if (airSt == AirState::GROUND) {
		MAX_HEIGHT = position.y - 52   ;
	}
 	

	if (App->sceneIntro->IsEnabled() == false && start == false) {
		position.x = 50;
		position.y = 190;
		Deathcollider->SetPos(position.x, position.y);
		start = true;
	}

	//Transforming Animations, middleground between two Power stages
	if (transforming == true && tranSt == Transform::DEFAULT && !destroyed) {
		
		currentAnimation = &AllAnimations.powerUp1;
		
	}
	if (AllAnimations.powerUp1.HasFinished() == true) {
		tranSt = Transform::POWER1;
		
		
		attack += 1;
		idle = true;
		AllAnimations.powerUp1.loopCount = 0;
		transforming = false;
		App->powers->Disable();
	}
	
	/*if (airSt == AirState::GROUND && Pcollider->Intersects(Pcollider->rect) == false) {
		airSt = AirState::AIRBORN;
	}*/

	
	ModulePlayer::Gravity_();
	
	// New jumping function

	if (App->input->keys[SDL_SCANCODE_SPACE] == KEY_DOWN && idle == true && airSt != AirState::AIRBORN && airSt != AirState::LANDING) {
		position.y -= 5;
		airSt = AirState::AIRBORN;
		jumped = true;
	}
	if (airSt == AirState::AIRBORN && jumped == true && transforming == false) {
		position.y -= impulse;
	}
	if (airSt == AirState::AIRBORN && jumped == false) {
		position.y += 3;
	}
	////Put a max height that makes the player fall faster for it to dont look like its floating ( not real but like the game )
	////impulse lesser, the faster it falls
	if (airSt == AirState::AIRBORN && position.y < MAX_HEIGHT) {
		impulse = -2.0;
		position.y = MAX_HEIGHT + 2;
	}


	if (tranSt == Transform::DEFAULT && transforming == false) {

		attack = 1;
		//if (playerKnocked == true) {
		//	if (dir == Direction::RIGHT) {
		//		Pcollider->SetPos(position.x + 20, position.y - 60);
		//		if (position.y >= 190) {
		//			currentAnimation = &AllAnimations.RiseR;
		//			
		//		}
		//		
		//		//KnockBack(0.8);
		//		if (position.y >= 190) {
		//			if (AllAnimations.RiseR.HasFinished() == true) {
		//				AllAnimations.RiseR.Reset();
		//				AllAnimations.RiseR.loopCount = 0;
		//				airSt = AirState::GROUND;
		//				position.y = 190;
		//				idle = true;
		//				playerKnocked = false;

		//			}
		//		}
		//		
		//	}
		//	if (dir == Direction::LEFT) {
		//		Pcollider->SetPos(position.x + 20, position.y - 60);
		//		if (position.y >= 190) {
		//			currentAnimation = &AllAnimations.RiseL;
		//			
		//		}

		//		if (AllAnimations.RiseL.HasFinished() == true) {
		//			AllAnimations.RiseL.Reset();
		//			AllAnimations.RiseL.loopCount = 0;
		//			airSt = AirState::GROUND;
		//			position.y = 190;
		//			idle = true;
		//			playerKnocked = false;

		//		}
		//	}
		//}
		if (destroyed == false) {
			if (idle == true && airSt == AirState::GROUND && iFrames == false) {
				//position.y = 190;
				knockImpulse = 0;
			}

			if (AllAnimations.knockBackLeft.HasFinished() == true) {
				idle = true;
				airSt == AirState::GROUND;
				AllAnimations.knockBackLeft.loopCount--;
			}
			if (AllAnimations.knockBackRight.HasFinished() == true) {
				idle = true;
				airSt == AirState::GROUND;
				AllAnimations.knockBackRight.loopCount--;
			}



			//Update Collider to current player pos, change it depending on direction and AirState
			if (dir == Direction::RIGHT && airSt == AirState::GROUND) { Pcollider->SetPos(position.x + 18, position.y - 65); }
			if (dir == Direction::LEFT && airSt == AirState::GROUND) { Pcollider->SetPos(position.x + 20, position.y - 65); }

			if (dir == Direction::RIGHT && airSt == AirState::CROUCH)
			{
				
				Crouchcollider->SetPos(position.x + 17, position.y - 40);
				Pcollider->SetPos(0, -5000);
			}
			if (dir == Direction::LEFT && airSt == AirState::CROUCH)
			{
				
				Crouchcollider->SetPos(position.x + 17, position.y - 40);
				Pcollider->SetPos(0, -5000);
			}
			if (airSt != AirState::CROUCH) {
				Crouchcollider->SetPos(0, -5000);
			}

			if (dir == Direction::RIGHT && airSt == AirState::AIRBORN) { Pcollider->SetPos(position.x + 20, position.y - 65); }
			if (dir == Direction::LEFT && airSt == AirState::AIRBORN) { Pcollider->SetPos(position.x + 20, position.y - 65); }

			if (idle == true) {
				attackCollider->SetPos(1000, 1000);
			}
			
			

			// Landing isn't used yet... i'm dreading having to implement it 

			if (airSt == AirState::LANDING) {
				if (dir == Direction::LEFT) { currentAnimation = &AllAnimations.LandingLeft; }
				if (dir == Direction::RIGHT) { currentAnimation = &AllAnimations.LandingRight; }
				landing--;
			}
			// 5 frame counter for landing animation
			if (landing <= 0) {
				airSt = AirState::GROUND;
				landing = 5;
			}




			//Reset the currentAnimation back to idle, either left/right, ground/crouch before updating the logic
			//Could be a switch
			if (idle == true && dir == Direction::RIGHT && airSt == AirState::GROUND)
			{
				currentAnimation = &AllAnimations.idleAnimRight;
			}
			if (idle == true && dir == Direction::LEFT && airSt == AirState::GROUND)
			{
				currentAnimation = &AllAnimations.idleAnimLeft;
			}
			if (idle == true && dir == Direction::RIGHT && airSt == AirState::CROUCH) {
				currentAnimation = &AllAnimations.crouchAnimRight;
			}
			if (idle == true && dir == Direction::LEFT && airSt == AirState::CROUCH) {
				currentAnimation = &AllAnimations.crouchAnimLeft;
			}
			if (idle == true && dir == Direction::RIGHT && airSt == AirState::AIRBORN) {
				currentAnimation = &AllAnimations.jumpRight;
			}
			if (idle == true && dir == Direction::LEFT && airSt == AirState::AIRBORN) {
				currentAnimation = &AllAnimations.jumpLeft;
			}
			if (App->input->keys[SDL_SCANCODE_D] == KEY_REPEAT && !destroyed && knockImpulse == 0)
			{
				if (idle == true && airSt == AirState::GROUND)/* Can't move if punching */ {
					//change direction
					dir = Direction::RIGHT;
					currentAnimation = &AllAnimations.forwardAnim;
					position.x += speed;
				}
				//Air
				if (airSt == AirState::AIRBORN) {
					position.x += AirSpeed;
				}
			}


			if (App->input->keys[SDL_SCANCODE_A] == KEY_REPEAT && !destroyed && knockImpulse == 0) {
				if (idle == true && airSt == AirState::GROUND)/* Can't move if punching */ {
					//change direction
					dir = Direction::LEFT;

					currentAnimation = &AllAnimations.backAnim;
					position.x -= speed;
				}
				//Air
				if (airSt == AirState::AIRBORN) {
					position.x -= AirSpeed;
				}
			}

			if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT && airSt == AirState::GROUND && idle == true) {

				airSt = AirState::CROUCH;

				if (dir == Direction::LEFT) {
					currentAnimation = &AllAnimations.crouchAnimLeft;
				}
				if (dir == Direction::RIGHT) {
					currentAnimation = &AllAnimations.crouchAnimRight;
				}

			}

			if (App->input->keys[SDL_SCANCODE_Z] == KEY_DOWN) {

				if (hitEnemy == false) {
					App->audio->PlayFx(nonLethalAtt, 3);
				}

				if (idle == true && dir == Direction::RIGHT && airSt == AirState::GROUND) {
					AllAnimations.punchAnimRight.Reset();
					currentAnimation = &AllAnimations.punchAnimRight;



					//activate punch collider when player punches
					attackCollider->SetPos(position.x + 38, position.y - 60);

					idle = false;
				}
				if (idle == true && dir == Direction::LEFT && airSt == AirState::GROUND)
				{
					AllAnimations.punchAnimLeft.Reset();
					currentAnimation = &AllAnimations.punchAnimLeft;
					//activate punch collider when player punches
					attackCollider->SetPos(position.x + 0, position.y - 60);

					idle = false;
				}
				if (idle == true && dir == Direction::RIGHT && airSt == AirState::CROUCH) {
					AllAnimations.crouchPunchRight.Reset();
					currentAnimation = &AllAnimations.crouchPunchRight;
					attackCollider->SetPos(position.x + 43, position.y - 40);
					idle = false;
				}
				if (idle == true && dir == Direction::LEFT && airSt == AirState::CROUCH) {
					AllAnimations.crouchPunchLeft.Reset();
					currentAnimation = &AllAnimations.crouchPunchLeft;
					//CHANGE x
					attackCollider->SetPos(position.x + 0, position.y - 40);
					idle = false;
				}
				if (airSt == AirState::AIRBORN) {
					if (dir == Direction::LEFT) {
						currentAnimation = &AllAnimations.airPunchLeft;

						idle = false;
					}
					if (dir == Direction::RIGHT) {
						currentAnimation = &AllAnimations.airPunchRight;

						idle = false;
					}
				}
			}
			if (currentAnimation == &AllAnimations.airPunchLeft) { attackCollider->SetPos(position.x + 0, position.y - 60); }
			if (currentAnimation == &AllAnimations.airPunchRight) { attackCollider->SetPos(position.x + 26, position.y - 60); }
			if (currentAnimation == &AllAnimations.airKickLeft) { attackCollider->SetPos(position.x + 0, position.y - 40); }
			if (currentAnimation == &AllAnimations.airKickRight) { attackCollider->SetPos(position.x + 33, position.y - 40); }

			if (App->input->keys[SDL_SCANCODE_X] == KEY_DOWN) {

				if (hitEnemy == false) {
					App->audio->PlayFx(nonLethalAtt, 3);
				}

				if (idle == true && dir == Direction::RIGHT && airSt == AirState::GROUND) {
					AllAnimations.kickAnimRight.Reset();
					currentAnimation = &AllAnimations.kickAnimRight;
					attackCollider->SetPos(position.x + 43, position.y - 30);
					idle = false;
				}
				if (idle == true && dir == Direction::LEFT && airSt == AirState::GROUND)
				{
					AllAnimations.kickAnimLeft.Reset();
					currentAnimation = &AllAnimations.kickAnimLeft;
					attackCollider->SetPos(position.x - 5, position.y - 30);
					idle = false;
				}
				if (idle == true && dir == Direction::RIGHT && airSt == AirState::CROUCH) {
					AllAnimations.kickCrouchRight.Reset();
					currentAnimation = &AllAnimations.kickCrouchRight;
					kickCollider->SetPos(position.x + 37, position.y - 60);
					idle = false;
				}
				if (idle == true && dir == Direction::LEFT && airSt == AirState::CROUCH) {
					AllAnimations.kickCrouchLeft.Reset();
					currentAnimation = &AllAnimations.kickCrouchLeft;
					kickCollider->SetPos(position.x + 20, position.y - 60);
					idle = false;
				}
				if (airSt == AirState::AIRBORN) {
					if (dir == Direction::LEFT) {
						currentAnimation = &AllAnimations.airKickLeft;

						idle = false;
					}
					if (dir == Direction::RIGHT) {
						currentAnimation = &AllAnimations.airKickRight;

						idle = false;
					}
				}
			}

			//OUTSIDE THE IF
			if (AllAnimations.punchAnimRight.HasFinished() == true) {
				AllAnimations.punchAnimRight.loopCount--;
				idle = true;
				//deactivate punch collider
				attackCollider->SetPos(1000, 1000);
			}
			//OUTSIDE THE IF
			if (AllAnimations.punchAnimLeft.HasFinished() == true) {
				AllAnimations.punchAnimLeft.loopCount--;
				idle = true;
				//deactivate punch collider
				attackCollider->SetPos(1000, 1000);
			}

			if (AllAnimations.crouchPunchRight.HasFinished() == true) {
				AllAnimations.crouchPunchRight.loopCount--;
				idle = true;
				if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT) {
					airSt = AirState::CROUCH;
				}
				else { airSt = AirState::GROUND; }
				//deactivate punch collider
				attackCollider->SetPos(1000, 1000); //quick fix to make collider disappear from scene by sending it oob, TRY TO CHANG

			}
			if (AllAnimations.crouchPunchLeft.HasFinished() == true) {
				AllAnimations.crouchPunchLeft.loopCount--;
				idle = true;
				if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT) {
					airSt = AirState::CROUCH;
				}
				else { airSt = AirState::GROUND; }
				//deactivate punch collider
				attackCollider->SetPos(1000, 1000); //quick fix to make collider disappear from scene by sending it oob, TRY TO CHANGE

			}
			if (AllAnimations.kickCrouchRight.HasFinished() == true) {
				AllAnimations.kickCrouchRight.loopCount--;
				idle = true;
				if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT) {
					airSt = AirState::CROUCH;
				}
				else { airSt = AirState::GROUND; }
				//deactivate punch collider
				kickCollider->SetPos(1000, 1000); //quick fix to make collider disappear from scene by sending it oob, TRY TO CHANG
			}
			if (AllAnimations.kickCrouchLeft.HasFinished() == true) {
				AllAnimations.kickCrouchLeft.loopCount--;
				idle = true;
				if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT) {
					airSt = AirState::CROUCH;
				}
				else { airSt = AirState::GROUND; }
				//deactivate punch collider
				kickCollider->SetPos(1000, 1000); //quick fix to make collider disappear from scene by sending it oob, TRY TO CHANG
			}

			if (AllAnimations.kickAnimRight.HasFinished() == true) {
				AllAnimations.kickAnimRight.loopCount--;   //VERY IMPORTANT , since HasFinished checks if the loop count has surpassed 0, after the animation has finished reset loop count
				idle = true;
				//deactivate kick collider
				attackCollider->SetPos(1000, 1000);
			}
			//OUTSIDE THE IF
			if (AllAnimations.kickAnimLeft.HasFinished() == true) {
				AllAnimations.kickAnimLeft.loopCount--;   //VERY IMPORTANT , since HasFinished checks if the loop count has surpassed 0, after the animation has finished reset loop count
				idle = true;
				//deactivate kick collider
				attackCollider->SetPos(1000, 1000);
			}

			if (AllAnimations.kickCrouchLeft.HasFinished() == true) {
				AllAnimations.kickCrouchLeft.loopCount--;
				idle = true;
				kickCollider->SetPos(1000, 1000);
			}

			if (AllAnimations.kickCrouchRight.HasFinished() == true) {
				AllAnimations.kickCrouchRight.loopCount--;
				idle = true;
				kickCollider->SetPos(1000, 1000);
			}



			if (App->input->keys[SDL_SCANCODE_S] == KEY_UP && idle == true) {


				airSt = AirState::GROUND;
				//deactivate punch collider
				kickCollider->SetPos(1000, 1000); //quick fix to make collider disappear from scene by sending it oob, TRY TO CHANGE
			}



			//Player gets killed
			if (destroyed) {


				if (dir == Direction::RIGHT) {
					if (destroyedCountdown > 117)
					{
						position.x -= 0.7f;
					}
					currentAnimation = &AllAnimations.deathAnimRight;

				}
				else if (dir == Direction::LEFT) {
					if (destroyedCountdown > 117) {
						position.x += 0.7f;
					}
					currentAnimation = &AllAnimations.deathAnimLeft;
				}

				impulse -= Gravity;
				position.y -= impulse;

				if (position.y >= 190) {
					position.x += 0;
					position.y = 190;
					destroyedCountdown -= 0.5f;
					if (destroyedCountdown <= 0) {
						//return update_status::UPDATE_STOP;
						
						App->fade->FadeToBlack((Module*)App->scene, (Module*)App->sceneIntro, 60.0f);
						this->CleanUp();
					}

				}


			}
		}
	}
	//Power1
	if (tranSt == Transform::POWER1) {

		if (idle == true && airSt == AirState::GROUND && iFrames == false) {
			
			knockImpulse = 0;
		}

		if (AllAnimations.knockBackLeft.HasFinished() == true) {
			idle = true;
			airSt == AirState::GROUND;
			AllAnimations.knockBackLeft.loopCount--;
		}
		if (AllAnimations.knockBackRight.HasFinished() == true) {
			idle = true;
			airSt == AirState::GROUND;
			AllAnimations.knockBackRight.loopCount--;
		}

		//Update Collider to current player pos, change it depending on direction and AirState
		if (dir == Direction::RIGHT && airSt == AirState::GROUND) { Pcollider->SetPos(position.x + 36, position.y - 68); }
		if (dir == Direction::LEFT && airSt == AirState::GROUND) { Pcollider->SetPos(position.x + 34, position.y - 68); }

		if (dir == Direction::RIGHT && airSt == AirState::CROUCH)
		{
			Pcollider->SetPos(0, position.y - 5000);
			Crouchcollider->SetPos(position.x + 36, position.y - 40);
		}
		if (dir == Direction::LEFT && airSt == AirState::CROUCH)
		{
			Pcollider->SetPos(0, position.y - 5000);
			Crouchcollider->SetPos(position.x + 40, position.y - 40);
		}
		if (airSt != AirState::CROUCH) { Crouchcollider->SetPos(0, -5000); }

		if (dir == Direction::RIGHT && airSt == AirState::AIRBORN) { Pcollider->SetPos(position.x + 40, position.y - 60); }
		if (dir == Direction::LEFT && airSt == AirState::AIRBORN) { Pcollider->SetPos(position.x + 30, position.y - 60); }

		if (idle == true) {
			attackCollider->SetPos(1000, 1000);
			kickCollider->SetPos(1000, 1000);
		}
		//Jump function
		













		if (airSt == AirState::LANDING) {
			if (dir == Direction::LEFT) { currentAnimation = &AllAnimations.P1LandingL; }
			if (dir == Direction::RIGHT) { currentAnimation = &AllAnimations.P1LandingR; }
			landing--;
		}
		if (landing <= 0) {
			airSt = AirState::GROUND;
			landing = 5;
		}
		//Reset the currentAnimation back to idle, either left/right, ground/crouch before updating the logic
		if (idle == true && dir == Direction::RIGHT && airSt == AirState::GROUND)
		{
			currentAnimation = &AllAnimations.P1IdleRight;
		}
		if (idle == true && dir == Direction::LEFT && airSt == AirState::GROUND)
		{
			currentAnimation = &AllAnimations.P1IdleLeft;
		}
		if (idle == true && dir == Direction::RIGHT && airSt == AirState::CROUCH) {
			currentAnimation = &AllAnimations.P1CrouchRight;
		}
		if (idle == true && dir == Direction::LEFT && airSt == AirState::CROUCH) {
			currentAnimation = &AllAnimations.P1CrouchLeft;
		}
		if (idle == true && dir == Direction::RIGHT && airSt == AirState::AIRBORN) {
			currentAnimation = &AllAnimations.P1JumpR;
		}
		if (idle == true && dir == Direction::LEFT && airSt == AirState::AIRBORN) {
			currentAnimation = &AllAnimations.P1JumpL;
		}
		if (App->input->keys[SDL_SCANCODE_D] == KEY_REPEAT && !destroyed && knockImpulse == 0)
		{
			if (idle == true && airSt == AirState::GROUND)/* Can't move if punching */ {
				//change direction
				dir = Direction::RIGHT;
				currentAnimation = &AllAnimations.P1walkRight;
				position.x += speed;
			}
			//Air
			if (airSt == AirState::AIRBORN) {
				position.x += AirSpeed;
			}
		}


		if (App->input->keys[SDL_SCANCODE_A] == KEY_REPEAT && !destroyed && knockImpulse == 0) {
			if (idle == true && airSt == AirState::GROUND)/* Can't move if punching */ {
				//change direction
				dir = Direction::LEFT;

				currentAnimation = &AllAnimations.P1walkLeft;
				position.x -= speed;
			}
			//Air
			if (airSt == AirState::AIRBORN) {
				position.x -= AirSpeed;
			}
		}

		if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT && airSt == AirState::GROUND && idle == true) {

			airSt = AirState::CROUCH;

			if (dir == Direction::LEFT) {
				currentAnimation = &AllAnimations.P1CrouchLeft;
			}
			if (dir == Direction::RIGHT) {
				currentAnimation = &AllAnimations.P1CrouchRight;
			}

		}

		if (App->input->keys[SDL_SCANCODE_Z] == KEY_DOWN) {

			if (hitEnemy == false) {
				App->audio->PlayFx(nonLethalAtt, 3);
			}

			if (idle == true && dir == Direction::RIGHT && airSt == AirState::GROUND) {
				AllAnimations.P1punchRight.Reset();
				currentAnimation = &AllAnimations.P1punchRight;



				//activate punch collider when player punches
				attackCollider->SetPos(position.x + 46, position.y - 60);

				idle = false;
			}
			if (idle == true && dir == Direction::LEFT && airSt == AirState::GROUND)
			{
				AllAnimations.P1punchLeft.Reset();
				currentAnimation = &AllAnimations.P1punchLeft;
				//activate punch collider when player punches
				attackCollider->SetPos(position.x + 8, position.y - 60);

				idle = false;
			}
			if (idle == true && dir == Direction::RIGHT && airSt == AirState::CROUCH) {
				AllAnimations.P1CrouchPunchR.Reset();
				currentAnimation = &AllAnimations.P1CrouchPunchR;
				attackCollider->SetPos(position.x + 51, position.y - 40);
				idle = false;
			}
			if (idle == true && dir == Direction::LEFT && airSt == AirState::CROUCH) {
				AllAnimations.P1CrouchPunchL.Reset();
				currentAnimation = &AllAnimations.P1CrouchPunchL;
				//CHANGE x
				attackCollider->SetPos(position.x + 4, position.y - 40);
				idle = false;
			}
			if (airSt == AirState::AIRBORN) {
				if (dir == Direction::LEFT) {
					currentAnimation = &AllAnimations.P1JumpPunchL;

					idle = false;
				}
				if (dir == Direction::RIGHT) {
					currentAnimation = &AllAnimations.P1JumpPunchR;

					idle = false;
				}
			}
		}
		//CHANGE
		if (currentAnimation == &AllAnimations.P1JumpPunchL) { attackCollider->SetPos(position.x + 0, position.y - 60); }
		if (currentAnimation == &AllAnimations.P1JumpPunchR) { attackCollider->SetPos(position.x + 59, position.y - 60); }
		if (currentAnimation == &AllAnimations.P1JumpKickL) { attackCollider->SetPos(position.x + 0, position.y - 40); }
		if (currentAnimation == &AllAnimations.P1JumpKickR) { attackCollider->SetPos(position.x + 55, position.y - 40); }


		if (App->input->keys[SDL_SCANCODE_X] == KEY_DOWN) {

			if (hitEnemy == false) {
				App->audio->PlayFx(nonLethalAtt, 3);
			}

			if (idle == true && dir == Direction::RIGHT && airSt == AirState::GROUND) {
				AllAnimations.P1KickRight.Reset();
				currentAnimation = &AllAnimations.P1KickRight;
				attackCollider->SetPos(position.x + 55, position.y - 30);
				idle = false;
			}
			if (idle == true && dir == Direction::LEFT && airSt == AirState::GROUND)
			{
				AllAnimations.P1KickLeft.Reset();
				currentAnimation = &AllAnimations.P1KickLeft;
				attackCollider->SetPos(position.x -1, position.y - 30);
				idle = false;
			}
			if (idle == true && dir == Direction::RIGHT && airSt == AirState::CROUCH) {
				AllAnimations.P1CrouchKickR.Reset();
				currentAnimation = &AllAnimations.P1CrouchKickR;
				kickCollider->SetPos(position.x + 51, position.y - 60);
				idle = false;
			}
			if (idle == true && dir == Direction::LEFT && airSt == AirState::CROUCH) {
				AllAnimations.P1CrouchKickL.Reset();
				currentAnimation = &AllAnimations.P1CrouchKickL;
				kickCollider->SetPos(position.x + 16, position.y - 60);
				idle = false;
			}
			if (airSt == AirState::AIRBORN) {
				if (dir == Direction::LEFT) {
					currentAnimation = &AllAnimations.P1JumpKickL;

					idle = false;
				}
				if (dir == Direction::RIGHT) {
					currentAnimation = &AllAnimations.P1JumpKickR;

					idle = false;
				}
			}
		}

		//OUTSIDE THE IF
		if (AllAnimations.P1punchRight.HasFinished() == true) {
			AllAnimations.P1punchRight.loopCount--;
			idle = true;
		}
		//OUTSIDE THE IF
		if (AllAnimations.P1punchLeft.HasFinished() == true) {
			AllAnimations.P1punchLeft.loopCount--;
			idle = true;
		}

		if (AllAnimations.P1CrouchPunchR.HasFinished() == true) {
			AllAnimations.P1CrouchPunchR.loopCount--;
			idle = true;
			if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT) {
				airSt = AirState::CROUCH;
			}
			else { airSt = AirState::GROUND; }

		}
		if (AllAnimations.P1CrouchPunchL.HasFinished() == true) {
			AllAnimations.P1CrouchPunchL.loopCount--;
			idle = true;
			if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT) {
				airSt = AirState::CROUCH;
			}
			else { airSt = AirState::GROUND; }

		}
		if (AllAnimations.P1CrouchKickR.HasFinished() == true) {
			AllAnimations.P1CrouchKickR.loopCount--;
			idle = true;
			if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT) {
				airSt = AirState::CROUCH;
			}
			else { airSt = AirState::GROUND; }
		}
		if (AllAnimations.P1CrouchKickL.HasFinished() == true) {
			AllAnimations.P1CrouchKickL.loopCount--;
			idle = true;
			if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT) {
				airSt = AirState::CROUCH;
			}
			else { airSt = AirState::GROUND; }
		}

		if (AllAnimations.P1KickRight.HasFinished() == true) {
			AllAnimations.P1KickRight.loopCount--;   //VERY IMPORTANT , since HasFinished checks if the loop count has surpassed 0, after the animation has finished reset loop count
			idle = true;
		}
		//OUTSIDE THE IF
		if (AllAnimations.P1KickLeft.HasFinished() == true) {
			AllAnimations.P1KickLeft.loopCount--;   //VERY IMPORTANT , since HasFinished checks if the loop count has surpassed 0, after the animation has finished reset loop count
			idle = true;
		}

		if (AllAnimations.P1CrouchKickL.HasFinished() == true) {
			AllAnimations.P1CrouchKickL.loopCount--;
			idle = true;
		}

		if (AllAnimations.P1CrouchKickR.HasFinished() == true) {
			AllAnimations.P1CrouchKickR.loopCount--;
			idle = true;
		}



		if (App->input->keys[SDL_SCANCODE_S] == KEY_UP && idle == true) {
			airSt = AirState::GROUND;
		}

		//Player gets killed
		if (destroyed) {

			if (dir == Direction::RIGHT) {
				if (destroyedCountdown > 297)
				{
					position.x -= 0.7f;
				}
				currentAnimation = &AllAnimations.deathAnimRight;

			}
			else if (dir == Direction::LEFT) {
				if (destroyedCountdown > 297) {
					position.x += 0.7f;
				}
				currentAnimation = &AllAnimations.deathAnimLeft;
			}

			impulse -= Gravity;
			position.y -= impulse;

			if (position.y >= 190) {
				position.x += 0;
				position.y = 190;
				
				destroyedCountdown -= 0.5f;
				if (destroyedCountdown <= 0) {
					//return update_status::UPDATE_STOP;
					
					App->fade->FadeToBlack((Module*)App->scene, (Module*)App->sceneIntro, 60);
					idle = true;
					airSt = AirState::GROUND;
					tranSt = Transform::DEFAULT;
					this->CleanUp();
				}

			}
			//if (position.y >= 190) {
			//	position.x += 0;
			//	position.y = 190;
			//	destroyedCountdown -= 0.5f;
			//	if (destroyedCountdown <= 0) {
			//		//return update_status::UPDATE_STOP;

			//		App->fade->FadeToBlack((Module*)App->scene, (Module*)App->sceneIntro, 60.0f);
			//		this->CleanUp();
			//	}

			//}

		}

	}

	
		currentAnimation->Update();

	
		if (iFrames == true)
		{
			
			airSt == AirState::AIRBORN;
			knockImpulse -= Gravity;
			position.y -= knockImpulse;

			position.x--;

			iTimer--;
			if (iTimer <= 0) {
				iFrames = false;
				iTimer = 30;
			}
			
		}

		if (App->input->keys[SDL_SCANCODE_F1] == KEY_DOWN && GodMode == false) {
			GodMode = true;

		}
		else if (App->input->keys[SDL_SCANCODE_F1] == KEY_DOWN && GodMode == true) {
			GodMode = false;
			lives = 3;
		}

		if (App->input->keys[SDL_SCANCODE_F3] == KEY_DOWN) {

			App->audio->PlayMusic("Assets/Music/Game_Over.ogg", 0.0f);
			
			if (tranSt == Transform::DEFAULT) {
				Deathcollider->SetPos(position.x+20, position.y-20);
			} 
			if (tranSt == Transform::POWER1) {
				Deathcollider->SetPos(position.x + 36, position.y - 65);
			}
			lives = 0;
			idle = true;
			airSt = AirState::GROUND;
			tranSt = Transform::DEFAULT;
		}
		else if (App->input->keys[SDL_SCANCODE_F4] == KEY_DOWN) {

			App->audio->PlayMusic("Assets/Music/Win.ogg", 0.0f);
			/*App->enemies->Disable();*/
			App->collisions->Disable();
		
			App->player->KilledBoss = true;
			App->scene2->killedBoss = true;
			
		}


		if (GodMode == true) {
			lives++;
		}

		if (KilledBoss == true) {

			

			if (FadeCnt == 200) { App->player->score += 20000; }
			FadeCnt--;
			
			if (FadeCnt <= 0) {
				KilledBoss = false;
				App->scene2->killedBoss = false;
				App->render->camera.x = 0;
				FadeCnt = 200;
				tranSt = Transform::DEFAULT;
				transforming = false;
				idle = true;
				airSt = AirState::GROUND;
				App->fade->FadeToBlack((Module*)App->scene, (Module*)App->sceneIntro, 60);
				/*this->CleanUp();*/
			}
		}
		if (startExTimer == true) {
			--exTimer;
		}
		return update_status::UPDATE_CONTINUE;
	
}

update_status ModulePlayer::PostUpdate()
{


	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	App->render->Blit(texture, position.x, position.y - rect.h, &rect);

	sprintf_s(scoreText, 10, "%7d", score);

	App->fonts->BlitText(60, 8, scoreFont, scoreText);
	App->fonts->BlitText(230, 8, scoreFont, "50000");



	return update_status::UPDATE_CONTINUE;
}




void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{
	if (GodMode == false) {

		//--------------------------------------Explosion Collision ------------------------------------
		if (c1 == Pcollider && c2->type == Collider::Type::ATTACK_XplosionTrigger && !destroyed && !iFrames)
		{
			startExTimer = true;

			if (exTimer <= 0) {

				PlayerHit(c2);
				startExTimer = false;
				exTimer = 40;
			}
			

			playerDamaged();
		}

		if (c1 == attackCollider && c2->type == Collider::Type::ENEMY) {
			hitEnemy = true;

			hitEnemy = false;
		}
		//-------------------------------------Bumping into enemy----------------------------
		if (c1 == Pcollider && c2->type == Collider::Type::ENEMY && !destroyed && iFrames == false)
		{

			PlayerBump();
			/*knockImpulse = 1.0f;
			up to change */


			/*App->scene->ScreenScroll = false;*/
		}

		//------------------------------------------------Getting hit by enemy attack-------------------------------------

		//if (c1 == Pcollider && c2->type == Collider::Type::ENEMY_SHOT && !destroyed && iFrames == false)
		//{
		//	PlayerHit(c2);

		//	/*App->scene->ScreenScroll = false;*/
		//}

		if (c1 == Pcollider && c2->type == Collider::Type::ENEMY_SHOT && !destroyed && iFrames == false)
		{
			Deathcollider->SetPos(1300, 1200);
			firstHit = true;

			knockImpulse = 1.0f;
			iFrames = true;
			hp--;


			position.y -= 0.1f;
			if (position.y < 190) {
				//shoudl call a different knockbackfunction
				if (c2 != Deathcollider) {
					//KnockBack(25);
					playerKnocked = true;
					if (hp > 0) { App->audio->PlayFx(loseHP, 6); }
					if (dir == Direction::LEFT) {
						idle = false;
						currentAnimation = &AllAnimations.knockBackLeft;
					}
					if (dir == Direction::RIGHT) {
						currentAnimation = &AllAnimations.knockBackRight;
						idle = false;
					}
				}
			}

			playerDamaged();
		}

		//Getting hit by enemy attack
		if (c1 == Pcollider && c2->type == Collider::Type::ENEMY_SHOT && !destroyed && iFrames == false)
		{
			PlayerHit(c2);

			playerDamaged();

			/*App->scene->ScreenScroll = false;*/
		}

	}
	
	// -------------------------------------------Camera Collisions-------------------------------

	if (c1->type == Collider::Type::PLAYER && c2 == App->scene->backCamLimit) {
		while (position.x < (App->render->camera.x * 0.3333333333f - 13.3333333333f)) {
			position.x = App->render->camera.x * 0.3333333333f - 13.3333333333f;
		}

	}

	if (c1->type == Collider::Type::PLAYER && c2 == App->scene->frontCamLimit) {

		while (position.x > App->scene->aux - 33.3333333333f) {
			position.x = App->scene->aux - 33.3333333333f;
		}

	}

	//-------------------------------------------PowerUp Collisions----------------------------------------
	
	if (c1 == Pcollider && c2->type == Collider::Type::POWER_UP && transforming == false) {

		transforming = true;
		App->audio->PlayFx(powerUp, -1);

		//El disable no funciona de momento, voy a esconderlo
		App->powers->gotten = true;
		App->powers->collider->SetPos(-3000, -3000);
		App->powers->Disable();


	}

	// -------------------------------------------Ground Collisions-------------------------------

	if (c1 == Pcollider && c2->type == Collider::Type::PLATFORM) {
		
		//Change to landing later
		position.y--;
		airSt = AirState::GROUND;
		idle = true;
		jumped = false;
	}
	else {
		//airSt = AirState::AIRBORN;
	}

	if (c1 == Pcollider && c2->type == Collider::Type::WALL) {
		position.x-=2;
	}
	if (c1 == Pcollider && c2->type == Collider::Type::WALL_RIGHT) {
		position.x += 2;
	}

	// -------------------------------------------Border Collisions-------------------------------

	if (c1 == Pcollider && c2->type == Collider::Type::BORDER && airSt != AirState::AIRBORN) {
		airSt = AirState::AIRBORN;
	}
}


void ModulePlayer:: KnockBack(float x) {
	
		//airSt == AirState::AIRBORN;
		//knockImpulse -= Gravity;
		//position.y -= knockImpulse;
		//
		//position.x -= x;
		//if (position.y >= 190) {
		//	airSt = AirState::GROUND;
		//	position.y = 190;
		//	idle = true;
		//	//jumpRight.Reset();
		///*	knockImpulse = 10;*/
		//}
	
}

void ModulePlayer::KnockBackL(float x) {

	//airSt == AirState::AIRBORN;
	//knockImpulse -= Gravity;
	//position.y -= knockImpulse;
	//
	//position.x += x;
	//if (position.y >= 190) {
	//	airSt = AirState::GROUND;
	//	position.y = 190;
	//	idle = true;
	//	//jumpRight.Reset();
	//	/*knockImpulse = 1;*/
	//}

}
void ModulePlayer::PlayerHit(Collider* c2) {

	Deathcollider->SetPos(1300, 1200);
	firstHit = true;
	/*if ((c2->rect.x + c2->rect.w) >= Pcollider->rect.x) {
		knockImpulse = -1.0f;
	} else if (c2->rect.x < (Pcollider->rect.x+ Pcollider->rect.w) ) {
		knockImpulse = 1.0f;

	}*/
	
	knockImpulse = 1.0f;
	iFrames = true;
	hp--;


	position.y -= 0.1f;
	if (position.y < 190) {
		//shoudl call a different knockbackfunction
		if (c2 != Deathcollider) {
			//KnockBack(25);
			playerKnocked = true;
			if (hp > 0) { App->audio->PlayFx(loseHP, 6); }
			if (dir == Direction::LEFT) {
				idle = false;
				currentAnimation = &AllAnimations.knockBackLeft;
			}
			if (dir == Direction::RIGHT) {
				currentAnimation = &AllAnimations.knockBackRight;
				idle = false;
			}
		}
	}

	playerDamaged();

}
void ModulePlayer::PlayerBump() {

	knockImpulse = 1.0f;
	iFrames = true;

	if (hp > 0) {
		App->audio->PlayFx(loseHP, 6);
	}

	position.y -= 0.1f;
	if (position.y < 190) {
		if (dir == Direction::RIGHT) { KnockBack(0.5); }
		if (dir == Direction::LEFT) { KnockBackL(0.5); }

		if (dir == Direction::LEFT) {
			idle = false;
			currentAnimation = &AllAnimations.knockBackLeft;
		}
		if (dir == Direction::RIGHT) {
			currentAnimation = &AllAnimations.knockBackRight;
			idle = false;
		}
	}

	playerDamaged();


}
void ModulePlayer::playerDamaged() {
	if (hp <= 0)
	{
		hp = 3;
		lives--;
	}

	if (lives <= 0)
	{

		
		hp = 0;
		//DEATH
		App->audio->PlayFx(playerDeathFX, 4);
		App->audio->PlayMusic("Assets/Music/Game_Over.ogg", 0.0f);
		destroyed = true;
		start = false;
		
		App->fade->FadeToBlack((Module*)App->scene, (Module*)App->sceneIntro, 60.0f);
	}
}

void ModulePlayer::Gravity_() {
	// Gravity_ Function
	if (airSt == AirState::AIRBORN) {
		impulse -= Gravity;
	}
	else {
		impulse = 3.5f;
	}
}