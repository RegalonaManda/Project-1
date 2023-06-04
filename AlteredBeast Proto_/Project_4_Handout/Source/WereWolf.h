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
#define JUMPANIMSPEED 0.025f
#define AIRKICKSPEED 0.06f
#define Gravity 0.07f;


