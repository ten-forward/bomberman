#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#ifdef ANDROID
namespace ouya {
	extern const int UP;
	extern const int DOWN;
	extern const int LEFT;
	extern const int RIGHT;
	extern const int O;
	extern const int U;
	extern const int Y;
	extern const int A;
	extern const int L1;
	extern const int R1;
	extern const int L2;
	extern const int R2;
	extern const int L3;
	extern const int R3;
}
#endif

namespace bomberman {
namespace constants {

	// Resolution
	extern const int SCREEN_WIDTH;
	extern const int SCREEN_HEIGHT;

	// Overscan compensation
	extern const int SAFE_WIDTH;
	extern const int SAFE_HEIGHT;
	extern const int SAFE_X;
	extern const int SAFE_Y;

	// Game parameters
	extern const int MAP_X;
	extern const int MAP_Y;

	extern const int MAP_COLUMNS;
	extern const int MAP_ROWS;

	extern const int TILE_WIDTH;
	extern const int TILE_HEIGHT;

	extern const int PLAYER_WIDTH;
	extern const int PLAYER_HEIGHT;

	extern const int SUBTILE_WIDTH;

	extern const int PLAYER_SPEED;
	extern const int PLAYER_UPDATE_DELAY;
	extern const int PLAYER_FRAME_UPDATE_DELAY;
}
}

#endif // CONSTANTS_HPP