#include "player.hpp"
#include "inputstate.hpp"
#include "bomb.hpp"
#include "block.hpp"

// SDL
#include <SDL_image.h>

using bomberman::arsenal::Bomb;
using bomberman::architecture::Block;

namespace bomberman {
namespace bestiary {

	namespace {

		inline int sign(int x)	
		{
			return (x > 0) - (x < 0);
		}
		
		bool CanStayAt(const MapPtr &iMap, int x, int y)
		{	
			if (!iMap->IsPointWithin(x, y))
			{
				return false;
			}

			for (auto entity : iMap->GetEntities(x, y)) 
			{
				if (typeid(*entity) == typeid(Block) ||
					typeid(*entity) == typeid(Player))

				{
					return false;
				}
			}

			return true;
		}
	}

	PlayerPtr Player::Create(const std::string &iName)
	{
		auto player = std::make_shared<Player>();
		player->_name = iName;
		player->zlevel = 2;
		return player;
	}

	std::shared_ptr<SDL_Texture>  Player::_Bomberman;

	void Player::InitializeGraphicRessources(SDL_Renderer *iRenderer) 
	{
		//bombergirl = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, "test/bombergirl.png"), SDL_DestroyTexture);
		_Bomberman = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(iRenderer, "test/bomberman.png"), SDL_DestroyTexture);
	}

	void Player::Evolve(const InputState& iInputs, uint32_t iTimestamp, const MapConstPtr &/*iPresentMap*/, const MapPtr &iFutureMap) const
	{
		const int kAmountPerTile = 8;
		
		auto player = std::make_shared<Player>(*this);

		int dx = 0, dy = 0;

		if (iInputs.GetLeftButtonState())
		{
			dx = -1;
		}
		else if (iInputs.GetRightButtonState())
		{
			dx = 1;
		}
		else if (iInputs.GetDownButtonState())
		{
			dy = 1;
		}
		else if (iInputs.GetUpButtonState())
		{
			dy = -1;
		}

		if (iInputs.GetAButtonState())
		{
			// make sure there isn't already a bomb there
			bool alreadyBombed = false;
			
			for(auto entity : iFutureMap->GetEntities(player->x, player->y))
			{
				if (typeid(*entity) == typeid(Bomb))
				{
					alreadyBombed = true;
					break;
				}
			}

			if (!alreadyBombed)
			{
				const int kBombTimer = 3000;
				auto newBomb = Bomb::Create(iTimestamp + kBombTimer, 2);
				newBomb->x = player->x;
				newBomb->y = player->y;

				iFutureMap->SetEntity(newBomb);
			}
		}

		// check if player has left the position where a bomb was
		//RemoveWhere<EntityPtr>(&overlappingBombs, [&](EntityPtr bomb)->bool
		//{
		//	return presentMap->TrySetEntity(bomb, bomb->x, bomb->y);
		//});

		if (dx != 0 || dy != 0)
		{
			if (player->mx == 0 && player->my == 0)
			{
				// object moving from flush
				int xprime = player->x + sign(dx);
				int yprime = player->y + sign(dy);

				if (CanStayAt(iFutureMap, xprime, yprime))
				{
					player->mx = sign(dx);
					player->my = sign(dy);
				}
			}
			else
			{
				// calculate where we will be
				int mxprime = 0, myprime = 0;

				if (player->mx) 
				{
					mxprime = player->mx + sign(dx);
				}
				else if (player->my)
				{	
					myprime = player->my + sign(dy);
				}
				
				if (abs(mxprime) >= kAmountPerTile || abs(myprime) >= kAmountPerTile)
				{
					// completing transition
					int xprime = player->x + sign(player->mx);
					int yprime = player->y + sign(player->my);
					
					player->x = xprime;
					player->y = yprime;
					player->mx = 0;
					player->my = 0;
				}
				else
				{
					// simple move
					player->mx = mxprime;
					player->my = myprime;
				}

			}
		}

		iFutureMap->SetEntity(player);
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