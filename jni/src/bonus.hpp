#pragma once

#include "entity.hpp"

#include <SDL.h>
#include <SDL_mixer.h>
// STL - libstdc++
#include <memory>
#include <map>

namespace bomberman {
namespace bonus {

	class Bonus;
	typedef std::shared_ptr<Bonus> BonusPtr;

	class Bonus : public Entity {

		public:
			enum BonusType
			{
				BOMBCOUNT,
				BOMBSTRENGTH,
				PROPBOMB,
                REMOTEBOMB,
			};

			static BonusPtr Create();
			virtual void Evolve(const std::vector<InputState>& iInputs, Uint32 iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const;
			virtual void Interact(const std::vector<InputState>& , Uint32 , const EntitySet &) {}
			virtual void Render(SDL_Renderer*) const;
			virtual void NotifyConsumed() { _state = Consumed; }
			BonusType GetType() const { return _bonusType; }
			
		private:

			enum State {
				Available,
				Consumed,
			};

			BonusType _bonusType;
			State _state;

			static void InitializeGraphicRessources(SDL_Renderer *iRenderer);

			static std::map<BonusType, std::shared_ptr<SDL_Texture>> _BonusTexture;
	};
}
}
