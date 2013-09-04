#pragma once

#include "entity.hpp"

// STL - libstdc++
#include <memory>

struct SDL_Texture;

namespace bomberman {
namespace arsenal {

	class Explosion;
	typedef std::shared_ptr<Explosion> ExplosionPtr;

	class Explosion : public Entity, public std::enable_shared_from_this<Explosion> {
		public:
			static ExplosionPtr Create(int iTimeout);
			virtual void Evolve(const InputState& iInputs, uint32_t iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const;
			virtual void Render(SDL_Renderer*) const;
		private:
			int _timeout;
			int _stage;
			static void InitializeGraphicRessources(SDL_Renderer *iRenderer);
			static std::shared_ptr<SDL_Texture> _Explosion[4];
	};
}
}
