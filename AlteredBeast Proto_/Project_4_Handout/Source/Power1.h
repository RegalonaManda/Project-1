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
//------- Animation Speeds ----------- //
#define WALKANIMSPEED 0.08f
#define PUNCHANIMSPEED 0.13f
#define CROUCHANIMSPEED 0.05f
#define CROUCHPUNCHSPEED 0.1f
#define KICKANIMSPEED 0.13f
#define CROUCHKICKSPEED 0.1f
#define DEATHANIMSPEED 0.05f
#define HITANIMSPEED 0.05f

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


void ModulePlayer::Power1Movement() {

	if (idle == true && airSt != AirState::CROUCH) {
		crouched = false;
	}

	attack = 2;
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
		if (idle == true && dir == Direction::RIGHT && airSt == AirState::AIRBORN && knock == false) {
			currentAnimation = &AllAnimations.P1JumpR;
		}
		if (idle == true && dir == Direction::LEFT && airSt == AirState::AIRBORN && knock == false) {
			currentAnimation = &AllAnimations.P1JumpL;
		}
		if (dir == Direction::RIGHT && airSt == AirState::AIRBORN && knock == true) {
			currentAnimation = &AllAnimations.knockBackRight;
		}
		if (dir == Direction::LEFT && airSt == AirState::AIRBORN && knock == true) {
			currentAnimation = &AllAnimations.knockBackRight;
		}
		if (idle == true && airSt != AirState::CROUCH && App->input->keys[SDL_SCANCODE_D] == KEY_REPEAT || App->input->pads[0].l_x > 0.5f) 
		{
			if (!destroyed ) {

				if (idle && airSt == AirState::GROUND)/* Can't move if punching */ {
					//change direction
					dir = Direction::RIGHT;
					currentAnimation = &AllAnimations.P1walkRight;
					position.x += speed;
				}
				//Air
				if (airSt == AirState::AIRBORN && CollideState == knock::NOT) {
					position.x += AirSpeed;
				}
			}
		}

		if (crouched == true) { airSt = AirState::CROUCH; }

		if (idle == true && airSt != AirState::CROUCH && App->input->keys[SDL_SCANCODE_A] == KEY_REPEAT || App->input->pads[0].l_x < -0.5f) {
			if (!destroyed) {
				if (idle && airSt == AirState::GROUND)/* Can't move if punching */ {
					//change direction
					dir = Direction::LEFT;

					currentAnimation = &AllAnimations.P1walkLeft;
					position.x -= speed;
				}
				//Air
				if (airSt == AirState::AIRBORN && CollideState == knock::NOT) {
					position.x -= AirSpeed;
				}
			}
		}

		if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT || App->input->pads[0].l_y > 0.5f) {

			if (idle && airSt == AirState::GROUND) {

				airSt = AirState::CROUCH;
				crouched = true;

				if (dir == Direction::LEFT) {
					currentAnimation = &AllAnimations.P1CrouchLeft;
					airSt = AirState::CROUCH;
				}
				if (dir == Direction::RIGHT) {
					currentAnimation = &AllAnimations.P1CrouchRight;
					airSt = AirState::CROUCH;
				}
			}

		}

		if (App->input->keys[SDL_SCANCODE_Z] == KEY_DOWN || App->input->pads[0].b) {

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


		if (App->input->keys[SDL_SCANCODE_X] == KEY_DOWN || App->input->pads[0].a) {

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
				attackCollider->SetPos(position.x - 1, position.y - 30);
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
			if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT || App->input->pads[0].l_y > 0.5f) {
				airSt = AirState::CROUCH;
			}
			else { airSt = AirState::GROUND; }

		}
		if (AllAnimations.P1CrouchPunchL.HasFinished() == true) {
			AllAnimations.P1CrouchPunchL.loopCount--;
			idle = true;
			if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT || App->input->pads[0].l_y > 0.5f) {
				airSt = AirState::CROUCH;
			}
			else { airSt = AirState::GROUND; }

		}
		if (AllAnimations.P1CrouchKickR.HasFinished() == true) {
			AllAnimations.P1CrouchKickR.loopCount--;
			idle = true;
			if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT || App->input->pads[0].l_y > 0.5f) {
				airSt = AirState::CROUCH;
			}
			else { airSt = AirState::GROUND; }
		}
		if (AllAnimations.P1CrouchKickL.HasFinished() == true) {
			AllAnimations.P1CrouchKickL.loopCount--;
			idle = true;
			if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT || App->input->pads[0].l_y > 0.5f) {
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

					App->fade->FadeToBlack((Module*)App->scene, (Module*)App->sceneIntro, 60);
					idle = true;
					airSt = AirState::GROUND;
					tranSt = Transform::DEFAULT;
					this->CleanUp();
				}
			}
		}
	}
}







