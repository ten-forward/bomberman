#include "floortile.hpp"
#include "constants_resolution.hpp"
#include "constants_game.hpp"

// SDL
#include <SDL_image.h>

namespace bomberman {
namespace architecture {

	FloorTilePtr FloorTile::Create() 
	{
		auto floorTile = std::make_shared<FloorTile>();
        floorTile->id = constants::FLOORID;
		floorTile->zlevel = constants::FLOOR_ZLEVEL;
		floorTile->elevel = constants::FLOOR_ELEVEL;
		return floorTile;
	}

	std::shared_ptr<SDL_Texture> FloorTile::_FloorTile;

	void FloorTile::InitializeGraphicRessources(SDL_Renderer *iRenderer) 
	{
		_FloorTile = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(iRenderer, "drawable/floor.png"), SDL_DestroyTexture);
	}

	void FloorTile::Evolve(const std::vector<InputState>& /*iInputs*/, uint32_t /*iTimestamp*/, const MapConstPtr &/*iPresentMap*/, const MapPtr &iFutureMap) const
	{
		iFutureMap->SetEntity(std::make_shared<FloorTile>(*this));
	}

	void FloorTile::Render(SDL_Renderer *iRenderer) const 
	{
		if (!_FloorTile)
		{
			InitializeGraphicRessources(iRenderer);
		}

		using namespace bomberman::constants;
		
		SDL_Rect r;
		r.w = TILE_WIDTH;
		r.h = TILE_HEIGHT;
		r.x = GetX() * TILE_WIDTH + MAP_X;	// <- just for overscan
		r.y = GetY() * TILE_WIDTH + MAP_Y;

		SDL_RenderCopy(iRenderer, _FloorTile.get(), nullptr, &r);
	}
}
}