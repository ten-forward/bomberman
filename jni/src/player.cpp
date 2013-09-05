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
		player->_dying = false;
		player->_frameId = 3;
		player->_nextFrameDueTime =0;
		player->_state = IdleDown;
		return player;
	}

	std::shared_ptr<SDL_Texture>  Player::_Bomberman;

	void Player::InitializeGraphicRessources(SDL_Renderer *iRenderer) 
	{
		auto surface = IMG_Load("test/SaturnBomberman-BlackBomberman.PNG");
		SDL_SetColorKey(surface, SDL_TRUE, 0x00ff00);
		_Bomberman = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(iRenderer, surface), SDL_DestroyTexture);	
	}

	void Player::Evolve(const InputState& iInputs, uint32_t iTimestamp, const MapConstPtr &/*iPresentMap*/, const MapPtr &iFutureMap) const
	{
		if (_dying)
		{
			//Adios amigos
			return;
		}

		const int kAmountPerTile = 8;
		
		auto player = std::make_shared<Player>(*this);
		
		int dx = 0, dy = 0;

		if (iInputs.GetLeftButtonState())
		{
			dx = -1;
			player->_state = WalkingLeft;
		}
		else if (iInputs.GetRightButtonState())
		{
			dx = 1;
			player->_state = WalkingRight;
		}
		else if (iInputs.GetDownButtonState())
		{
			dy = 1;
			player->_state = WalkingDown;
		}
		else if (iInputs.GetUpButtonState())
		{
			dy = -1;
			player->_state = WalkingUp;
		} 
		else
		{
			// To Idle states transitions ...
			switch (_state)
			{
				case WalkingDown:
					player->_state = IdleDown;
					break;
				case WalkingUp:
					player->_state = IdleUp;
					break;
				case WalkingRight:
					player->_state = IdleRight;
					break;
				case WalkingLeft:
					player->_state = IdleLeft;
					break;
				default:
					player->_state = _state;
			}
		}

		if (_nextFrameDueTime < iTimestamp)
		{
			player->_frameId++;
			player->_frameId %= 3;
			player->_nextFrameDueTime = iTimestamp + 150;
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

		SDL_Rect src[12];
		for (int i = 0; i < 12; ++i)
		{
			src[i].w = 16;
			src[i].h = 32;
			src[i].x = 1 + i * 17;
			src[i].y = 1;
		}

		SDL_Rect dst;
		dst.w = 64;
		dst.h = 64;
		dst.x = x * dst.w + mx * 8 + 20;
		dst.y = y * dst.h + my * 8 + 20;

		int idx = GetFrameIndex();

		SDL_RenderCopy(iRenderer, _Bomberman.get(), &src[idx], &dst);
	}

	void Player::Kill()
	{
		_dying = true;
	}

	int Player::GetFrameIndex() const
	{
		switch(_state)
		{
			case WalkingUp:
				return _frameId;
			case WalkingDown:
				return 6 + _frameId;
			case WalkingLeft:
				return 3 + _frameId;
			case WalkingRight:
				return 9 + _frameId;
			case IdleUp:
				return 0 + 2;
			case IdleDown:
				return 6 + 2;
			case IdleLeft:
				return 3 + 2;
			case IdleRight:
				return 9 + 2;
		}
	}
}
}