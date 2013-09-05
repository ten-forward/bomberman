#include "floortile.hpp"

// SDL
#include <SDL_image.h>

namespace bomberman {
namespace architecture {

	FloorTilePtr FloorTile::Create() 
	{
		auto floorTile = std::make_shared<FloorTile>();
		floorTile->zlevel = 0;
		floorTile->elevel = -1;
		return floorTile;
	}

	std::shared_ptr<SDL_Texture> FloorTile::_FloorTile;

	void FloorTile::InitializeGraphicRessources(SDL_Renderer *iRenderer) 
	{
		_FloorTile = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(iRenderer, "test/floor.png"), SDL_DestroyTexture);
	}

	void FloorTile::Evolve(const InputState& /*iInputs*/, uint32_t /*iTimestamp*/, const MapConstPtr &/*iPresentMap*/, const MapPtr &iFutureMap) const
	{
		iFutureMap->SetEntity(std::make_shared<FloorTile>(*this));
	}

	void FloorTile::Render(SDL_Renderer *iRenderer) const 
	{
		if (!_FloorTile)
		{
			InitializeGraphicRessources(iRenderer);
		}

		SDL_Rect r;
		r.w = 64;
		r.h = 64;
		r.x = x * r.w + 20;	// <- just for overscan
		r.y = y * r.h + 20;

		SDL_RenderCopy(iRenderer, _FloorTile.get(), nullptr, &r);
	}
}
}