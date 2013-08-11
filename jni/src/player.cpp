#include "player.hpp"

// SDL
#include <SDL_image.h>

namespace bomberman {
namespace bestiary {

	PlayerPtr Player::Create(const std::string &iName)
	{
		auto player = std::make_shared<Player>();
		player->_name = iName;
		return player;
	}

	std::shared_ptr<SDL_Texture>  Player::_Bomberman;

	void Player::InitializeGraphicRessources(SDL_Renderer *iRenderer) 
	{
		//bombergirl = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, "test/bombergirl.png"), SDL_DestroyTexture);
		_Bomberman = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(iRenderer, "test/bomberman.png"), SDL_DestroyTexture);
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

		SDL_RenderCopy(iRenderer, _Bomberman.get(), NULL, &r);
	}

}
}