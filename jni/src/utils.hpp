#pragma once

#include <SDL.h>

#ifdef ANDROID
namespace ouya {
	const int O = 96;
	const int U = 99;
	const int Y = 100;
	const int A = 97;
	const int L1 = 102;
	const int R1 = 103;
	const int L2 = 104;
	const int R2 = 105;
	const int L3 = 106;
	const int R3 = 107;
}
#endif

namespace bomberman {
namespace constants {

// Resolution
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

// Overscan compensation
const int SAFE_WIDTH  = 1728;
const int SAFE_HEIGHT  = 972;
const int SAFE_X = ((SCREEN_WIDTH-SAFE_WIDTH)/2);
const int SAFE_Y = ((SCREEN_HEIGHT-SAFE_HEIGHT)/2);

// Game numbers
const int MAP_X = (SAFE_X + 64);
const int MAP_Y = (SAFE_Y + 128);

const int MAP_COLUMNS = 25;
const int MAP_ROWS = 13;

const int TILE_WIDTH = 64;
const int TILE_HEIGHT = 64;

const int PLAYER_WIDTH = 64;
const int PLAYER_HEIGHT = 128;

const int SUBTILE_WIDTH = 8;
}

namespace utils {
	SDL_Rect GetSafeArea1920();
}

}

