#include "bonus.hpp"
#include "constants.hpp"
#include "printlog.hpp"
#include <array>

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

		std::array<double, 3> probabilities;
		probabilities[PROPBOMB] = 0.02;
		probabilities[BOMBCOUNT] = 0.49;
		probabilities[BOMBSTRENGTH] = 0.49;
		
		double number = (double)rand() / (double)RAND_MAX;
		double soFar = 0;
		for (int i = 0;i < probabilities.size(); i++)
		{
			soFar += probabilities[i];
			printlog("number = %lf, soFar = %lf\n", number, soFar);
			if (number < soFar)
			{
				bonus->_bonusType = (BonusType)i;
				break;
			}
		}

		return bonus;
	}

	std::map<Bonus::BonusType, std::shared_ptr<SDL_Texture>> Bonus::_BonusTexture;

	void Bonus::InitializeGraphicRessources(SDL_Renderer *iRenderer) 
	{
		if (_BonusTexture.empty())
		{
			_BonusTexture.insert(std::make_pair(Bonus::PROPBOMB, std::shared_ptr<SDL_Texture>(IMG_LoadTexture(iRenderer, "test/propbombbonus.png"), SDL_DestroyTexture)));
			_BonusTexture.insert(std::make_pair(Bonus::BOMBCOUNT, std::shared_ptr<SDL_Texture>(IMG_LoadTexture(iRenderer, "test/bombbonus.png"), SDL_DestroyTexture)));
			_BonusTexture.insert(std::make_pair(Bonus::BOMBSTRENGTH, std::shared_ptr<SDL_Texture>(IMG_LoadTexture(iRenderer, "test/firebonus.png"), SDL_DestroyTexture)));
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
		InitializeGraphicRessources(iRenderer);

		using namespace bomberman::constants;
		
		SDL_Rect r;
		r.w = TILE_WIDTH;
		r.h = TILE_HEIGHT;
		r.x = x * TILE_WIDTH + MAP_X;
		r.y = y * TILE_WIDTH + MAP_Y;
		
		SDL_RenderCopy(iRenderer, _BonusTexture[_bonusType].get(), nullptr, &r);
	}
}
}
