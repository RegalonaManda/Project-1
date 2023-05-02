//#pragma once;
//#include "Enemy.h"
//#include "ModuleEnemies.h"
//#include "Application.h"
//#include "ModuleCollisions.h"
//#include "ModuleRender.h"
//#include "Animation.h"
//#include"ModuleTextures.h"
//#include "Module.h"
//
//#include "SDL/include/SDL.h"
//#include "SDL_image/include/SDL_image.h"
//#include <iostream>
//using namespace std;
//#pragma comment(lib, "SDL/libx86/SDL2.lib")
//#pragma comment(lib, "SDL/libx86/SDL2main.lib")
//#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )
//
//
//class ZombieDeath;
//
//class ZombieDeath {
//
//public:
//
//
//	Animation deathAnim;
//	Animation* currentanim = nullptr;
//	SDL_Texture* texture;
//	SDL_Surface* Surf;
//
//	ZombieDeath() {
//
//		//texture = App->textures->Load("Assets/EnemiesProto.png");
//		if (App != nullptr) {
//			Surf = IMG_Load("Assets/Enemies.png");
//			texture = SDL_CreateTextureFromSurface(App->render->renderer, Surf);
//
//
//		}
//
//
//
//		deathAnim.PushBack({ 127,1,41,68 });
//		deathAnim.PushBack({ 132,1,41,68 });
//		deathAnim.PushBack({ 127,1,41,68 });
//		deathAnim.PushBack({ 132,1,41,68 });
//		deathAnim.loop = false;
//		deathAnim.totalFrames = 4;
//		deathAnim.speed = 0.03f;
//	}
//
//	void KillZombie(int x, int y) {
//
//		currentanim = &deathAnim;
//		currentanim->Update();
//
//		SDL_Rect rect = currentanim->GetCurrentFrame();
//		App->render->Blit(texture, x, y - rect.h, &rect);
//
//	}
//
//
//
//
//
//
//};
