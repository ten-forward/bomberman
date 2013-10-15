#include "player.hpp"
#include "corpse.hpp"
#include "inputstate.hpp"
#include "bomb.hpp"
#include "propbomb.hpp"
#include "block.hpp"
#include "bonus.hpp"
#include "softblock.hpp"
#include "constants.hpp"
#include "printlog.hpp"
#include "umpire.hpp"

// SDL
#include <SDL.h>
#include <SDL_image.h>
#include <boost/foreach.hpp>

using bomberman::arsenal::Bomb;
using bomberman::arsenal::PropBomb;
using bomberman::architecture::Block;
using bomberman::architecture::SoftBlock;

namespace bomberman {
namespace bestiary {

	namespace {

		inline int sign(int x)	
		{
			return (x > 0) - (x < 0);
		}
		
		bool CanStayAt(const MapConstPtr &iMap, int x, int y)
		{	
			if (iMap->CheckPosition(x, y) != Map::FREE)
			{
				return false;
			}

			return true;
		}
	}

	std::shared_ptr<Mix_Chunk> Player::_bombPlaceSound;

	PlayerPtr Player::Create(const std::string &iName, const std::string &iSpriteName, int iInputStateIdx, SDL_Renderer* iRenderer, bool* alive)
	{
		auto player = std::make_shared<Player>();
		player->_name = iName;
		player->_spriteName = iSpriteName;
		player->zlevel = 2;
		player->_frameId = 3;
		player->_nextFrameDueTime = 0;
		player->_nextUpdateDueTime = 0;
		player->_state = IdleDown;
		player->_inputStateIdx = iInputStateIdx;
		player->InitializeGraphicRessources(iRenderer);
		player->_nbProBomb = 0;
		player->_availableBombs = 1;
		return player;
	}

	void Player::InitializeGraphicRessources(SDL_Renderer *iRenderer) 
	{
		auto surface = IMG_Load(_spriteName.c_str());
		SDL_SetColorKey(surface, SDL_TRUE, 0x00ff00);
		_Bomberman = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(iRenderer, surface), SDL_DestroyTexture);
		SDL_FreeSurface(surface);

		if (!_bombPlaceSound)
		{
			_bombPlaceSound = std::shared_ptr<Mix_Chunk>(Mix_LoadWAV("sound/bombplace.wav"), Mix_FreeChunk);
			if (!_bombPlaceSound)
			{
				printlog("Mix_LoadWAV: %s\n", Mix_GetError());
			}
		}
	}
	
	bool preventDiagonalMovement(std::shared_ptr<Entity> ntt) 
	{
		if (ntt->mx != 0 && ntt->dy != 0)
		{
			ntt->dy = 0;
		}
      
		if (ntt->my != 0 && ntt->dx != 0)
		{
			ntt->dx = 0;
		}
      
		if (ntt->mx == 0 && ntt->my == 0)
		{
			if (ntt->dx && ntt->dy)
			{
				ntt->dy = 0;
				ntt->dx = 0;
				return false;
			}
		}
		return true;
	}

	void Player::EvolutionRoutine(const PlayerPtr thePlayer, const std::vector<InputState>& iInputs, uint32_t iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const
	{
		auto umpire = std::static_pointer_cast<Umpire>(iFutureMap->GetEntity(constants::UMPIRE));
		if (_state == Dying)
		{
			auto corpse = Corpse::Create(this->_Bomberman);
			corpse->x = this->x;
			corpse->y = this->y;
			corpse->mx = this->mx;
			corpse->my = this->my;
			iFutureMap->SetEntity(corpse);
			umpire->NotifyPlayerDied(_inputStateIdx);
			return;
		}
				
		auto player = thePlayer;

		const auto &inputs = iInputs[_inputStateIdx];
		
		if (inputs.GetButtonState(InputState::LEFT))
		{
			player->dx = -constants::PLAYER_SPEED;
			player->_state = WalkingLeft;
		}
		else if (inputs.GetButtonState(InputState::RIGHT))
		{
			player->dx = constants::PLAYER_SPEED;
			player->_state = WalkingRight;
		}
		else if (inputs.GetButtonState(InputState::DOWN))
		{
			player->dy = constants::PLAYER_SPEED;
			player->_state = WalkingDown;
		}
		else if (inputs.GetButtonState(InputState::UP))
		{
			player->dy = -constants::PLAYER_SPEED;
			player->_state = WalkingUp;
		} 
		else if (player->dx == 0 && player->dy == 0)
		{
		  player->_state = DynamicToStaticState(_state);
		}
		
		if (inputs.GetButtonPressed(InputState::A))
		{
			// make sure there isn't already a bomb there
			bool alreadyBombed = false;
			
			BOOST_FOREACH(auto entity, iPresentMap->GetEntities(player->x, player->y))
			{
				if (typeid(*entity) == typeid(Bomb))
				{
					alreadyBombed = true;
					break;
				}
			}

			if (!alreadyBombed && umpire->GetBombCount(_inputStateIdx) < _availableBombs)
			{
				const int kBombTimer = 3000;
				EntityPtr newBomb;
				if(_nbProBomb)
				{
					player->_nbProBomb = _nbProBomb - 1;
					newBomb = PropBomb::Create(iTimestamp + kBombTimer, 2);
				}
				else
				{
					newBomb = Bomb::Create(iTimestamp + kBombTimer, 2, _inputStateIdx);
				}
				umpire->IncrementBombCount(_inputStateIdx);

				newBomb->x = player->x;
				newBomb->y = player->y;

				iFutureMap->SetEntity(newBomb);
				
				if (Mix_PlayChannel(-1, _bombPlaceSound.get(), 0) == -1)
				{
					printlog("Mix_PlayChannel: %s\n",Mix_GetError());
				}
			}
		}

		if (_nextFrameDueTime < iTimestamp)
		{
			player->_frameId++;
			player->_frameId %= 3;
			player->_nextFrameDueTime = iTimestamp + constants::PLAYER_FRAME_UPDATE_DELAY;
		}
		
		if (_nextUpdateDueTime < iTimestamp)
		{
			player->_nextUpdateDueTime = iTimestamp + constants::PLAYER_UPDATE_DELAY;
	       
			int dx = player->dx;
			int dy = player->dy;

			if (preventDiagonalMovement(player))
			{
				if (dx != 0 || dy != 0)
				{
					if (player->mx == 0 && player->my == 0)
					{
						// object moving from flush
						int xprime = player->x + sign(dx);
						int yprime = player->y + sign(dy);

						if (CanStayAt(iPresentMap, xprime, yprime))
						{
							player->mx = sign(dx);
							player->my = sign(dy);
						}
						else
						{
						    player->_state = DynamicToStaticState(_state);
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
				
						if (abs(mxprime) >= constants::AMOUNT_PER_TILE || abs(myprime) >= constants::AMOUNT_PER_TILE)
						{
							// completing transition
							int xprime = player->x + sign(player->mx);
							int yprime = player->y + sign(player->my);
					
							player->x = xprime;
							player->y = yprime;
							player->mx = 0;
							player->my = 0;
							player->dx = 0;
							player->dy = 0;

						}
						else
						{
							// simple move
							player->mx = mxprime;
							player->my = myprime;
						}
					}
				}
			}
		}

		iFutureMap->SetEntity(player);
	}

	void Player::Evolve(const std::vector<InputState>& iInputs, uint32_t iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const
	{
		EvolutionRoutine(std::make_shared<Player>(*this), iInputs, iTimestamp, iPresentMap, iFutureMap);
	}

	void Player::Interact(const std::vector<InputState>& , Uint32 , const EntitySet &iOthers)
	{
		using bomberman::bonus::Bonus;

		BOOST_FOREACH (auto other, iOthers)
		{
			if(typeid(*other) == typeid(Bonus))
			{
				_nbProBomb += 3;
				auto bonus = std::dynamic_pointer_cast<Bonus>(other);
				bonus->NotifyConsumed();
			}
		}
	}

	void Player::Render(SDL_Renderer *iRenderer) const 
	{

		SDL_Rect src[12];
		for (int i = 0; i < 12; ++i)
		{
			src[i].w = 16;
			src[i].h = 32;
			src[i].x = 1 + i * 17;
			src[i].y = 1;
		}

		using namespace bomberman::constants;

		SDL_Rect dst;
		dst.w = PLAYER_WIDTH;
		dst.h = PLAYER_HEIGHT;
		dst.x = x * TILE_WIDTH + mx * SUBTILE_WIDTH + MAP_X;
		dst.y = y * TILE_WIDTH + my * SUBTILE_WIDTH + MAP_Y - (PLAYER_HEIGHT - TILE_HEIGHT);

		int idx = GetFrameIndex();

		SDL_RenderCopy(iRenderer, _Bomberman.get(), &src[idx], &dst);
	}

	void Player::Kill()
	{
		_state = Dying;
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
			default:
				return 3;
		}
		return 3;
	}

	Player::State Player::DynamicToStaticState(Player::State iState) 
	  {
	    switch (iState)
	      {
	      case WalkingDown:
			return IdleDown;
	      case WalkingUp:
			return IdleUp;
	      case WalkingRight:
			return IdleRight;
	      case WalkingLeft:
			return IdleLeft;
	      default:
			;
	      }	 
	    return iState;
	  }
}
}
