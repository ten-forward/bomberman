#include "bomb.hpp"
#include "explosion.hpp"
#include "constants.hpp"
#include "softblock.hpp"
#include "block.hpp"

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

	BombPtr Bomb::Create(int iTimeout, int iStrength) 
	{
		auto bomb = std::make_shared<Bomb>();
		bomb->_timeout = iTimeout;
		bomb->_strength = iStrength;
		bomb->zlevel = 2;
		bomb->_detonating = false;
		bomb->_frameId = 0;
		bomb->_nextFrameDueTime = 0;
		return bomb;
	}

	std::shared_ptr<SDL_Texture>  Bomb::_Bomb;

	void Bomb::InitializeGraphicRessources(SDL_Renderer *iRenderer) 
	{
		_Bomb = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(iRenderer, "test/bomb.png"), SDL_DestroyTexture);
	}

	void Bomb::Evolve(const std::vector<InputState>& /*iInputs*/, uint32_t iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const
	{
		if (iTimestamp >= _timeout || _detonating) 
		{
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
