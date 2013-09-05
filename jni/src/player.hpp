#pragma once

#include "entity.hpp"

// STL- libstdc++
#include <memory>
#include <string>

// Forward declarations
struct SDL_Texture;

namespace bomberman {
namespace bestiary {

	class Player;

	typedef std::shared_ptr<Player> PlayerPtr;

	class Player : public bomberman::Entity {
		public:
			static PlayerPtr Create(const std::string &iName);
			virtual void Evolve(const InputState& iInputs, uint32_t iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const;
			virtual void Interact(const InputState& , uint32_t , const EntitySet &) {}
			virtual void Render(SDL_Renderer*) const;
			void Kill();
		protected:

			enum State {
				WalkingUp,
				WalkingDown,
				WalkingLeft,
				WalkingRight,
				IdleUp,
				IdleDown,
				IdleLeft,
				IdleRight
			};

			std::string _name;
			bool _dying;
			int _frameId;
			uint32_t _nextFrameDueTime;
			State _state;

			int GetFrameIndex() const;

			static void InitializeGraphicRessources(SDL_Renderer *iRenderer);
			static std::shared_ptr<SDL_Texture> _Bomberman;
	};
}
}
