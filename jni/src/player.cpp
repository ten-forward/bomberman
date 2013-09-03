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
				if (/*typeid(*entity) == typeid(Block) || */
					typeid(*entity) == typeid(Player))

				{
					return false;
				}
			}

			return true;
		}

		/*#define PREVENT_DIAGONAL_MOVEMENT \
			if (ntt->mx != 0 && ntt->dy != 0)\
			{\
				ntt->dy = 0;\
			}\
			\
			if (ntt->my != 0 && ntt->dx != 0)\
			{\
				ntt->dx = 0;\
			}\
			\
			if (ntt->mx == 0 && ntt->my == 0)\
			{\
				if (ntt->dx && ntt->dy)\
				{\
					ntt->dy = 0;\
					ntt->dx = 0;\
					continue;\
				}\
			}*/
	}

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

	void Player::Evolve(const InputState& iInputs, uint32_t iTimestamp, const MapConstPtr &/*iPresentMap*/, const MapPtr &iFutureMap) const
	{

		auto player = std::make_shared<Player>(*this);

		//PREVENT_DIAGONAL_MOVEMENT
		player->dx = player->dy = 0;

		if (iInputs.GetLeftButtonState())
		{
			player->dx = -1;
		}
		else if (iInputs.GetRightButtonState())
		{
			player->dx = 1;
		}
		else if (iInputs.GetDownButtonState())
		{
			player->dy = 1;
		}
		else if (iInputs.GetUpButtonState())
		{
			player->dy = -1;
		}
		else if (iInputs.GetAButtonState())
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

		int dx = player->dx;
		int dy = player->dy;

		if (dx != 0 || dy != 0)
		{
			if (player->mx == 0 && player->my == 0)
			{
				// object moving from flush
				int xprime = player->x + sign(dx);
				int yprime = player->y + sign(dy);

				if (CanStayAt(iFutureMap, xprime, yprime))
				{
					int mxprime = player->mx + sign(dx);
					int myprime = player->my + sign(dy);

					// occupy 2 tiles at once.
					//map[xprime][yprime] = player;
					player->x = xprime;
					player->y = yprime;
					player->mx = mxprime;
					player->my = myprime;
				}
				else
				{
					// cannot move!
					player->dx = 0;
					player->dy = 0;
				}
			}
			else
			{
				// check if we just reversed!
				// if we did we need to fix
				// the values to avoid
				// leaving block trails on the map
				if (sign(dx) != sign(player->mx))
				{
					player->x += sign(player->mx);
					player->mx = -sign(player->mx) * (amountPerTile - abs(player->mx));
				}

				if (sign(dy) != sign(player->my))
				{
					player->y += sign(player->my);
					player->my = -sign(player->my) * (amountPerTile - abs(player->my));
				}

				// calculate where we will be
				int mxprime = player->mx + sign(dx);
				int myprime = player->my + sign(dy);

				if (abs(mxprime) >= amountPerTile || abs(myprime) >= amountPerTile)
				{
					// completing transition
					int xprime = player->x + sign(player->mx);
					int yprime = player->y + sign(player->my);
					
					player->x = xprime;
					player->y = yprime;
					player->mx = 0;
					player->my = 0;

					if (player->brakes)
					{
						player->dx = 0;
						player->dy = 0;
					}
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