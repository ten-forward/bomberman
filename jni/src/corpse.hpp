#pragma once

#include "entity.hpp"
#include <SDL.h>

// STL- libstdc++
#include <memory>
#include <string>

// Forward declarations

namespace bomberman {
namespace bestiary {

	class Corpse;

	typedef std::shared_ptr<Corpse> CorpsePtr;

	class Corpse : public bomberman::Entity {
		public:
			static CorpsePtr Create(std::shared_ptr<SDL_Texture> corpseSprite);
			virtual void Evolve(const std::vector<InputState>& iInputs, Uint32 iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const;
			virtual void Interact(const std::vector<InputState>& , Uint32 , const EntitySet &) {}
			virtual void Render(SDL_Renderer*) const;
		protected:
			
			int _frameId;
			Uint32 _nextFrameDueTime;
			Uint32 _nextUpdateDueTime;
			std::shared_ptr<SDL_Texture> _BombermanCorpse;
	};
}
}
