#pragma once

#include "entityptr.hpp"

namespace bomberman {
namespace constants {

	const EntityID UMPIRE = 0x134515;

	const int DEFAULT_ELEVEL = 0;
	const int UMPIRE_ELEVEL = -1;
	const int SOFTBLOCK_ELEVEL = -1;
	const int FLOOR_ELEVEL = -1;
	const int BLOCK_ELEVEL = -1;

	const int PLAYER_ZLEVEL = 2;	// For blocks, bombs and players
	const int BLOCK_ZLEVEL = 2;
	const int BOMB_ZLEVEL = 2;
	const int CORPSE_ZLEVEL = 1;
	const int EXPLOSION_ZLEVEL = 3;
	const int BONUS_ZLEVEL = 1;
	const int FLOOR_ZLEVEL = 0;

	const int NO_PLAYER = -1;

}
}
