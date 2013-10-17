#pragma once

#include "entityptr.hpp"

#ifdef ANDROID
namespace ouya {
	const int UP = 19;
	const int DOWN = 20;
	const int LEFT = 21;
	const int RIGHT = 22;
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
	const int OUYABUTTON = 82;
}
#endif

namespace bomberman {
namespace constants {

	const EntityID UMPIRE = 0x134515;

	const int DEFAULT_ELEVEL = 0;
	const int UMPIRE_ELEVEL = -1;
	const int SOFTBLOCK_ELEVEL = -1;
	const int FLOOR_ELEVEL = -1;
	const int BLOCK_ELEVEL = -1;

	const int NO_PLAYER = -1;

#ifdef SMALL_SCREEN

	// Resolution
	const int SCREEN_WIDTH = 960;
	const int SCREEN_HEIGHT = 540;

	// Overscan compensation
	const int SAFE_WIDTH  = 864;
	const int SAFE_HEIGHT  = 486;
	const int SAFE_X = ((SCREEN_WIDTH-SAFE_WIDTH)/2);
	const int SAFE_Y = ((SCREEN_HEIGHT-SAFE_HEIGHT)/2);

	// Game parameters
	const int MAP_X = (SAFE_X + 32);
	const int MAP_Y = (SAFE_Y + 32);

	const int MAP_COLUMNS = 25;
	const int MAP_ROWS = 13;

	const int TILE_WIDTH = 32;
	const int TILE_HEIGHT = 64;

	const int PLAYER_WIDTH = 32;
	const int PLAYER_HEIGHT = 64;
	
	const int SPRITE_MAGNIFICATION = 2;
	const int AMOUNT_PER_TILE = 8;
	const int SUBTILE_WIDTH = 4;

	const int PLAYER_SPEED = 1;
	const int PLAYER_UPDATE_DELAY = 20;
	const int PLAYER_FRAME_UPDATE_DELAY = 150;

	const int BOMB_FRAME_UPDATE_DELAY = 200;

	const int PLAYER_SPACE = 2;

	const int PLAYER_DASHBOARD_WIDTH = 200;
	const int PLAYER_DASHBOARD_HEIGHT = 60;
	const int PLAYER_DASHBOARD_X = 62;
	const int PLAYER_DASHBOARD_Y = 27;
	const int PLAYER_DASHBOARD_PADDING = 12;
	const int PLAYER_DASHBOARD_AVATAR_X = 8;
	const int PLAYER_DASHBOARD_AVATAR_Y = 8;

#else // SMALL_SCREEN

	// Resolution
	const int SCREEN_WIDTH = 1920;
	const int SCREEN_HEIGHT = 1080;

	// Overscan compensation
	const int SAFE_WIDTH  = 1728;
	const int SAFE_HEIGHT  = 972;
	const int SAFE_X = ((SCREEN_WIDTH-SAFE_WIDTH)/2);
	const int SAFE_Y = ((SCREEN_HEIGHT-SAFE_HEIGHT)/2);

	// Game parameters
	const int MAP_X = (SAFE_X + 64);
	const int MAP_Y = (SAFE_Y + 64);

	const int MAP_COLUMNS = 25;
	const int MAP_ROWS = 13;

	const int TILE_WIDTH = 64;
	const int TILE_HEIGHT = 128;

	const int PLAYER_WIDTH = 64;
	const int PLAYER_HEIGHT = 128;
	
	const int SPRITE_MAGNIFICATION = 4;
	const int AMOUNT_PER_TILE = 8;
	const int SUBTILE_WIDTH = 8;

	const int PLAYER_SPEED = 1;
	const int PLAYER_UPDATE_DELAY = 20;
	const int PLAYER_FRAME_UPDATE_DELAY = 150;

	const int BOMB_FRAME_UPDATE_DELAY = 200;

	const int PLAYER_SPACE = 2;

	const int PLAYER_DASHBOARD_WIDTH = 414;
	const int PLAYER_DASHBOARD_HEIGHT = 115;
	const int PLAYER_DASHBOARD_X = 108 ;
	const int PLAYER_DASHBOARD_Y = 56;
	const int PLAYER_DASHBOARD_PADDING = 16;
	const int PLAYER_DASHBOARD_AVATAR_X = 8;
	const int PLAYER_DASHBOARD_AVATAR_Y = 16;
	
#endif // SMALL SCREEN
}
}

