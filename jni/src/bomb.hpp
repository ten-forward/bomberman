#pragma once

#include "entity.hpp"

#include <SDL.h>
// STL - libstdc++
#include <memory>

namespace bomberman {
namespace arsenal {

	class Bomb;
	typedef std::shared_ptr<Bomb> BombPtr;

	class Bomb : public Entity {
		public:
			static BombPtr Create(int iTimeout, int iStrength);
			virtual void Evolve(const InputState& iInputs, Uint32 iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const;
			virtual void Interact(const InputState& , Uint32 , const EntitySet &) {}
			virtual void Render(SDL_Renderer*) const;

			void Detonate();

		private:
			int _timeout;
			bool _detonating;
			int _strength; // In megaton
			static void InitializeGraphicRessources(SDL_Renderer *iRenderer);
			static std::shared_ptr<SDL_Texture> _Bomb;
	};
}
}
