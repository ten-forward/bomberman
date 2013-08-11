#ifndef COMPUTER_HPP
#define COMPUTER_HPP

#include "player.hpp"

// STL - libstdc++
#include <memory>

namespace bomberman {
namespace bestiary {

	class Computer;
	typedef std::shared_ptr<Computer> ComputerPtr;

	class Computer : public Player {
	public:
		static ComputerPtr Create();
		//virtual void Render(SDL_Renderer*) const;
	};
}
}
#endif