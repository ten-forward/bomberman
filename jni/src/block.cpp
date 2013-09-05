#include "block.hpp"

// SDL
#include <SDL_image.h>

namespace bomberman {
namespace architecture {

	BlockPtr Block::Create() 
	{
		auto block = std::make_shared<Block>();
		block->zlevel = 0;
		block->elevel = -1;
		return block;
	}

	std::shared_ptr<SDL_Texture> Block::_Block;

	void Block::InitializeGraphicRessources(SDL_Renderer *iRenderer) 
	{
		_Block = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(iRenderer, "test/block.png"), SDL_DestroyTexture);
	}

	void Block::Evolve(const InputState& /*iInputs*/, uint32_t /*iTimestamp*/, const MapConstPtr &/*iPresentMap*/, const MapPtr &iFutureMap) const
	{
		iFutureMap->SetEntity(std::make_shared<Block>(*this));		
	}

	void Block::Render(SDL_Renderer *iRenderer) const 
	{
		if (!_Block)
		{
			InitializeGraphicRessources(iRenderer);
		}

		SDL_Rect r;
		r.w = 64;
		r.h = 64;
		r.x = x * r.w + 20;	// <- just for overscan
		r.y = y * r.h + 20;
		
		SDL_RenderCopy(iRenderer, _Block.get(), nullptr, &r);
	}
}
}
