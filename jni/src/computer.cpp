#include "computer.hpp"

#include <fstream>

namespace bomberman {
namespace bestiary {
	ComputerPtr Computer::Create(PlayerId id, const std::string &iName, const std::string &iSpriteName, const std::string &iAiScript, int iInputStateIdx, SDL_Renderer* iRenderer)
	{
#ifdef ANDROID_TEST_SCRIPT
		std::ifstream in(iAiScript, std::ios::in | std::ios::binary);
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		std::string script = contents;

#else 
		SDL_RWops *rw = SDL_RWFromFile(iAiScript.c_str(), "r");

		auto size = rw->size(rw);
		char* script = new char[size + 1];
		memset(script, 0, size + 1);
		rw->read(rw, script, sizeof(char), size);
		SDL_FreeRW(rw);

#endif

		ScriptAPI sa(script);

		auto player = std::make_shared<Computer>(sa);
		player->id = id;
		player->_name = iName;
		player->_spriteName = iSpriteName;
		player->zlevel = 2;
		player->_frameId = 3;
		player->_nextFrameDueTime = 0;
		player->_nextUpdateDueTime = 0;
		player->_state = IdleDown;
		player->_inputStateIdx = iInputStateIdx;
		player->_nbProBomb = 0;
		player->_availableBombs = 1;
		player->_bombStrength = 2;
		player->InitializeGraphicRessources(iRenderer);

#ifndef ANDROID_TEST_SCRIPT
		delete[] script;
#endif

		return player;
	}

	Computer::Computer(ScriptAPI script) : 
		_script(script)
	{
	}

	void Computer::Evolve(const std::vector<InputState>& /* iInputs */, uint32_t iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const
	{
		std::vector<InputState> inputs(4);

		if (_nextUpdateDueTime < iTimestamp)
		{
            // TODO only pass x y
			inputs[_inputStateIdx] = _script.Resume(GetX(), GetY(), mx, my, iPresentMap);
		}
	
		EvolutionRoutine(std::make_shared<Computer>(*this), inputs, iTimestamp, iPresentMap, iFutureMap);
	}

	void Computer::Interact(const std::vector<InputState>& iInputs, uint32_t iTimestamp, const EntitySet &iEntities)
	{
		Player::Interact(iInputs, iTimestamp, iEntities);
	}
}
}
