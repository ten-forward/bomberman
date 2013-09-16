#include "propexplosion.hpp"
#include "block.hpp"
#include "softblock.hpp"
#include "bomb.hpp"
#include "propbomb.hpp"
#include "player.hpp"
#include "constants.hpp"

// SDL
#include <SDL_image.h>
#include <boost/foreach.hpp>

using bomberman::architecture::Block;
using bomberman::architecture::SoftBlock;

namespace bomberman {
namespace arsenal {

	namespace {
		const int kExplosionTimer = 200;

		bool CanPropagate(const MapConstPtr &iMap, int x, int y)
		{
			if (iMap->CheckPosition(x, y) != Map::BOUNDARY)
			{
				return false;
			}

			BOOST_FOREACH (auto entity, iMap->GetEntities(x, y)) 
			{
				if (typeid(*entity) == typeid(Block))
				{
					return false;
				}
			}

			return true;
		}
	}

	PropExplosionPtr PropExplosion::Create(int iCreationTime) {
		auto explosion = std::make_shared<PropExplosion>();
		explosion->_timeout = iCreationTime + kExplosionTimer;
		explosion->_stage = 0;
		explosion->zlevel = 2;
		explosion->_propagation = IsoTropic;
		explosion->_willPropagate = true;
		return explosion;
	}

	std::shared_ptr<SDL_Texture> PropExplosion::_Explosion[];

	void PropExplosion::InitializeGraphicRessources(SDL_Renderer *iRenderer) 
	{
		_Explosion[0] = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(iRenderer, "test/explosion1.png"), SDL_DestroyTexture);
		_Explosion[1] = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(iRenderer, "test/explosion2.png"), SDL_DestroyTexture);
		_Explosion[2] = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(iRenderer, "test/explosion3.png"), SDL_DestroyTexture);
		_Explosion[3] = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(iRenderer, "test/explosion4.png"), SDL_DestroyTexture);
	}

	void PropExplosion::Evolve(const std::vector<InputState>& /*iInputs*/, Uint32 iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const
	{
		if (iTimestamp < _timeout) 
		{
			auto explosion = std::make_shared<PropExplosion>(*this);
			explosion->active = true;
		  	iFutureMap->SetEntity(explosion);
		}
		else
		{
			if (_stage == 0)
			{
				if (_willPropagate)
				{
					Propagate(iTimestamp, iPresentMap, iFutureMap);
				}
			}
			

			if (_stage < 4)
			{
				auto explosion = std::make_shared<PropExplosion>(*this);
				explosion->active = true;
				explosion->_timeout = iTimestamp + kExplosionTimer;
				explosion->_stage++;
				iFutureMap->SetEntity(explosion);
			}
		}
	}

	void PropExplosion::Interact(const std::vector<InputState>& iInputs, Uint32 iTimestamp, const EntitySet &iOthers)
	{
		using bomberman::arsenal::Bomb;
		using bomberman::arsenal::PropBomb;
		using bomberman::architecture::SoftBlock;
		using bomberman::bestiary::Player;

		BOOST_FOREACH (auto other, iOthers)
		{
			if(typeid(*other) == typeid(PropBomb))
			{
				auto bomb = std::dynamic_pointer_cast<PropBomb>(other);
				bomb->Detonate();
			}
			else if(typeid(*other) == typeid(Bomb))
			{
				auto bomb = std::dynamic_pointer_cast<Bomb>(other);
				bomb->Detonate();
			}
			else if(typeid(*other) == typeid(PropExplosion))
			{
				auto explosion = std::dynamic_pointer_cast<PropExplosion>(other);
				if (explosion->_stage > _stage)
				{
					explosion->active = false;
				}
			}
			else if(typeid(*other) == typeid(Player))
			{
				auto player = std::dynamic_pointer_cast<Player>(other);
				player->Kill();
			}
			else if (typeid(*other) == typeid(SoftBlock))
			{
				auto softblock = std::dynamic_pointer_cast<SoftBlock>(other);
				_willPropagate = false;
				softblock->Kill();
			}
		}
	}

	void PropExplosion::Render(SDL_Renderer *iRenderer) const 
	{
		if (!_Explosion[0]) 
		{
			InitializeGraphicRessources(iRenderer);
		}

		using namespace bomberman::constants;
		
		SDL_Rect r;
		r.w = TILE_WIDTH;
		r.h = TILE_HEIGHT;
		r.x = x * TILE_WIDTH + MAP_X;
		r.y = y * TILE_WIDTH + MAP_Y;
		
		SDL_RenderCopy(iRenderer, _Explosion[_stage].get(), nullptr, &r);
	}

	 void PropExplosion::Propagate(Uint32 iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const
	 {
	 	switch(_propagation)
	 	{
	 		case IsoTropic:

 				if (CanPropagate(iPresentMap, x + 1, y))
 				{
 					auto explosion = std::make_shared<PropExplosion>(*this);
 					explosion->active = true;
				    explosion->_timeout = iTimestamp + kExplosionTimer;
				    explosion->x = x + 1;
				    explosion->_propagation = Right;
				    iFutureMap->SetEntity(explosion);	  
 				}

 				if (CanPropagate(iPresentMap, x - 1, y))
 				{
 					auto explosion = std::make_shared<PropExplosion>(*this);
 					explosion->active = true;
				    explosion->_timeout = iTimestamp + kExplosionTimer;
				    explosion->x = x - 1;
				    explosion->_propagation = Left;
				    iFutureMap->SetEntity(explosion);	  
 				}

				if (CanPropagate(iPresentMap, x, y + 1))
 				{
 					auto explosion = std::make_shared<PropExplosion>(*this);
 					explosion->active = true;
				    explosion->_timeout = iTimestamp + kExplosionTimer;
				    explosion->y = y + 1;
				    explosion->_propagation = Down;
				    iFutureMap->SetEntity(explosion);	  
 				}

 				if (CanPropagate(iPresentMap, x, y - 1))
 				{
 					auto explosion = std::make_shared<PropExplosion>(*this);
 					explosion->active = true;
				    explosion->_timeout = iTimestamp + kExplosionTimer;
				    explosion->y = y - 1;
				    explosion->_propagation = Up;
				    iFutureMap->SetEntity(explosion);	  
 				}

	 			return;

	 		case Up:

	 			if (CanPropagate(iPresentMap, x, y - 1))
 				{
 					auto explosion = std::make_shared<PropExplosion>(*this);
 					explosion->active = true;
				    explosion->_timeout = iTimestamp + kExplosionTimer;
				    explosion->y = y - 1;
				    explosion->_propagation = Up;
				    iFutureMap->SetEntity(explosion);	  
 				}

 				return;

	 		case Down:

	 			if (CanPropagate(iPresentMap, x, y + 1))
 				{
 					auto explosion = std::make_shared<PropExplosion>(*this);
 					explosion->active = true;
				    explosion->_timeout = iTimestamp + kExplosionTimer;
				    explosion->y = y + 1;
				    explosion->_propagation = Down;
				    iFutureMap->SetEntity(explosion);	  
 				}

 				return;

	 		case Left:

				if (CanPropagate(iPresentMap, x - 1, y))
 				{
 					auto explosion = std::make_shared<PropExplosion>(*this);
 					explosion->active = true;
				    explosion->_timeout = iTimestamp + kExplosionTimer;
				    explosion->x = x - 1;
				    explosion->_propagation = Left;
				    iFutureMap->SetEntity(explosion);	  
 				}

				return;

	 		case Right:

	 			if (CanPropagate(iPresentMap, x + 1, y))
 				{
 					auto explosion = std::make_shared<PropExplosion>(*this);
 					explosion->active = true;
				    explosion->_timeout = iTimestamp + kExplosionTimer;
				    explosion->x = x + 1;
				    explosion->_propagation = Right;
				    iFutureMap->SetEntity(explosion);	  
 				}

 				return;
	 	}
	 }
}
}
