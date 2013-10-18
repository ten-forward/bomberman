#pragma once

#include "entity.hpp"

#include <SDL.h>
// STL - libstdc++
#include <memory>

namespace bomberman {
namespace arsenal {

	class PropBomb;
	typedef std::shared_ptr<PropBomb> PropBombPtr;

	class PropBomb : public Entity {
		public:
			static PropBombPtr Create(int iTimeout, int iStrength, int playerId);
			virtual void Evolve(const std::vector<InputState>& iInputs, Uint32 iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const;
			virtual void Interact(const std::vector<InputState>& , Uint32 , const EntitySet &) {}
			virtual void Render(SDL_Renderer*) const;

			void Detonate();

		private:
			Uint32 _nextFrameDueTime;
			int _frameId;
			int _timeout;
			PlayerId _playerId;
			bool _detonating;
			int _strength; // In megaton
			static void InitializeGraphicRessources(SDL_Renderer *iRenderer);
			static std::shared_ptr<SDL_Texture> _Bomb;
	};
}
}
