#include "umpire.hpp"
#include "constants.hpp"

#include <algorithm>

namespace bomberman
{
	Umpire::~Umpire()
	{
	}

	void Umpire::Evolve(const std::vector<InputState>& iInputs, Uint32 iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const
	{
		auto umpire = std::make_shared<Umpire>(*this);
		iFutureMap->SetEntity(umpire);
	}
	
	void Umpire::Interact(const std::vector<InputState>& iInputs, Uint32 iTimestamp, const EntitySet &iOthers)
	{
	}
	
	void Umpire::Render(SDL_Renderer* iRenderer) const
	{
	}
	
	UmpirePtr Umpire::Create()
	{
		auto umpire = std::make_shared<Umpire>();
		umpire->id = constants::UMPIRE;
		umpire->x = 1;
		umpire->y = 1;
		umpire->elevel = -1;
		return umpire;
	}

	void Umpire::NotifyPlayerBorn(PlayerId id)
	{
		_playersAlive.insert(id);
		_bombCount.insert(std::make_pair(id, 0));
	}

	void Umpire::NotifyPlayerDied(PlayerId id)
	{
		_playersAlive.erase(id);
		_bombCount.erase(id);
	}

	bool Umpire::HasGameEnded() const
	{
		if ( GetPlayersRemaining() < 2)
		{
			return true;
		}

		return false;
	}

	PlayerId Umpire::GetRemainingPlayer() const
	{
		return _playersAlive.empty() ? constants::NO_PLAYER : *_playersAlive.begin();
	}

	int Umpire::GetPlayersRemaining() const
	{
		return _playersAlive.size();
	}

	int Umpire::GetBombCount(PlayerId id) const
	{
		auto iter =  _bombCount.find(id);
		return iter == _bombCount.end() ? -1 : iter->second;
	}

	void Umpire::IncrementBombCount(PlayerId id)
	{
		_bombCount[id]++;
	}

	void Umpire::DecrementBombCount(PlayerId id)
	{
		_bombCount[id]--;
	}

}