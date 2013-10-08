#include "softblock.hpp"
#include "constants.hpp"
#include "bonus.hpp"

// SDL
#include <SDL_image.h>

using bomberman::bonus::Bonus;

namespace bomberman {
namespace architecture {

	SoftBlockPtr SoftBlock::Create(double iBonusProbability) 
	{
		auto block = std::make_shared<SoftBlock>();
		block->zlevel = 2;
		block->elevel = -1;
		block->isAlive = true;
		block->_bonusProbability = iBonusProbability;
		return block;
	}

	std::shared_ptr<SDL_Texture> SoftBlock::_SoftBlock;

	void SoftBlock::InitializeGraphicRessources(SDL_Renderer *iRenderer) 
	{
		_SoftBlock = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(iRenderer, "test/softblock.png"), SDL_DestroyTexture);
	}

	void SoftBlock::Evolve(const std::vector<InputState>& /*iInputs*/, uint32_t /*iTimestamp*/, const MapConstPtr &/*iPresentMap*/, const MapPtr &iFutureMap) const
	{
		if (!isAlive)
		{
			if (random() < RAND_MAX * _bonusProbability) 
			{
				iFutureMap->SetEntity(Bonus::Create());
			}
			return;
		}

		iFutureMap->SetEntity(std::make_shared<SoftBlock>(*this));
	}

	void SoftBlock::Render(SDL_Renderer *iRenderer) const 
	{
		if (!_SoftBlock)
		{
			InitializeGraphicRessources(iRenderer);
		}

		using namespace bomberman::constants;
		
		SDL_Rect r;
		r.w = TILE_WIDTH;
		r.h = TILE_HEIGHT;
		r.x = x * TILE_WIDTH + MAP_X;	// <- just for overscan
		r.y = y * TILE_WIDTH + MAP_Y;
		
		SDL_RenderCopy(iRenderer, _SoftBlock.get(), nullptr, &r);
	}

	void SoftBlock::Kill()
	{
		isAlive = false;
	}
}
}
