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
			static PlayerPtr Create(const std::string &iName, int iInputStateIdx);
			virtual void Evolve(const std::vector<InputState>& iInputs, Uint32 iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const;
			virtual void Interact(const std::vector<InputState>& , Uint32 , const EntitySet &) {}
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
				IdleRight,
				Dying
			};

			std::string _name;
			int _frameId;
			Uint32 _nextFrameDueTime;
			State _state;
			int _inputStateIdx;

			int GetFrameIndex() const;

			static void InitializeGraphicRessources(SDL_Renderer *iRenderer);
			static std::shared_ptr<SDL_Texture> _Bomberman;
	};
}
}
