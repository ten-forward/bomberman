#ifndef BOMB_HPP
#define BOMB_HPP

#include "entity.hpp"

// STL - libstdc++
#include <memory>

namespace bomberman {
namespace arsenal {

	class Bomb;
	typedef std::shared_ptr<Bomb> BombPtr;

	class Bomb : public Entity {
		public:
			static BombPtr Create();
			virtual void Render(SDL_Renderer*) const {};
	};
}
}

#endif