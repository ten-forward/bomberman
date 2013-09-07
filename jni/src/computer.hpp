#pragma once

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
		void Evolve(const std::vector<InputState>& iInputs, uint32_t iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const;
		virtual void Interact(const std::vector<InputState>& , uint32_t , const EntitySet &) {}
	};
}
}
