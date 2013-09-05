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
	}
}
}
