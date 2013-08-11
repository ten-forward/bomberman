#include "bomb.hpp"

namespace bomberman {
namespace arsenal {
	BombPtr Bomb::Create() {
		return std::make_shared<Bomb>();
	}
}
}