#include "bomb.hpp"
#include "explosion.hpp"
#include "constants.hpp"
#include "softblock.hpp"
#include "block.hpp"
#include "printlog.hpp"
#include "umpire.hpp"

#include <boost/foreach.hpp>
#include <SDL_image.h>

using bomberman::architecture::Block;
using bomberman::architecture::SoftBlock;

namespace bomberman {
namespace arsenal {

	namespace {
		void SetBlast(int iStrength, 
			int initX, 
			int initY,
			Explosion::Orientation orientation,
			uint32_t iTimestamp, 
			const MapConstPtr &iPresentMap,
			const MapPtr &iFutureMap, 
			std::function<void(int amount, int* x, int* y)> propagator)
		{
			for (int i=1; i<=iStrength; i++)
			{
				int x = initX;
				int y = initY;
				propagator(i, &x, &y);
				if(iPresentMap->CheckPosition(x,y) == Map::BOUNDARY 
					|| iPresentMap->CheckPosition(x,y) == Map::HARD_OCCUPIED)
				{
					break;
				}
					
				auto blast = Explosion::Create(iTimestamp, orientation);
				blast->x = x;
				blast->y = y;
				iFutureMap->SetEntity(blast);
				if (iPresentMap->CheckPosition(x,y) == Map::SOFT_OCCUPIED)
				{
					break;
				}
			}
		}	
	}

	BombPtr Bomb::Create(int iTimeout, int iStrength, int iPlayerId) 
	{
		auto bomb = std::make_shared<Bomb>();
		bomb->_timeout = iTimeout;
		bomb->_strength = iStrength;
		bomb->zlevel = 2;
		bomb->_detonating = false;
		bomb->_frameId = 0;
		bomb->_nextFrameDueTime = 0;
		bomb->_playerId = iPlayerId;
		return bomb;
	}

	std::shared_ptr<SDL_Texture>  Bomb::_Bomb;
	std::shared_ptr<Mix_Chunk> Bomb::_explosionSound;

	void Bomb::InitializeGraphicRessources(SDL_Renderer *iRenderer) 
	{
		if (!_Bomb)
		{
			_Bomb = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(iRenderer, "drawable/bomb.png"), SDL_DestroyTexture);
		}
		
		if (!_explosionSound)
		{
			_explosionSound = std::shared_ptr<Mix_Chunk>(Mix_LoadWAV("sound/explosion.wav"), Mix_FreeChunk);
			if (!_explosionSound)
			{
				printlog("Mix_LoadWAV: %s\n", Mix_GetError());
			}
		}
	}

	void Bomb::Evolve(const std::vector<InputState>& /*iInputs*/, uint32_t iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const
	{
		if (iTimestamp >= _timeout || _detonating) 
		{
			auto umpire = std::static_pointer_cast<Umpire>(iFutureMap->GetEntity(constants::UMPIRE));
			umpire->DecrementBombCount(_playerId);

			// center
			auto blast = Explosion::Create(iTimestamp, Explosion::IsoTropic);
			blast->x = x;
			blast->y = y;
			iFutureMap->SetEntity(blast);

			// outwards
			SetBlast(_strength, x, y, Explosion::Horizontal, iTimestamp, iPresentMap, iFutureMap,
				[](int i, int* x, int* y) { *x -= i; });
			SetBlast(_strength, x, y, Explosion::Vertical, iTimestamp, iPresentMap, iFutureMap,
				[](int i, int* x, int* y) { *y -= i; });
			SetBlast(_strength, x, y, Explosion::Horizontal, iTimestamp, iPresentMap, iFutureMap,
				[](int i, int* x, int* y) { *x += i; });
			SetBlast(_strength, x, y, Explosion::Vertical, iTimestamp, iPresentMap, iFutureMap,
				[](int i, int* x, int* y) { *y += i; });

			if (Mix_PlayChannel(-1, _explosionSound.get(), 0) == -1)
			{
				printlog("Mix_PlayChannel: %s\n",Mix_GetError());
			}
		}
		else
		{
			auto bomb = std::make_shared<Bomb>(*this);

			if (_nextFrameDueTime < iTimestamp)
			{
				bomb->_frameId++;
				bomb->_frameId %= 3;
				bomb->_nextFrameDueTime = iTimestamp + constants::BOMB_FRAME_UPDATE_DELAY;
			}

			iFutureMap->SetEntity(bomb);
		}
	}

	void Bomb::Render(SDL_Renderer *iRenderer) const 
	{
		if (!_Bomb) 
		{
			InitializeGraphicRessources(iRenderer);
		}

		using namespace bomberman::constants;
		
		SDL_Rect sourceRect;
		sourceRect.w = 16;
		sourceRect.h = 32;
		sourceRect.x = 0 + _frameId * 16;
		sourceRect.y = 0;

		SDL_Rect r;
		r.w = TILE_WIDTH;
		r.h = TILE_HEIGHT;
		r.x = x * TILE_WIDTH + mx * SUBTILE_WIDTH + MAP_X;
		r.y = y * TILE_WIDTH + my * SUBTILE_WIDTH + MAP_Y;

		SDL_RenderCopy(iRenderer, _Bomb.get(), &sourceRect, &r);
	}

	void Bomb::Detonate()
	{
		_detonating = true;
	}
}
}
