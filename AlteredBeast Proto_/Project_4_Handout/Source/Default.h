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


void ModulePlayer::DefaultMovement() {

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

	}

	//Update Collider to current player pos, change it depending on direction and AirState
	if (dir == Direction::RIGHT && airSt == AirState::GROUND) { Pcollider->SetPos(position.x + 36, position.y - 65); }
	if (dir == Direction::LEFT && airSt == AirState::GROUND) { Pcollider->SetPos(position.x + 36, position.y - 65); }

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

	if (dir == Direction::RIGHT && airSt == AirState::AIRBORN) { Pcollider->SetPos(position.x + 36, position.y - 60); }
	if (dir == Direction::LEFT && airSt == AirState::AIRBORN) { Pcollider->SetPos(position.x + 36, position.y - 60); }

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
	if (dir == Direction::RIGHT && airSt == AirState::AIRBORN && knock == true) {
		currentAnimation = &AllAnimations.knockBackRight;
	}
	if (dir == Direction::LEFT && airSt == AirState::AIRBORN && knock == true) {
		currentAnimation = &AllAnimations.knockBackRight;
	}
	if (App->input->keys[SDL_SCANCODE_D] == KEY_REPEAT || App->input->pads[0].l_x > 0.5f)
	{
		if (!destroyed) {
			if (idle == true && airSt == AirState::GROUND)/* Can't move if punching */ {
				//change direction
				dir = Direction::RIGHT;
				currentAnimation = &AllAnimations.forwardAnim;
				position.x += speed;
			}
			//Air
			if (airSt == AirState::AIRBORN && CollideState == knock::NOT) {
				position.x += AirSpeed;
			}
		}


	}


	if (App->input->keys[SDL_SCANCODE_A] == KEY_REPEAT || App->input->pads[0].l_x < -0.5f) {

		if (!destroyed) {
			if (idle == true && airSt == AirState::GROUND)/* Can't move if punching */ {
				//change direction
				dir = Direction::LEFT;

				currentAnimation = &AllAnimations.backAnim;
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


				if (dir == Direction::LEFT) {
					currentAnimation = &AllAnimations.crouchAnimLeft;
				}
				if (dir == Direction::RIGHT) {
					currentAnimation = &AllAnimations.crouchAnimRight;
				}
			}
		}

		if (App->input->keys[SDL_SCANCODE_Z] == KEY_DOWN || App->input->pads[0].b) {

			if (hitEnemy == false) {
				App->audio->PlayFx(nonLethalAtt, 3);
			}

			if (idle == true && dir == Direction::RIGHT && airSt == AirState::GROUND) {
				AllAnimations.punchAnimRight.Reset();
				currentAnimation = &AllAnimations.punchAnimRight;



				//activate punch collider when player punches  
				attackCollider->SetPos(position.x + 56, position.y -60);

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
				attackCollider->SetPos(position.x + 60, position.y - 40);
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

		if (App->input->keys[SDL_SCANCODE_X] == KEY_DOWN || App->input->pads[0].a) {

			if (hitEnemy == false) {
				App->audio->PlayFx(nonLethalAtt, 3);
			}

			if (idle == true && dir == Direction::RIGHT && airSt == AirState::GROUND) {
				AllAnimations.kickAnimRight.Reset();
				currentAnimation = &AllAnimations.kickAnimRight;
				attackCollider->SetPos(position.x + 58, position.y - 30);
				idle = false;
			}
			if (idle == true && dir == Direction::LEFT && airSt == AirState::GROUND)
			{
				AllAnimations.kickAnimLeft.Reset();
				currentAnimation = &AllAnimations.kickAnimLeft;
				attackCollider->SetPos(position.x +5, position.y - 30);
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

	/*	if (AllAnimations.jumpLeft.HasFinished()) {
			idle = true;
			AllAnimations.jumpLeft.loopCount--;
			airSt = AirState::GROUND; 
		}

		if (AllAnimations.jumpLeft.HasFinished()) {
			idle = true;
			AllAnimations.jumpLeft.loopCount--;
			airSt = AirState::GROUND;
		}*/

		if (AllAnimations.crouchPunchRight.HasFinished() == true) {
			AllAnimations.crouchPunchRight.loopCount--;
			idle = true;
			if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT || App->input->pads[0].l_y > 0.5f) {
				airSt = AirState::CROUCH;
			}
			else { airSt = AirState::GROUND; }
			//deactivate punch collider
			attackCollider->SetPos(1000, 1000); //quick fix to make collider disappear from scene by sending it oob, TRY TO CHANG

		}
		if (AllAnimations.crouchPunchLeft.HasFinished() == true) {
			AllAnimations.crouchPunchLeft.loopCount--;
			idle = true;
			if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT || App->input->pads[0].l_y > 0.5f) {
				airSt = AirState::CROUCH;
			}
			else { airSt = AirState::GROUND; }
			//deactivate punch collider
			attackCollider->SetPos(1000, 1000); //quick fix to make collider disappear from scene by sending it oob, TRY TO CHANGE

		}
		if (AllAnimations.kickCrouchRight.HasFinished() == true) {
			AllAnimations.kickCrouchRight.loopCount--;
			idle = true;
			if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT || App->input->pads[0].l_y > 0.5f) {
				airSt = AirState::CROUCH;
			}
			else { airSt = AirState::GROUND; }
			//deactivate punch collider
			kickCollider->SetPos(1000, 1000); //quick fix to make collider disappear from scene by sending it oob, TRY TO CHANG
		}
		if (AllAnimations.kickCrouchLeft.HasFinished() == true) {
			AllAnimations.kickCrouchLeft.loopCount--;
			idle = true;
			if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT || App->input->pads[0].l_y > 0.5f) {
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
	








