#include "explosion.hpp"
#include "block.hpp"
#include "bomb.hpp"
#include "player.hpp"
#include "utils.hpp"

// SDL
#include <SDL_image.h>
#include <boost/foreach.hpp>

namespace bomberman {
namespace arsenal {

	namespace {
		const int kExplosionTimer = 200;

		bool CanPropagate(const MapConstPtr &iMap, int x, int y)
		{	
			using bomberman::architecture::Block;

			if (!iMap->IsPointWithin(x, y))
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

	ExplosionPtr Explosion::Create(int iCreationTime) {
		auto explosion = std::make_shared<Explosion>();
		explosion->_timeout = iCreationTime + kExplosionTimer;
		explosion->_stage = 0;
		explosion->zlevel = 3;
		explosion->_propagation = IsoTropic;
		return explosion;
	}

	std::shared_ptr<SDL_Texture>  Explosion::_Explosion[];

	void Explosion::InitializeGraphicRessources(SDL_Renderer *iRenderer) 
	{
		_Explosion[0] = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(iRenderer, "test/explosion1.png"), SDL_DestroyTexture);
		_Explosion[1] = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(iRenderer, "test/explosion2.png"), SDL_DestroyTexture);
		_Explosion[2] = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(iRenderer, "test/explosion3.png"), SDL_DestroyTexture);
		_Explosion[3] = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(iRenderer, "test/explosion4.png"), SDL_DestroyTexture);
	}

	void Explosion::Evolve(const std::vector<InputState>& /*iInputs*/, Uint32 iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const
	{
		if (iTimestamp < _timeout) 
		{
			auto explosion = std::make_shared<Explosion>(*this);
			explosion->active = true;
		  	iFutureMap->SetEntity(explosion);
		}
		else
		{
		  if (_stage == 0)
		  {
		    Propagate(iTimestamp, iPresentMap, iFutureMap);
		  }

		  if (_stage < 4)
		  {
		    auto explosion = std::make_shared<Explosion>(*this);
		    explosion->active = true;
		    explosion->_timeout = iTimestamp + kExplosionTimer;
		    explosion->_stage++;
		    iFutureMap->SetEntity(explosion);		  
		  }
		}
	}

	void Explosion::Interact(const std::vector<InputState>& iInputs, Uint32 iTimestamp, const EntitySet &iOthers)
	{	
		using bomberman::arsenal::Bomb;
		using bomberman::bestiary::Player;

		BOOST_FOREACH (auto other, iOthers)
		{
			if(typeid(*other) == typeid(Bomb))
			{
				auto bomb = std::dynamic_pointer_cast<Bomb>(other);
				bomb->Detonate();
			}
			else if(typeid(*other) == typeid(Explosion))
			{
				auto explosion = std::dynamic_pointer_cast<Explosion>(other);
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
		}
	}

	void Explosion::Render(SDL_Renderer *iRenderer) const 
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
		r.y = y * TILE_HEIGHT + MAP_Y;
		
		SDL_RenderCopy(iRenderer, _Explosion[_stage].get(), nullptr, &r);
	}

	 void Explosion::Propagate(Uint32 iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const
	 {
	 	switch(_propagation)
	 	{
	 		case IsoTropic:

 				if (CanPropagate(iPresentMap, x + 1, y))
 				{
 					auto explosion = std::make_shared<Explosion>(*this);
 					explosion->active = true;
				    explosion->_timeout = iTimestamp + kExplosionTimer;
				    explosion->x = x + 1;
				    explosion->_propagation = Right;
				    iFutureMap->SetEntity(explosion);	  
 				}

 				if (CanPropagate(iPresentMap, x - 1, y))
 				{
 					auto explosion = std::make_shared<Explosion>(*this);
 					explosion->active = true;
				    explosion->_timeout = iTimestamp + kExplosionTimer;
				    explosion->x = x - 1;
				    explosion->_propagation = Left;
				    iFutureMap->SetEntity(explosion);	  
 				}

				if (CanPropagate(iPresentMap, x, y + 1))
 				{
 					auto explosion = std::make_shared<Explosion>(*this);
 					explosion->active = true;
				    explosion->_timeout = iTimestamp + kExplosionTimer;
				    explosion->y = y + 1;
				    explosion->_propagation = Down;
				    iFutureMap->SetEntity(explosion);	  
 				}

 				if (CanPropagate(iPresentMap, x, y - 1))
 				{
 					auto explosion = std::make_shared<Explosion>(*this);
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
 					auto explosion = std::make_shared<Explosion>(*this);
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
 					auto explosion = std::make_shared<Explosion>(*this);
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
 					auto explosion = std::make_shared<Explosion>(*this);
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
 					auto explosion = std::make_shared<Explosion>(*this);
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
