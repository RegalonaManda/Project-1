#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Globals.h"


#define NUM_MODULES 18 // 1 less than actual amount, to be used as arr index

//ALL NEW MODULES DEFINED HERE                                                                                                                                                              
class Module;
class ModuleWindow;
class ModuleInput;
class ModuleTextures;
class ModuleAudio;
class ScenePreIntro;
class SceneIntro;
class ModuleScene;
class ModulePlayer;
class ModuleBoss;
class ModuleParticles;
class ModuleEnemies;
class ModuleScene2;
class ModuleCollisions;
class ModuleFadeToBlack;
class ModuleFonts;
class ModulePower;
class ModuleRender;
class ModuleGreyScene;

class Application
{

public:

	//Constructor. Creates all necessary modules for the application
	Application();

	//Destructor. Removes all module objects
	~Application();

	//Initializes all modules
	bool Init();

	//Updates all modules (PreUpdate, Update and PostUpdate)
	update_status Update();

	//Releases all the application data
	bool CleanUp();

public:
	// An array to store all modules
	Module* modules[NUM_MODULES];

	// All APPLICATION MODULES INITIALIZED TO NULLPTR
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleTextures* textures = nullptr;
	ModuleAudio* audio = nullptr;
	ModulePlayer* player = nullptr;
	ScenePreIntro* scenePreIntro = nullptr;
	SceneIntro* sceneIntro = nullptr;
	ModuleScene* scene = nullptr;
	ModuleScene2* scene2 = nullptr;
	ModuleCollisions* collisions = nullptr;
	ModuleFadeToBlack* fade = nullptr;
	ModuleBoss* bossfight = nullptr;
	ModuleParticles* particles = nullptr;
	ModuleFonts* fonts = nullptr;
	ModulePower* powers = nullptr;
	ModuleEnemies* enemies = nullptr;
	ModuleRender* render = nullptr;
	ModuleGreyScene* grey_scene = nullptr;
	
};

// Global var made extern for Application ---
extern Application* App;

#endif // __APPLICATION_H__