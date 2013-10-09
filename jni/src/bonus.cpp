#include "bonus.hpp"
#include "constants.hpp"

// SDL
#include <SDL_image.h>

namespace bomberman {
namespace bonus {

	BonusPtr Bonus::Create() 
	{
		auto bonus = std::make_shared<Bonus>();
		bonus->zlevel = 2;
		bonus->elevel = -1;
		bonus->_state = Available;
		return bonus;
	}

	std::shared_ptr<SDL_Texture> Bonus::_Bonus;
	void Bonus::InitializeGraphicRessources(SDL_Renderer *iRenderer) 
	{
		if (!_Bonus)
		{
			_Bonus = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(iRenderer, "test/bonus.png"), SDL_DestroyTexture);
		}
	}

	void Bonus::Evolve(const std::vector<InputState>& /*iInputs*/, uint32_t /*iTimestamp*/, const MapConstPtr &/*iPresentMap*/, const MapPtr &iFutureMap) const
	{
		if (_state == Available) 
		{
			iFutureMap->SetEntity(std::make_shared<Bonus>(*this));
		}
	}

	void Bonus::Render(SDL_Renderer *iRenderer) const 
	{
		if (!_Bonus)
		{
			InitializeGraphicRessources(iRenderer);
		}

		using namespace bomberman::constants;
		
		SDL_Rect r;
		r.w = TILE_WIDTH;
		r.h = TILE_HEIGHT;
		r.x = x * TILE_WIDTH + MAP_X;
		r.y = y * TILE_WIDTH + MAP_Y;
		
		SDL_RenderCopy(iRenderer, _Bonus.get(), nullptr, &r);
	}
}
}
