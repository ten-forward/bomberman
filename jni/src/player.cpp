#include "player.hpp"
#include "corpse.hpp"
#include "inputstate.hpp"
#include "bomb.hpp"
#include "propbomb.hpp"
#include "block.hpp"
#include "softblock.hpp"
#include "constants_resolution.hpp"
#include "constants_game.hpp"
#include "umpire.hpp"
#include "printlog.hpp"
#include "floortile.hpp"
	
// SDL
#include <SDL.h>
#include <SDL_image.h>
#include <boost/foreach.hpp>

using bomberman::arsenal::Bomb;
using bomberman::arsenal::PropBomb;
using bomberman::architecture::Block;
using bomberman::architecture::SoftBlock;
using bomberman::bonus::Bonus;

namespace bomberman {
namespace bestiary {

	namespace {

		inline int sign(int x)	
		{
			return (x > 0) - (x < 0);
		}
	}

	std::shared_ptr<Mix_Chunk> Player::_bombPlaceSound;

	PlayerPtr Player::Create(PlayerId id, const std::string &iName, const std::string &iSpriteName, int iInputStateIdx, SDL_Renderer* iRenderer)
	{
		auto player = std::make_shared<Player>();
		player->id = id;
		player->_name = iName;
		player->_spriteName = iSpriteName;
		player->zlevel = constants::PLAYER_ZLEVEL;
		player->_frameId = 3;
		player->_nextFrameDueTime = 0;
		player->_nextUpdateDueTime = 0;
		player->_state = IdleDown;
		player->_inputStateIdx = iInputStateIdx;
		player->InitializeGraphicRessources(iRenderer);
		player->_nbProBomb = 0;
		player->_availableBombs = 1;
		player->_bombStrength = 2;
        player->_bombPosX = -1000;
        player->_bombPosY = -1000;
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
	
	void Player::EvolutionRoutine(const PlayerPtr &player, const std::vector<InputState>& iInputs, uint32_t iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const
	{
		auto umpire = std::static_pointer_cast<Umpire>(iFutureMap->GetEntity(constants::UMPIRE));
		if (_state == Dying)
		{
			auto corpse = Corpse::Create(this->_Bomberman);
			corpse->SetX(GetX());
			corpse->SetY(GetY());
			corpse->mx = this->mx;
			corpse->my = this->my;
			iFutureMap->SetEntity(corpse);
			umpire->NotifyPlayerDied(id);
			return;
		}

		const auto &inputs = iInputs[_inputStateIdx];
		
		if (inputs.GetButtonState(InputState::LEFT))
		{
            player->dx = -constants::PLAYER_SPEED;
		}
		else if (inputs.GetButtonState(InputState::RIGHT))
		{
            player->dx = constants::PLAYER_SPEED;
		}
        
		if (inputs.GetButtonState(InputState::DOWN))
		{
            player->dy = constants::PLAYER_SPEED;
		}
		else if (inputs.GetButtonState(InputState::UP))
		{
            player->dy = -constants::PLAYER_SPEED;
		}
        
        if (player->dx == 0 && player->dy == 0)
		{
			player->_state = DynamicToStaticState(_state);
		}

		if (inputs.GetButtonPressed(InputState::A))
		{

		}
		
		if (inputs.GetButtonPressed(InputState::X))
		{
			// make sure there isn't already a bomb there
			bool alreadyBombed = false;
			
			BOOST_FOREACH(auto entity, iPresentMap->GetEntities(player->GetX(), player->GetY()))
			{
				if (typeid(*entity) == typeid(Bomb))
				{
					alreadyBombed = true;
					break;
				}
			}
            
            bool climbedOff = false;
            // make sure we already climbed off another bomb
            if (
                player->mx >= player->_bombPosX + constants::AMOUNT_PER_TILE ||
                player->mx + constants::AMOUNT_PER_TILE <= player->_bombPosX ||
                player->my >= player->_bombPosY + constants::AMOUNT_PER_TILE ||
                player->my + constants::AMOUNT_PER_TILE <= player->_bombPosY
                )
            {
                climbedOff = true;
            }

			if (!alreadyBombed && climbedOff && umpire->GetBombCount(id) < _availableBombs)
			{
				const int kBombTimer = 3000;
				EntityPtr newBomb;
				if(_nbProBomb)
				{
					player->_nbProBomb = _nbProBomb - 1;
					newBomb = PropBomb::Create(iTimestamp + kBombTimer, _bombStrength, id);
				}
				else
				{
					newBomb = Bomb::Create(iTimestamp + kBombTimer, _bombStrength, id);
				}
				umpire->IncrementBombCount(id);

				newBomb->SetX(player->GetX());
				newBomb->SetY(player->GetY());
                
                player->_bombPosX = newBomb->mx;
                player->_bombPosY = newBomb->my;
                
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
			player->_frameId %= 4;
			player->_nextFrameDueTime = iTimestamp + constants::PLAYER_FRAME_UPDATE_DELAY;
		}
		
		if (_nextUpdateDueTime < iTimestamp && (player->dx || player->dy))
		{
			player->_nextUpdateDueTime = iTimestamp + constants::PLAYER_UPDATE_DELAY;
            
            int nx = player->mx + player->dx;
            int ny = player->my + player->dy;
            
            if (CanMoveTo(nx, player->my, iPresentMap, player))
            {
                player->mx = nx;
            }
            
            if (CanMoveTo(player->mx, ny, iPresentMap, player))
            {
                player->my = ny;
            }

            // stop if flush
            if (player->mx % constants::AMOUNT_PER_TILE == 0)
            {
                player->dx = 0;
            }
            
            if (player->my % constants::AMOUNT_PER_TILE == 0)
            {
                player->dy = 0;
            }
            
            // reset bomb position (climbed off a bomb)
            if (
                player->mx >= player->_bombPosX + constants::AMOUNT_PER_TILE ||
                player->mx + constants::AMOUNT_PER_TILE <= player->_bombPosX ||
                player->my >= player->_bombPosY + constants::AMOUNT_PER_TILE ||
                player->my + constants::AMOUNT_PER_TILE <= player->_bombPosY
                )
            {
                player->_bombPosX = -1000;
                player->_bombPosY = -1000;
            }
            
            if (player->dx < 0) { player->_state = WalkingLeft; }
            if (player->dx > 0) { player->_state = WalkingRight; }
            if (player->dy < 0) { player->_state = WalkingUp; }
            if (player->dy > 0) { player->_state = WalkingDown; }
        }

		iFutureMap->SetEntity(player);
	}

    bool Player::CanMoveTo(int mx, int my, const MapConstPtr &iPresentMap, PlayerPtr player) const
    {
        int w = 1;
        int h = 1;
        if ((mx + 8) % constants::AMOUNT_PER_TILE)
        {
            w = 2;
        }
        if ((my + 8) % constants::AMOUNT_PER_TILE)
        {
            h = 2;
        }
        
        int x = (mx+8) / constants::AMOUNT_PER_TILE - 1;
        int y = (my+8) / constants::AMOUNT_PER_TILE - 1;
        
        for (int xpos = 0; xpos < w; xpos++)
        {
            for (int ypos = 0; ypos < h; ypos++)
            {
                int xx = xpos + x;
                int yy = ypos + y;
                
                if (xx < 0 || xx >= iPresentMap->GetWidth() || yy < 0 || yy >= iPresentMap->GetHeight())
                {
                    return false;
                }
                
                BOOST_FOREACH(auto ntt, iPresentMap->GetEntities(xx, yy))
                {
                    if (
                        typeid(*ntt) != typeid(architecture::FloorTile) &&
                        typeid(*ntt) != typeid(Umpire) &&
                        typeid(*ntt) != typeid(bomberman::bonus::Bonus) &&
                        ntt->id != player->id
                        )
                    {
                        // ignore the bomb we are on
                        if (typeid(*ntt) == typeid(arsenal::PropBomb) ||
                            typeid(*ntt) == typeid(arsenal::Bomb))
                        {
                            if (ntt->mx == player->_bombPosX &&
                                ntt->my == player->_bombPosY)
                            {
                                continue;
                            }
                        }
                        
                        // collision
                        if (
                            player->mx >= ntt->mx + constants::AMOUNT_PER_TILE &&
                            player->mx + constants::AMOUNT_PER_TILE <= ntt->mx &&
                            player->my >= ntt->my + constants::AMOUNT_PER_TILE &&
                            player->my + constants::AMOUNT_PER_TILE <= ntt->my
                            )
                        {}
                        else
                        {
                            return false;
                        }
                    }
                }
            }
        }
        
        return true;
    }
    
	void Player::Evolve(const std::vector<InputState>& iInputs, uint32_t iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const
	{
		EvolutionRoutine(std::make_shared<Player>(*this), iInputs, iTimestamp, iPresentMap, iFutureMap);
	}

	void Player::Interact(const std::vector<InputState>& , Uint32 , const EntitySet &iOthers)
	{
		BOOST_FOREACH (auto other, iOthers)
		{
			if(typeid(*other) == typeid(Bonus))
			{
				auto bonus = std::dynamic_pointer_cast<Bonus>(other);
				ConsumeBonus(bonus);
			}
		}
	}

	void Player::Render(SDL_Renderer *iRenderer) const 
	{
		using namespace bomberman::constants;

		SDL_Rect dst;
		dst.w = PLAYER_WIDTH;
		dst.h = PLAYER_HEIGHT;
		dst.x = mx * SUBTILE_WIDTH + MAP_X;
		dst.y = my * SUBTILE_WIDTH + MAP_Y - (PLAYER_HEIGHT - TILE_HEIGHT);

		Render(iRenderer, dst);
	}

	void Player::Render(SDL_Renderer *iRenderer, SDL_Rect &dst) const 
	{
		int idx = GetFrameIndex();

		SDL_Rect src;
		src.w = 16;
		src.h = 32;
		src.x = _state > 3 ? 0 : _frameId * 16;
		src.y = idx * 32;

		SDL_RenderCopy(iRenderer, _Bomberman.get(), &src, &dst);
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
				return 2;
			case WalkingDown:
				return 0;
			case WalkingLeft:
				return 3;
			case WalkingRight:
				return 1;
			case IdleUp:
				return 2;
			case IdleDown:
				return 0;
			case IdleLeft:
				return 3;
			case IdleRight:
				return 1;
		}
		return 0;
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

	  void Player::ConsumeBonus(const bonus::BonusPtr &iBonus)
	  {
	  	using bomberman::bonus::Bonus;
		switch(iBonus->GetType())
		{
		case Bonus::BOMBSTRENGTH:
			_bombStrength++;
			break;

		case Bonus::BOMBCOUNT:
			_availableBombs++;
			break;

		case Bonus::PROPBOMB:
			_nbProBomb++;
			break;
                
        case Bonus::REMOTEBOMB:
            _nbRemoteBomb++;
            break;
		}
		iBonus->NotifyConsumed();
	  }
}
}
