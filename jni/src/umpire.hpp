#pragma once

#include "entity.hpp"
#include <map>
#include <set>

namespace bomberman
{
	class Umpire;
	typedef std::shared_ptr<Umpire> UmpirePtr;
	class Umpire : public Entity
	{
		public:

			virtual ~Umpire();

			static UmpirePtr Create();

			virtual void Evolve(const std::vector<InputState>& iInputs, Uint32 iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const;
			virtual void Interact(const std::vector<InputState>& iInputs, Uint32 iTimestamp, const EntitySet &iOthers);
			virtual void Render(SDL_Renderer* iRenderer) const;

			void NotifyPlayerBorn(PlayerId id);
			void NotifyPlayerDied(PlayerId id);

			bool HasGameEnded() const;
			int GetPlayersRemaining() const;
			
			PlayerId GetRemainingPlayer() const;

			int GetBombCount(PlayerId id) const;
			void IncrementBombCount(PlayerId id);

			void DecrementBombCount(PlayerId id);

		private:
			std::map<PlayerId, int> _bombCount;
			std::set<PlayerId> _playersAlive;
	};
}
