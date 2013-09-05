#include "utils.hpp"

SDL_Rect GetSafeArea1920()
{
	SDL_Rect rect;

	rect.w = SAFE_WIDTH;
	rect.h = SAFE_HEIGHT;
	rect.x = SAFE_X;
	rect.y = SAFE_Y;

	return rect;
}
