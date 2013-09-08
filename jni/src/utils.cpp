#include "constants.hpp"
#include "utils.hpp"

namespace bomberman {
namespace utils {
	SDL_Rect GetSafeArea1920()
	{
		using namespace bomberman::constants;

		SDL_Rect rect;

		rect.w = SAFE_WIDTH;
		rect.h = SAFE_HEIGHT;
		rect.x = SAFE_X;
		rect.y = SAFE_Y;

		return rect;
	}
}
}

