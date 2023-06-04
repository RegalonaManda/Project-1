#include "ModuleScene2.h"
#include "ModuleBoss.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFonts.h"
#include "ModuleAudio.h"
#include "ModuleGreyScene.h"
#include "ModuleScene.h"
// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA



ModuleScene2::ModuleScene2(bool startEnabled) : Module(startEnabled)
{
	/*startEnabled = true;*/

	// Background / sky
	background.x = 0;
	background.y = 191;
	background.w = 7177;
	background.h = 84;

	grey_background.x = 0;
	grey_background.y = 191;
	grey_background.w = 7177;
	grey_background.h = 84;

	W_background.x = 3;
	W_background.y = 636;
	W_background.w = 320;
	W_background.h = 636;

	Fire.x = 0;
	Fire.y = 0;
	Fire.w = 320;
	Fire.h = 224;
	
	hpAnim.PushBack({ 1,10,46,8 });
	hpAnim.PushBack({ 48,10,46,8 });
	hpAnim.PushBack({ 95,10,46,8 });
	hpAnim.PushBack({ 1,19,46,8 });
	hpAnim.PushBack({ 48,19,46,8 });
	hpAnim.PushBack({ 95,19,46,8 });
	hpAnim.PushBack({ 1,28,46,8 });
	hpAnim.PushBack({ 48,28,46,8 });
	hpAnim.PushBack({ 95,28,46,8 });

	livesAnim.PushBack({ 177,10,23,16 });
	livesAnim.PushBack({ 201,10,23,16 });
	livesAnim.PushBack({ 225,10,23,16 });

	iconAnim.PushBack({ 142,10,17,15 });
	iconAnim.PushBack({ 160,10,17,15 });

	gameOverAnim.PushBack({ 0,0,140,20 });


	TranformationAnim.PushBack({ 4,0,122,117 });
	TranformationAnim.PushBack({ 127,0,122,117 });
	TranformationAnim.PushBack({ 250,0,122,117 });
	TranformationAnim.PushBack({ 373,0,122,117 });
	TranformationAnim.PushBack({ 496,0,122,117 });
	TranformationAnim.PushBack({ 6,128,122,117 });
	TranformationAnim.PushBack({ 129,128,122,117 });
TranformationAnim.PushBack({ 252,128,122,117 });
TranformationAnim.PushBack({ 375,128,122,117 });
TranformationAnim.PushBack({ 498,128,122,117 });
TranformationAnim.PushBack({ 621,128,122,117 });
TranformationAnim.loop = false;
TranformationAnim.speed = 0.11f;

FireAnim.PushBack({ 0,0,320,224 });
FireAnim.PushBack({ 320,0,320,224 });
FireAnim.loop = true;
FireAnim.speed = 0.1f;
}

ModuleScene2::~ModuleScene2()
{

}

// Load assets
bool ModuleScene2::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	layer1 = App->textures->Load("Assets/Layer1.png");
	grey_layer = App->textures->Load("Assets/grey_Layer1.png");
	uiTexture = App->textures->Load("Assets/UiElements.png");
	gameOverTexture = App->textures->Load("Assets/gameOver.png");

	Transf_background = App->textures->Load("Assets/transitionBackground.png");
	// looking for better quality fire
	Transf_Fire = App->textures->Load("Assets/Fire.png");
	Tranf_Portrait = App->textures->Load("Assets/TransformationPortrait.png");

	Ground = App->collisions->AddCollider({ 0,189,7177,84 }, Collider::Type::PLATFORM, (Module*)App->player);
	playOnce = 0;
	return ret;
}

update_status ModuleScene2::Update()
{



	return update_status::UPDATE_CONTINUE;

}

// Update: draw background
update_status ModuleScene2::PostUpdate()
{
	SDL_Rect rec;
	SDL_Rect rac;
	SDL_Rect ric;
	SDL_Rect reckt = { 0,0,140,20 };

	if (App->player->hp == 3)
	{
		rec = hpAnim.frames[0];
	}
	if (App->player->hp == 2)
	{
		rec = hpAnim.frames[3];
	}
	if (App->player->hp == 1)
	{
		rec = hpAnim.frames[6];
	}//se necessita investigar mas como funcionan las vidas i la hp

	if (App->player->lives == 3)
	{
		rac = livesAnim.frames[0];
	}
	if (App->player->lives == 2)
	{
		rac = livesAnim.frames[1];
	}
	if (App->player->lives == 1)
	{
		rac = livesAnim.frames[2];
	}//se necessita investigar mas como funcionan las vidas i la hp

	ric = iconAnim.frames[0]; // 0 or 1



	App->render->Blit(layer1, 0, 145, &background, 1.2);

	if (App->grey_scene->Grey == true) {
		App->render->Blit(grey_layer, 0, 145, &grey_background, 1.2);
	}

	App->render->Blit(uiTexture, 30, 200, &rec, 0); // blue hp dots
	App->render->Blit(uiTexture, 40, 10, &rac, 0);// golden lives
	App->render->Blit(uiTexture, 10, 10, &ric, 0);//human-wolf icon
	if (App->player->lives <= 0) {
		App->render->Blit(gameOverTexture, SCREEN_WIDTH / 2 - 70, SCREEN_HEIGHT / 2 - 10, &reckt, 0);//Game over text
		FadeCnt--;
		if (FadeCnt <= 0) {
			FadeCnt = 120;
			App->fade->FadeToBlack((Module*)App->scene, (Module*)App->sceneIntro, 60.0f);
		}
		
	}
	
	if (App->bossfight->beaten == true) {
		SDL_Rect ClearRec = { 0,0,225,10 };
		App->render->Blit(uiTexture, 48, 72, &ClearRec, 0);
		if (playOnce == 0) {
			App->audio->PlayMusic("Assets/Music/Win.ogg", 1.0f);
			playOnce++;
		}


		FadeCnt--;
		App->grey_scene->Grey = false;
		/*
		if (App->bossfight->IsEnabled()) {
			App->audio->PlayMusic("Assets/Music/Win.ogg", 1.0f);
			App->bossfight->Disable();
			App->grey_scene->Grey = false;
		}*/

		if (FadeCnt <= 0) {
			FadeCnt = 120;

			App->bossfight->beaten = false;
			App->fade->FadeToBlack((Module*)App->scene, (Module*)App->sceneIntro);

		}

	}
	
	if (wolfTransform == true) {
		current = &TranformationAnim;
		fireCurrent = &FireAnim;
		wolfTransformY--;
		App->render->Blit(Transf_background, 0, wolfTransformY--, &W_background, 0);
		

		App->render->Blit(Tranf_Portrait, SCREEN_WIDTH/2 -60, 105, &current->GetCurrentFrame(), 0);
		App->render->Blit(Transf_Fire, 0, 1, &fireCurrent->GetCurrentFrame(), 0);

		if (TranformationAnim.HasFinished() == true) {
			TranformationAnim.Reset();
			TranformationAnim.loopCount = 0;
			wolfTransform = false;
			App->player->wolfTransformCnt = 0;
			wolfTransformY = 0;
			
		}

		current->Update();
		fireCurrent->Update();
	}
	

	return update_status::UPDATE_CONTINUE;
}


bool ModuleScene2::CleanUp() {

	LOG("Freeing Scene2 textures")
	App->textures->Unload(layer1);
	App->textures->Unload(grey_layer);
	App->textures->Unload(uiTexture);
	App->textures->Unload(gameOverTexture);
	App->textures->Unload(Tranf_Portrait);
	App->textures->Unload(Transf_background);
	App->textures->Unload(Transf_Fire);

	return true;
}