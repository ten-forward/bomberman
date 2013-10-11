#include "corpse.hpp"
#include "inputstate.hpp"
#include "bomb.hpp"
#include "block.hpp"
#include "softblock.hpp"
#include "constants.hpp"

// SDL
#include <SDL.h>
#include <SDL_image.h>
#include <boost/foreach.hpp>

using bomberman::arsenal::Bomb;
using bomberman::architecture::Block;
using bomberman::architecture::SoftBlock;

namespace bomberman {
namespace bestiary {


	CorpsePtr Corpse::Create(std::shared_ptr<SDL_Texture> corpseSprite)
	{
		auto corpse = std::make_shared<Corpse>();
		corpse->_frameId = 0;
		corpse->_BombermanCorpse = corpseSprite;

		corpse->zlevel = 3;
		corpse->_frameId = 0;
		corpse->_nextFrameDueTime = 0;
		corpse->_nextUpdateDueTime = 0;

		return corpse;
	}
	
	void Corpse::Evolve(const std::vector<InputState>& iInputs, uint32_t iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const
	{
		auto player = std::make_shared<Corpse>(*this);

		if (_nextFrameDueTime < iTimestamp)
		{
			player->_frameId++;
			player->_nextFrameDueTime = iTimestamp + constants::PLAYER_FRAME_UPDATE_DELAY;
		}

		if (player->_frameId > 9)
		{
			//Adios amigos
			return;
		}

		iFutureMap->SetEntity(player);
	}

	void Corpse::Render(SDL_Renderer *iRenderer) const 
	{

		SDL_Rect src[9];
		int i=0;
		for (; i < 4; ++i)
		{
			src[i].w = 16;
			src[i].h = 32;
			src[i].x = 290 - i * 17;
			src[i].y = 100;
		}

		for (; i < 9; ++i)
		{
			src[i].w = 24;
			src[i].h = 32;
			src[i].x = 214 - (i-4) * 25;
			src[i].y = 100;
		}

		using namespace bomberman::constants;

		SDL_Rect dst;
		dst.w = src[_frameId].w * SPRITE_MAGNIFICATION;
		dst.h = src[_frameId].h * SPRITE_MAGNIFICATION;
		dst.x = x * TILE_WIDTH + mx * SUBTILE_WIDTH + MAP_X - (src[_frameId].w * SPRITE_MAGNIFICATION - TILE_WIDTH) / 2;
		dst.y = y * TILE_WIDTH + my * SUBTILE_WIDTH + MAP_Y - (PLAYER_HEIGHT - TILE_HEIGHT);

		SDL_RenderCopy(iRenderer, _BombermanCorpse.get(), &src[_frameId], &dst);
	}

}
}