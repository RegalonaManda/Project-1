#include "ModuleCollisions.h"

#include "Application.h"

#include "ModuleRender.h"
#include "ModuleInput.h"
#include "SDL/include/SDL.h"

ModuleCollisions::ModuleCollisions(bool startEnabled) : Module(startEnabled)
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[Collider::Type::WALL][Collider::Type::WALL] = false;
	matrix[Collider::Type::WALL][Collider::Type::WALL_RIGHT] = false;
	matrix[Collider::Type::WALL][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::WALL][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::WALL][Collider::Type::PLAYER_SHOT] = true;
	matrix[Collider::Type::WALL][Collider::Type::ENEMY_SHOT] = true;
	matrix[Collider::Type::WALL][Collider::Type::CAMLIMIT] = false;
	matrix[Collider::Type::WALL][Collider::Type::POWER_UP] = false;
	matrix[Collider::Type::WALL][Collider::Type::ATTACK_XplosionTrigger] = false;
	matrix[Collider::Type::WALL][Collider::Type::ENEMY_SELF_DESTRUCT] = false;
	matrix[Collider::Type::WALL][Collider::Type::PLATFORM] = false;

	matrix[Collider::Type::WALL_RIGHT][Collider::Type::WALL] = false;
	matrix[Collider::Type::WALL_RIGHT][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::WALL_RIGHT][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::WALL_RIGHT][Collider::Type::PLAYER_SHOT] = true;
	matrix[Collider::Type::WALL_RIGHT][Collider::Type::ENEMY_SHOT] = true;
	matrix[Collider::Type::WALL_RIGHT][Collider::Type::CAMLIMIT] = false;
	matrix[Collider::Type::WALL_RIGHT][Collider::Type::POWER_UP] = false;
	matrix[Collider::Type::WALL_RIGHT][Collider::Type::ATTACK_XplosionTrigger] = false;
	matrix[Collider::Type::WALL_RIGHT][Collider::Type::ENEMY_SELF_DESTRUCT] = false;
	matrix[Collider::Type::WALL_RIGHT][Collider::Type::PLATFORM] = false;

	matrix[Collider::Type::PLATFORM][Collider::Type::WALL] = false;
	matrix[Collider::Type::PLATFORM][Collider::Type::WALL_RIGHT] = false;
	matrix[Collider::Type::PLATFORM][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::PLATFORM][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::PLATFORM][Collider::Type::PLAYER_SHOT] = true;
	matrix[Collider::Type::PLATFORM][Collider::Type::ENEMY_SHOT] = true;
	matrix[Collider::Type::PLATFORM][Collider::Type::CAMLIMIT] = false;
	matrix[Collider::Type::PLATFORM][Collider::Type::POWER_UP] = false;
	matrix[Collider::Type::PLATFORM][Collider::Type::ATTACK_XplosionTrigger] = false;
	matrix[Collider::Type::PLATFORM][Collider::Type::ENEMY_SELF_DESTRUCT] = false;


	matrix[Collider::Type::PLAYER][Collider::Type::WALL] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::WALL_RIGHT] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::PLAYER_SHOT] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::ENEMY_SHOT] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::CAMLIMIT] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::POWER_UP] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::ATTACK_XplosionTrigger] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::ENEMY_SELF_DESTRUCT] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::PLATFORM] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::BORDER] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::RANGE] = true;


	matrix[Collider::Type::ENEMY][Collider::Type::WALL] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::WALL_RIGHT] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::PLAYER_SHOT] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::ENEMY_SHOT] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::CAMLIMIT] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::POWER_UP] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::ATTACK_XplosionTrigger] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::ENEMY_SELF_DESTRUCT] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::PLATFORM] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::BORDER] = true;


	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::WALL] = false;
	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::WALL_RIGHT] = false;
	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::PLAYER_SHOT] = false;
	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::ENEMY_SHOT] = false;
	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::CAMLIMIT] = true;
	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::POWER_UP] = false;
	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::ATTACK_XplosionTrigger] = false;
	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::ENEMY_SELF_DESTRUCT] = false;
	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::PLATFORM] = false;
	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::BORDER] = false;

	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::WALL] = true;
	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::WALL_RIGHT] = true;
	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::PLAYER_SHOT] = false;
	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::ENEMY_SHOT] = false;
	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::CAMLIMIT] = false;
	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::POWER_UP] = false;
	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::ATTACK_XplosionTrigger] = false;
	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::ENEMY_SELF_DESTRUCT] = false;
	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::PLATFORM] = true;
	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::BORDER] = false;


	matrix[Collider::Type::CAMLIMIT][Collider::Type::WALL] = false;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::WALL_RIGHT] = false;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::PLAYER_SHOT] = false;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::ENEMY_SHOT] = false;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::CAMLIMIT] = false;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::POWER_UP] = false;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::ATTACK_XplosionTrigger] = false;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::ENEMY_SELF_DESTRUCT] = false;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::PLATFORM] = false;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::BORDER] = false;

	matrix[Collider::Type::POWER_UP][Collider::Type::WALL] = false;
	matrix[Collider::Type::POWER_UP][Collider::Type::WALL_RIGHT] = false;
	matrix[Collider::Type::POWER_UP][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::POWER_UP][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::POWER_UP][Collider::Type::PLAYER_SHOT] = false;
	matrix[Collider::Type::POWER_UP][Collider::Type::ENEMY_SHOT] = false;
	matrix[Collider::Type::POWER_UP][Collider::Type::CAMLIMIT] = false;
	matrix[Collider::Type::POWER_UP][Collider::Type::ATTACK_XplosionTrigger] = false;
	matrix[Collider::Type::POWER_UP][Collider::Type::ENEMY_SELF_DESTRUCT] = false;
	matrix[Collider::Type::POWER_UP][Collider::Type::PLATFORM] = false;
	matrix[Collider::Type::CAMLIMIT][Collider::Type::BORDER] = false;

						  
	matrix[Collider::Type::ATTACK_XplosionTrigger][Collider::Type::WALL] = false;
	matrix[Collider::Type::ATTACK_XplosionTrigger][Collider::Type::WALL_RIGHT] = false;
	matrix[Collider::Type::ATTACK_XplosionTrigger][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::ATTACK_XplosionTrigger][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::ATTACK_XplosionTrigger][Collider::Type::PLAYER_SHOT] = false;
	matrix[Collider::Type::ATTACK_XplosionTrigger][Collider::Type::ENEMY_SHOT] = false;
	matrix[Collider::Type::ATTACK_XplosionTrigger][Collider::Type::CAMLIMIT] = false;
	matrix[Collider::Type::ATTACK_XplosionTrigger][Collider::Type::POWER_UP] = false;
	matrix[Collider::Type::ATTACK_XplosionTrigger][Collider::Type::ENEMY_SELF_DESTRUCT] = false;
	matrix[Collider::Type::ATTACK_XplosionTrigger][Collider::Type::PLATFORM] = false;
	matrix[Collider::Type::ATTACK_XplosionTrigger][Collider::Type::BORDER] = false;

	matrix[Collider::Type::RANGE][Collider::Type::WALL] = false;
	matrix[Collider::Type::RANGE][Collider::Type::WALL_RIGHT] = false;
	matrix[Collider::Type::RANGE][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::RANGE][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::RANGE][Collider::Type::PLAYER_SHOT] = false;
	matrix[Collider::Type::RANGE][Collider::Type::ENEMY_SHOT] = false;
	matrix[Collider::Type::RANGE][Collider::Type::CAMLIMIT] = false;
	matrix[Collider::Type::RANGE][Collider::Type::POWER_UP] = false;
	matrix[Collider::Type::RANGE][Collider::Type::ENEMY_SELF_DESTRUCT] = false;
	matrix[Collider::Type::RANGE][Collider::Type::PLATFORM] = false;
	matrix[Collider::Type::RANGE][Collider::Type::BORDER] = false;
	matrix[Collider::Type::RANGE][Collider::Type::ATTACK_XplosionTrigger] = false;

	matrix[Collider::Type::ENEMY_SELF_DESTRUCT][Collider::Type::WALL] = false;
	matrix[Collider::Type::ENEMY_SELF_DESTRUCT][Collider::Type::WALL_RIGHT] = false;
	matrix[Collider::Type::ENEMY_SELF_DESTRUCT][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::ENEMY_SELF_DESTRUCT][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::ENEMY_SELF_DESTRUCT][Collider::Type::PLAYER_SHOT] = false;
	matrix[Collider::Type::ENEMY_SELF_DESTRUCT][Collider::Type::POWER_UP] = false;
	matrix[Collider::Type::ENEMY_SELF_DESTRUCT][Collider::Type::ENEMY_SHOT] = false;
	matrix[Collider::Type::ENEMY_SELF_DESTRUCT][Collider::Type::CAMLIMIT] = false;
	matrix[Collider::Type::ENEMY_SELF_DESTRUCT][Collider::Type::ATTACK_XplosionTrigger] = false;
	matrix[Collider::Type::ENEMY_SELF_DESTRUCT][Collider::Type::PLATFORM] = false;
	matrix[Collider::Type::ENEMY_SELF_DESTRUCT][Collider::Type::BORDER] = false;

	matrix[Collider::Type::BORDER][Collider::Type::WALL] = false;
	matrix[Collider::Type::BORDER][Collider::Type::WALL_RIGHT] = false;
	matrix[Collider::Type::BORDER][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::BORDER][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::BORDER][Collider::Type::PLAYER_SHOT] = false;
	matrix[Collider::Type::BORDER][Collider::Type::POWER_UP] = false;
	matrix[Collider::Type::BORDER][Collider::Type::ENEMY_SHOT] = false;
	matrix[Collider::Type::BORDER][Collider::Type::CAMLIMIT] = false;
	matrix[Collider::Type::BORDER][Collider::Type::ATTACK_XplosionTrigger] = false;
	matrix[Collider::Type::BORDER][Collider::Type::PLATFORM] = false;


}

// Destructor


update_status ModuleCollisions::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->pendingToDelete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->Intersects(c2->rect))
			{
				if (matrix[c1->type][c2->type] && c1->listener)
					c1->listener->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->listener)
					c2->listener->OnCollision(c2, c1);
			}
		}
	}

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleCollisions::Update()
{
	if (App->input->keys[SDL_SCANCODE_F2] == KEY_DOWN ||App->input->pads[0].r3)
		debug = !debug;

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleCollisions::PostUpdate()
{
	if (debug)
		DebugDraw();

	return update_status::UPDATE_CONTINUE;
}

void ModuleCollisions::DebugDraw()
{
	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case Collider::Type::NONE: // white
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case Collider::Type::WALL: // blue
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
			break;
		case Collider::Type::WALL_RIGHT: // blue
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
			break;
		case Collider::Type::PLAYER: // green
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case Collider::Type::ENEMY: // red
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha); 
			break;
		case Collider::Type::PLAYER_SHOT: // yellow
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 0, alpha);
			break;
		case Collider::Type::ENEMY_SHOT: // magenta? this is fucking turquoise
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 255, alpha);
			break;
		case Collider::Type::CAMLIMIT: //black
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 0, alpha);
			break;

		case Collider::Type::POWER_UP: //violet
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 255, alpha);
			break;

		case Collider::Type::ATTACK_XplosionTrigger: //orange
			App->render->DrawQuad(colliders[i]->rect, 255, 165, 0, alpha);
			break;
		case Collider::Type::RANGE: //orange
			App->render->DrawQuad(colliders[i]->rect, 255, 180, 0, alpha);
			break;

		case Collider::Type::ENEMY_SELF_DESTRUCT: //white
			App->render->DrawQuad(colliders[i]->rect, 165, 42, 42, alpha);
			break;
		case Collider::Type::PLATFORM: //pinky
			App->render->DrawQuad(colliders[i]->rect, 204, 0, 102, alpha);
			break;
		case Collider::Type::BORDER: //BROWN
			App->render->DrawQuad(colliders[i]->rect, 101, 67, 33, alpha);
			break;
		}

	}
	
}



// Called before quitting
bool ModuleCollisions::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* ModuleCollisions::AddCollider(SDL_Rect rect, Collider::Type type, Module* listener)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, listener);
			break;
		}
	}

 	return ret;
}


Collider* ModuleCollisions::AddFloatCollider(SDL_FRect rect, Collider::Type type, Module* listener)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, listener);
			break;
		}
	}

	return ret;
}