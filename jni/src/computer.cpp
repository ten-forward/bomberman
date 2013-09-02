#include "computer.hpp"

namespace bomberman {
namespace bestiary {
	ComputerPtr Computer::Create() 
	{
		auto computer = std::make_shared<Computer>();
		computer->_name = "T800";
		computer->zlevel = 1;
		return computer;
	}

	void Computer::Evolve(const InputState& /*iInputs*/, uint32_t /*iTimestamp*/, const MapConstPtr &/*iPresentMap*/, const MapPtr &/*iFutureMap*/) const
	{
		// random "AI" player
		/*if (computer->mx == 0 && computer->my == 0)
		{
			computer->dx = rand() % 3 - 1;
			computer->dy = rand() % 3 - 1;
		}*/
	}
}
}
