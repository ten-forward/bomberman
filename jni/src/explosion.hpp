#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include "entity.hpp"

// STL - libstdc++
#include <memory>

struct SDL_Texture;

namespace bomberman {
namespace arsenal {

	class Explosion;
	typedef std::shared_ptr<Explosion> ExplosionPtr;

	class Explosion : public Entity {
		public:
			static ExplosionPtr Create(int iTimeout);
			virtual void Render(SDL_Renderer*) const;
		private:
			int _timeout;
			int _stage;
			static void InitializeGraphicRessources(SDL_Renderer *iRenderer);
			static std::shared_ptr<SDL_Texture> _Explosion[4];
	};
}
}

#endif