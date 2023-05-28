#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "SDL/include/SDL_Rect.h"

class Module;

struct Collider
{
	enum Type
	{
		NONE = -1,
		WALL,
		WALL_RIGHT,
		PLATFORM,
		PLAYER,
		ENEMY,
		PLAYER_SHOT,
		ENEMY_SHOT,
		CAMLIMIT,
		POWER_UP,
		ATTACK_XplosionTrigger,
		ENEMY_SELF_DESTRUCT,
		BORDER,
		RANGE,

		MAX
	};

	//Methods

	Collider (SDL_Rect rectangle, Type type, Module* listener = nullptr);

	Collider(SDL_FRect rectangle, Type type, Module* listener = nullptr);

	void SetPos(int x, int y);

	void UpdatePos(int x, int y);

	void SetProportions(int w, int h);

	void UpdateProportions(int w, int h);

	bool Intersects(const SDL_Rect& r) const;

	// Float rect functions

	void SetFloatPos(float x, float y);

	void UpdateFloatPos(float x, float y);

	void SetFloatProportions(float w, float h);

	void UpdateFloatProportions(float w, float h);

	bool IntersectsFloat(const SDL_FRect& r) const;

	//Variables
	SDL_Rect rect;

	SDL_FRect ract;

	bool pendingToDelete = false;
	Type type;
	Module* listener = nullptr;
};


#endif // !__COLLIDER_H__
