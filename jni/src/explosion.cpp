#include "explosion.hpp"

// SDL
#include <SDL_image.h>

namespace bomberman {
namespace arsenal {

	ExplosionPtr Explosion::Create(int iTimeout) {
		auto explosion = std::make_shared<Explosion>();
		explosion->_timeout = iTimeout;
		explosion->_stage = 0;
		explosion->zlevel = 2;
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