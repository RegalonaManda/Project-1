#pragma once;
#include "Enemy.h"
#include "ModuleEnemies.h"
#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleRender.h"
#include "Animation.h"
#include"ModuleTextures.h"

class ZombieDeath;

class ZombieDeath {

public:


	Animation deathAnim;
	Animation* currentanim = nullptr;
	SDL_Texture* texture;

	ZombieDeath() {

		texture = App->textures->Load("Assets/Enemies Proto.png");








		deathAnim.PushBack({ 127,1,41,68 });
		deathAnim.PushBack({ 132,1,41,68 });
		deathAnim.PushBack({ 127,1,41,68 });
		deathAnim.PushBack({ 132,1,41,68 });
		deathAnim.loop = false;
		deathAnim.totalFrames = 4;
		deathAnim.speed = 0.03f;
	}

	void KillZombie(int x, int y) {

		currentanim = &deathAnim;
		currentanim->Update();

		SDL_Rect rect = currentanim->GetCurrentFrame();
		App->render->Blit(texture, x, y - rect.h, &rect);

	}






};
