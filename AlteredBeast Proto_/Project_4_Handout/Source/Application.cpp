#include "Application.h"

#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "SceneIntro.h"
#include "ModuleScene.h"
#include "ModuleRender.h"
#include "ModuleScene2.h"
#include "ModuleCollisions.h"
#include "ModuleFadeToBlack.h"
#include "ModuleEnemies.h"
#include "ModuleFonts.h"
#include "ModuleParticles.h"
#include "ModulePower.h"
#include "ScenePreIntro.h"

// SHOULD INCLUDE ALL MODULE .h FILES

Application::Application()
{
	// The order in which the modules are added is very important.
	// It will define the order in which Pre/Update/Post will be called
	// Render should always be last, as our last action should be updating the screen
	modules[0] =	window =		new ModuleWindow(true);
	modules[1] =	input =			new ModuleInput(true);
	modules[2] =	textures =		new ModuleTextures(true);
	modules[3] =	audio =			new ModuleAudio(true);

	modules[6] =	scenePreIntro =	new ScenePreIntro(true);
	modules[7] =	sceneIntro =	new SceneIntro(false);
	modules[4] =	scene =			new ModuleScene(false);
	modules[5] =	player =		new ModulePlayer(false);
	modules[8] =	enemies =		new ModuleEnemies(false);
	
	modules[9] =	particles =		new ModuleParticles(true);
	modules[10] =	scene2 =		new ModuleScene2(false);
	modules[11] =	collisions =	new ModuleCollisions(false);
	modules[12] =	fade =			new ModuleFadeToBlack(true);
	modules[13] =	powers =		new ModulePower(false);
	modules[14] =	fonts =			new ModuleFonts(true);
	// render should always be the last module
	modules[15] =	render =		new ModuleRender(true);
}

Application::~Application()
{
	for (int i = 0; i < NUM_MODULES; ++i)
	{
		//Important: when deleting a pointer, set it to nullptr afterwards
		//It allows us for null check in other parts of the code
		delete modules[i];
		modules[i] = nullptr;
	}
}

bool Application::Init()
{
	bool ret = true;

	// All modules (active and disabled) will be initialized
	for (int i = 0; i < NUM_MODULES && ret; ++i)
		ret = modules[i]->Init();

	// Only active modules will be 'started'
	for (int i = 0; i < NUM_MODULES && ret; ++i)
		ret = modules[i]->IsEnabled() ? modules[i]->Start() : true;

	return ret;
}

update_status Application::Update()
{
	update_status ret = update_status::UPDATE_CONTINUE;

	for (int i = 0; i < NUM_MODULES && ret == update_status::UPDATE_CONTINUE; ++i)
		ret = modules[i]->IsEnabled() ? modules[i]->PreUpdate() : update_status::UPDATE_CONTINUE;

	for (int i = 0; i < NUM_MODULES && ret == update_status::UPDATE_CONTINUE; ++i)
		ret = modules[i]->IsEnabled() ? modules[i]->Update() : update_status::UPDATE_CONTINUE;

	for (int i = 0; i < NUM_MODULES && ret == update_status::UPDATE_CONTINUE; ++i)
		ret = modules[i]->IsEnabled() ? modules[i]->PostUpdate() : update_status::UPDATE_CONTINUE;

	return ret;
		
}
 
bool Application::CleanUp()
{
	bool ret = true;

	for (int i = NUM_MODULES - 1; i >= 0 && ret; --i)
		ret = modules[i]->IsEnabled() ? modules[i]->CleanUp() : true;

	return ret;
}
