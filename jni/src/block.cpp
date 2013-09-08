#include "block.hpp"
#include "constants.hpp"

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

	void Block::Evolve(const std::vector<InputState>& /*iInputs*/, uint32_t /*iTimestamp*/, const MapConstPtr &/*iPresentMap*/, const MapPtr &iFutureMap) const
	{
		iFutureMap->SetEntity(std::make_shared<Block>(*this));		
	}

	void Block::Render(SDL_Renderer *iRenderer) const 
	{
		if (!_Block)
		{
			InitializeGraphicRessources(iRenderer);
		}

		using namespace bomberman::constants;
		
		SDL_Rect r;
		r.w = TILE_WIDTH;
		r.h = TILE_HEIGHT;
		r.x = x * TILE_WIDTH + MAP_X;	// <- just for overscan
		r.y = y * TILE_HEIGHT + MAP_Y;
		
		SDL_RenderCopy(iRenderer, _Block.get(), nullptr, &r);
	}
}
}
