#include "Grave.h"
#include "Application.h"
#include "ModuleCollisions.h"
#include "Enemy.h"
#include "ModuleEnemies.h"




Graves::Graves(int x, int y) : Enemy(x, y) {

	texture = App->textures->Load("Assets/toomb.png");
	idle.PushBack({ 0,0,32,61 });

}

void Graves::Update() {

	Ecollider->SetPos(position.x, position.y);

	currentAnim = &idle;

	currentAnim->Update();
	Enemy::Update();

}

void Graves::OnCollision(Collider* collider) {



}
