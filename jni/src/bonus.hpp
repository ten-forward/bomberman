#pragma once

#include "entity.hpp"

#include <SDL.h>
#include <SDL_mixer.h>
// STL - libstdc++
#include <memory>

namespace bomberman {
namespace bonus {

	class Bonus;
	typedef std::shared_ptr<Bonus> BonusPtr;

	class Bonus : public Entity {
		public:
			static BonusPtr Create();
			virtual void Evolve(const std::vector<InputState>& iInputs, Uint32 iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const;
			virtual void Interact(const std::vector<InputState>& , Uint32 , const EntitySet &) {}
			virtual void Render(SDL_Renderer*) const;

			void Detonate();

		private:
			/*Uint32 _nextFrameDueTime;
			static void InitializeGraphicRessources(SDL_Renderer *iRenderer);
			static std::shared_ptr<SDL_Texture> _Bonus;*/
	};
}
}
