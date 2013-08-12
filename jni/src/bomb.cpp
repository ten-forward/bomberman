#include "bomb.hpp"

// SDL
#include <SDL_image.h>

namespace bomberman {
namespace arsenal {

	BombPtr Bomb::Create(int iTimeout, int iStrength) {
		auto bomb = std::make_shared<Bomb>();
		bomb->_timeout = iTimeout;
		bomb->_strength = iStrength;
		bomb->zlevel = 1;
		return bomb;
	}

	std::shared_ptr<SDL_Texture>  Bomb::_Bomb;

	void Bomb::InitializeGraphicRessources(SDL_Renderer *iRenderer) 
	{
		_Bomb = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(iRenderer, "test/bomb.png"), SDL_DestroyTexture);
	}

	void Bomb::Render(SDL_Renderer *iRenderer) const 
	{
		if (!_Bomb) 
		{
			InitializeGraphicRessources(iRenderer);
		}

		SDL_Rect r;
		r.w = 64;
		r.h = 64;
		r.x = x * r.w + mx * 8 + 20;
		r.y = y * r.h + my * 8 + 20;

		SDL_RenderCopy(iRenderer, _Bomb.get(), NULL, &r);
	}
}
}