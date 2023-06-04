#ifndef __MODULE_BOSS_H__
#define __MODULE_BOSS_H__

#include <time.h>
#include<stdlib.h>
#include "Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"
#include "ModuleBoss.h"
#include "ModuleScene.h"


struct SDL_Texture;
struct Collider;



class Head {

public:
	

	float positionX;
	float positionY;

	float acceleration = 0.1f;
	float deacceleration = 1.5f;

	Collider* headCollider;
	Collider* hurtCollider;

	SDL_Texture* text = nullptr;

	Animation* current;

	bool fallen = false;

	

	Animation travelAnim;
	Animation turnAnim;
	Animation fallAnim;
	Animation XplodeAnim;
	Animation notActive;

	void Start() {
		
		
		headCollider = App->collisions->AddCollider({ 0,0,44,40 }, Collider::Type::ENEMY_SHOT, (Module*)App->bossfight);
		hurtCollider = App->collisions->AddCollider({ 1,1,42,39 }, Collider::Type::ENEMY, (Module*)App->bossfight);

		
	};

	void Anims() {
		 
		travelAnim.PushBack({ 1139, 1422, 48, 48 });
		travelAnim.PushBack({ 1090, 1422, 48, 48 });
		travelAnim.loop = false;
		travelAnim.speed = 0.08f;

		turnAnim.PushBack({ 1041,1422,48,48 });
		turnAnim.loop = true;

		fallAnim.PushBack({ 992,1422,48,48 });
		fallAnim.loop = true;

		XplodeAnim.PushBack({ 1186,1512,61,45 });
		XplodeAnim.PushBack({ 1124,1512,61,45 });
		XplodeAnim.PushBack({ 1062,1512,61,45 });
		XplodeAnim.PushBack({ 1000,1512,61,45 });
		XplodeAnim.PushBack({ 938,1512,61,45 });
		XplodeAnim.PushBack({ 876,1512,61,45 });
		XplodeAnim.PushBack({ 0,0,1,1 });
		XplodeAnim.loop = false;
		XplodeAnim.speed = 0.2f;

		notActive.PushBack({ 0,0,1,1 });
	}

	int FinalX;

	void Trajectory() {
		if (positionX >= FinalX) { 
			current = &travelAnim;
			if (deacceleration >= 1.0f) {
				deacceleration -= 0.02f;
			}
			
			positionX -= 3 * deacceleration;
			positionY -= 0.7;
			if (positionX == FinalX) {
				current = &turnAnim;
			}
		}
		else if (!fallen) {
			acceleration += 0.05f;
			positionY += 2 * acceleration;
			current = &fallAnim;
			headCollider->SetPos(positionX+2, positionY);
			hurtCollider->SetPos(positionX+3, positionY);
		}
		

		if (!fallen) {
			
		}
		
		
	}

	void Draw() {

		SDL_Rect Head = current->GetCurrentFrame();
		App->render->Blit(text, positionX, positionY, &Head);
	}

};


class AttackPattern {

public:

	Head headAttack[6];
	int activeHeads = 0;

	int ID;

};

class ModuleBoss : public Module
{
public:
	// Constructor
	ModuleBoss(bool startEnabled);

	// Destructor
	~ModuleBoss();


	bool Start() override;

	bool Initialize();

	update_status Update() override;


	update_status PostUpdate() override;


	void OnCollision(Collider* c1, Collider* c2) override;

	bool Attack(AttackPattern& currentPattern);

	bool CleanUp() override;

	bool attackFinished;
	int auxCnt = 40;
	bool initilized;
	int playOnce = 0;


public:

	fPoint position;

	int cooldown = 20;

	bool Neff_activ = false;

	int speed;

	int hp = 100;
	int destroyedCountdown = 20;
	
	SDL_Texture* texture = nullptr;

	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction

	SDL_Rect initialframe = { 1185,744,62,144 };

	Animation* currentAnim;
	Animation* currentRedAnim;

	Animation notActive;
	Animation transform;
	Animation cloud;
	Animation idleAnim;
	Animation red_idleAnim;
	Animation attackAnim;
	Animation red_attackAnim;
	
	Animation deathAnim;

	fPoint spawnPos;

	Collider* colliderBoss = nullptr;

	// A flag to detect when the player has been destroyed
	bool beaten = false;
	bool transformed = false;
	int RandID;
	bool selected = false;

	AttackPattern* currentPattern;

	AttackPattern pattern[4];

	int attackCnt = 40;
	int currentHead = 0;
	int currentPattern_ = 0;

	bool Red = false;
	int Redcnt = 0;

	// Sound effects indices
	uint welcomeDoom = 0;
	uint headThrow = 0;
	uint deathExpl = 0;

};





#endif 
