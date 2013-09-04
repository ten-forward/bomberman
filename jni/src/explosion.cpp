#include "explosion.hpp"

// SDL
#include <SDL_image.h>

namespace bomberman {
namespace arsenal {

	ExplosionPtr Explosion::Create(int iTimeout) {
		auto explosion = std::make_shared<Explosion>();
		explosion->_timeout = iTimeout;
		explosion->_stage = 0;
		explosion->zlevel = 3;
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

	void Explosion::Evolve(const InputState& /*iInputs*/, uint32_t /*iTimestamp*/, const MapConstPtr &/*iPresentMap*/, const MapPtr &iFutureMap) const
	{
		iFutureMap->SetEntity(shared_from_this());
		/*// process explosions.
		BOOST_FOREACH(ExplosionInfo& explosion, explosions)
		{
			if (now > explosion.timeout)
			{
				explosion.timeout = now + EXPLOSIONTIMER;
				explosion.stage++;
			}

			auto ntt = map.GetEntity(explosion.x, explosion.y);
			if (ntt)
			{
				// kill it!
			}
		};	

		// remove expired explosions
		RemoveWhere<ExplosionInfo>(&explosions, [&](ExplosionInfo explosion)->bool
		{
			return explosion.stage > EXPLOSIONSTAGES;
		});*/
	}

	void Explosion::Render(SDL_Renderer *iRenderer) const 
	{
		if (!_Explosion[0]) 
		{
			InitializeGraphicRessources(iRenderer);
		}

		SDL_Rect r;
		r.w = 64;
		r.h = 64;
		r.x = x * r.w + 20;
		r.y = y * r.h + 20;
		
		SDL_RenderCopy(iRenderer, _Explosion[_stage].get(), nullptr, &r);
	}
}
}