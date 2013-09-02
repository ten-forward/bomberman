#include "player.hpp"

// SDL
#include <SDL_image.h>

namespace bomberman {
namespace bestiary {

	PlayerPtr Player::Create(const std::string &iName)
	{
		auto player = std::make_shared<Player>();
		player->_name = iName;
		player->zlevel = 1;
		return player;
	}

	std::shared_ptr<SDL_Texture>  Player::_Bomberman;

	void Player::InitializeGraphicRessources(SDL_Renderer *iRenderer) 
	{
		//bombergirl = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, "test/bombergirl.png"), SDL_DestroyTexture);
		_Bomberman = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(iRenderer, "test/bomberman.png"), SDL_DestroyTexture);
	}

	void Player::Evolve(const InputState& /*iInputs*/, uint32_t /*iTimestamp*/, const MapConstPtr &/*iPresentMap*/, const MapPtr &/*iFutureMap*/) const
	{
		/*if (inputs.GetLeftButtonState())
		{
			player->dx = -1;
		}
		else if (inputs.GetRightButtonState())
		{
			player->dx = 1;
		}
		else if (inputs.GetDownButtonState())
		{
			player->dy = 1;
		}
		else if (inputs.GetUpButtonState())
		{
			player->dy = -1;
		}
		else if (inputs.GetAButtonState())
		{
			// make sure there isn't already a bomb there
			bool alreadyBombed = false;
			for(auto bip : bombs)
			{
				if (player->x == bip.first->x && player->y == bip.first->y)
				{
					alreadyBombed = true;
					break;
				}
			};

			if (!alreadyBombed)
			{
				auto newBomb = Bomb::Create();
				newBomb->x = player->x;
				newBomb->y = player->y;
				newBomb->userdata = BOMBID;

				BombInfo info;
				info.strength = 2;
				info.timeout = now + BOMBTIMER;

				overlappingBombs.push_back(newBomb);
				bombs.push_back(bombInfoPair(newBomb,info));
			}
		}
	}

	// check if player has left the position where a bomb was
	RemoveWhere<EntityPtr>(&overlappingBombs, [&](EntityPtr bomb)->bool
	{
		return presentMap->TrySetEntity(bomb, bomb->x, bomb->y);
	});*/
	}

	void Player::Render(SDL_Renderer *iRenderer) const 
	{
		if (!_Bomberman) 
		{
			InitializeGraphicRessources(iRenderer);
		}

		SDL_Rect r;
		r.w = 64;
		r.h = 64;
		r.x = x * r.w + mx * 8 + 20;
		r.y = y * r.h + my * 8 + 20;

		SDL_RenderCopy(iRenderer, _Bomberman.get(), nullptr, &r);
	}

}
}