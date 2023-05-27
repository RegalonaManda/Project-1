#include "Colliders.h"

Collider::Collider(SDL_Rect rectangle, Type type, Module* listener) : rect(rectangle), type(type), listener(listener)
{

}

Collider::Collider(SDL_FRect rectangle, Type type, Module* listener) : ract(rectangle), type(type), listener(listener), rect({ 1,1,50,50 })
{

}

void Collider::SetPos(int x, int y)
  {
	rect.x = x;
	rect.y = y;
}

void Collider::UpdatePos(int x, int y) {
	rect.x += x;
	rect.y += y;
}

void Collider::SetProportions(int w, int h) {
	rect.w = w;
	rect.h = h;

}

void Collider::UpdateProportions(int w, int h) {
	rect.w += w;
	rect.h += h;

}

bool Collider::Intersects(const SDL_Rect& r) const
{
	// returns true if there has been an intersection
	return (rect.x < r.x + r.w &&
		rect.x + rect.w > r.x &&
		rect.y < r.y + r.h &&
		rect.h + rect.y > r.y);
}


void Collider::SetFloatPos(float x, float y)
{
	ract.x = x;
	ract.y = y;
}

void Collider::UpdateFloatPos(float x, float y) {
	ract.x += x;
	ract.y += y;
}

void Collider::SetFloatProportions(float w, float h) {
	ract.w = w;
	ract.h = h;

}

void Collider::UpdateFloatProportions(float w, float h) {
	ract.w += w;
	ract.h += h;

}

bool Collider::IntersectsFloat(const SDL_FRect& r) const
{
	// returns true if there has been an intersection
	return (ract.x < r.x + r.w &&
		ract.x + ract.w > r.x &&
		ract.y < r.y + r.h &&
		ract.h + ract.y > r.y);
}