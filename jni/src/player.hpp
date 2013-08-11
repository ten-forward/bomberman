#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "entity.hpp"

// STL- libstd
#include <memory>
#include <string>

// Forward declarations
struct SDL_Texture;

namespace bomberman {
namespace bestiary {

	class Player;

	typedef std::shared_ptr<Player> PlayerPtr;

	class Player : public bomberman::Entity {
		public:
			static PlayerPtr Create(const std::string &iName);
			virtual void Render(SDL_Renderer*) const;
		protected:
			std::string _name;
			static void InitializeGraphicRessources(SDL_Renderer *iRenderer);
			static std::shared_ptr<SDL_Texture> _Bomberman;
	};
}
}

#endif