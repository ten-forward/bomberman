#pragma once

#include "entity.hpp"
#include <SDL.h>
#include <SDL_mixer.h>

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
			static PlayerPtr Create(const std::string &iName, const std::string &iSpriteName, int iInputStateIdx, SDL_Renderer* iRenderer, bool* alive);
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

			std::string _spriteName;
			std::string _name;
			int _frameId;
			Uint32 _nextFrameDueTime;
			Uint32 _nextUpdateDueTime;
			State _state;
			int _inputStateIdx;

			bool* _alive;

			int GetFrameIndex() const;

			void InitializeGraphicRessources(SDL_Renderer *iRenderer);
			std::shared_ptr<SDL_Texture> _Bomberman;

			static std::shared_ptr<Mix_Chunk> _bombPlaceSound;
			static State DynamicToStaticState(State iState);
	};
}
}
