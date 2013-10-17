#pragma once

#include "entity.hpp"
#include "bonus.hpp"

// SDL
#include <SDL.h>
#include <SDL_mixer.h>

// STL- libstdc++
#include <memory>
#include <string>

namespace bomberman {
namespace bestiary {

	class Player;

	typedef std::shared_ptr<Player> PlayerPtr;

	class Player : public bomberman::Entity {
		public:
			static PlayerPtr Create(const std::string &iName, const std::string &iSpriteName, int iInputStateIdx, SDL_Renderer* iRenderer, bool* alive);
			virtual void Evolve(const std::vector<InputState>& iInputs, Uint32 iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const;
			virtual void Interact(const std::vector<InputState>& , Uint32 , const EntitySet &);
			virtual void Render(SDL_Renderer*) const;
			void Kill();
			int GetPlayerIndex() const { return _inputStateIdx; }
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
			int _availableBombs;
			int _bombStrength;

			int _nbProBomb;

			int GetFrameIndex() const;

			void InitializeGraphicRessources(SDL_Renderer *iRenderer);
			std::shared_ptr<SDL_Texture> _Bomberman;

			static std::shared_ptr<Mix_Chunk> _bombPlaceSound;
			static State DynamicToStaticState(State iState);

			void EvolutionRoutine(const PlayerPtr thePlayer, const std::vector<InputState>& iInputs, Uint32 iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const;
			void ConsumeBonus(const bonus::BonusPtr &iBonus);
	};
}
}
