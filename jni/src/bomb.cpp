#include "bomb.hpp"
#include "explosion.hpp"

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

	void Bomb::Evolve(const InputState& /*iInputs*/, uint32_t iTimestamp, const MapConstPtr &/*iPresentMap*/, const MapPtr &iFutureMap) const
	{
		if (iTimestamp >= _timeout) {
			const int kExplosionTimer = 200;
			auto blast = Explosion::Create(iTimestamp + kExplosionTimer);
			blast->x = x;
			blast->y = y;
			iFutureMap->SetEntity(blast);
		} else {
			iFutureMap->SetEntity(shared_from_this());
		}
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