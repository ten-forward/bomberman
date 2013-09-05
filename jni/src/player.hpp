#pragma once

#include "entity.hpp"
#include <SDL.h>

// STL- libstdc++
#include <memory>
#include <string>

// Forward declarations

namespace bomberman {
namespace bestiary {

	class Player;

	typedef std::shared_ptr<Player> PlayerPtr;

	class Player : public bomberman::Entity {
		public:
			static PlayerPtr Create(const std::string &iName);
			virtual void Evolve(const InputState& iInputs, Uint32 iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const;
			virtual void Interact(const InputState& , Uint32 , const EntitySet &) {}
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
			Uint32 _nextFrameDueTime;
			State _state;

			int GetFrameIndex() const;

			static void InitializeGraphicRessources(SDL_Renderer *iRenderer);
			static std::shared_ptr<SDL_Texture> _Bomberman;
	};
}
}
