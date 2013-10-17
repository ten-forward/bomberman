#pragma once

#include "player.hpp"
#include "inputstate.hpp"
#include "scriptapi.hpp"

#include <memory>


namespace bomberman {
namespace bestiary {

	class Computer;
	typedef std::shared_ptr<Computer> ComputerPtr;

	class Computer : public Player
	{
		ScriptAPI _script;
		
	public:
		static ComputerPtr Create(const std::string &iName, const std::string &iSpriteName, const std::string &iAiScript, int iInputStateIdx, SDL_Renderer* iRenderer);
		Computer(ScriptAPI scrip);
		virtual void Evolve(const std::vector<InputState>& iInputs, uint32_t iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const;
		virtual void Interact(const std::vector<InputState>& , uint32_t , const EntitySet &);
	};
}
}
