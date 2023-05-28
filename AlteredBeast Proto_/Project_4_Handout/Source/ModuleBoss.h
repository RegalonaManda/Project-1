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
	

	int positionX;
	int positionY;

	Collider* headCollider;

	SDL_Texture* text = nullptr;

	Animation* current;

	bool fallen = false;

	Animation fallAnim;

	void Start() {
		
		text = App->textures->Load("Assets/Boss spsheet Proto.png");
		headCollider = App->collisions->AddCollider({ 0,0,48,40 }, Collider::Type::ENEMY_SHOT, (Module*)App->bossfight);

		fallAnim.PushBack({ 992,1422,49,49 });
		fallAnim.loop = true; 

			current = &fallAnim;

	};

	int FinalX;

	void Trajectory() {
		if (positionX > FinalX) { 
			positionX-=2; 
		}
		else {
			positionY+=2; 
		}

		if (positionY >= 200) {
			fallen = true;
		}
		headCollider->SetPos(positionX, positionY);
	}


	void Draw() {

		SDL_Rect Head = current->GetCurrentFrame();
		App->render->Blit(text, positionX, positionY, &Head);


	}

};


class AttackPattern {

public:

	Head headAttack[6];
	int activeHeads = 1;

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

	void Attack(AttackPattern& currentPattern);

public:

	fPoint position;

	int cooldown = 60;

	int speed;

	
	SDL_Texture* texture = nullptr;

	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction

	SDL_Rect initialframe = { 1185,744,62,144 };

	Animation* currentAnim;

	Animation transform; //grow + cloud (?)
	Animation idleAnim;
	Animation attackAnim;
	Animation hitAnim;
	Animation deathAnim;

	fPoint spawnPos;

	Collider* colliderBoss = nullptr;

	// A flag to detect when the player has been destroyed
	bool beaten = false;
	bool transformed = false;
	int RandID;

	AttackPattern* currentPattern;

	AttackPattern pattern[4];

	int attackCnt = 40;
	int currentHead = 0;
	int currentPattern_ = 0;

	// Sound effects indices
	uint welcomeDoom = 0;
	uint headThrow = 0;
	uint deathExpl = 0;

};





#endif 
