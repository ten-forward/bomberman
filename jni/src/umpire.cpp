#include "umpire.hpp"
#include "constants.hpp"

#include <algorithm>

namespace bomberman
{
	Umpire::Umpire()
	{
		std::fill(_bombCount.begin(), _bombCount.end(), 0);
		std::fill(_playersAlive.begin(), _playersAlive.end(), false);
	}

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
		_playersAlive[id] = true;
	}

	void Umpire::NotifyPlayerDied(PlayerId id)
	{
		_playersAlive[id] = false;
	}

	bool Umpire::HasGameEnded() const
	{
		if (GetPlayersRemaining() < 2)
		{
			return true;
		}

		return false;
	}

	int Umpire::GetPlayersRemaining() const
	{
		int count = 0;
		for (int i=0;i<4;i++)
		{
			if ( _playersAlive[i] )
			{
				count++;
			}
		}
		return count;
	}

	PlayerId Umpire::GetRemainingPlayer() const
	{
		PlayerId player = constants::NO_PLAYER;
		for (int i=0;i<4;i++)
		{
			if ( _playersAlive[i] )
			{
				player = i;
			}
		}
		return player;
	}

	int Umpire::GetBombCount(PlayerId id) const
	{
		return _bombCount[id];
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