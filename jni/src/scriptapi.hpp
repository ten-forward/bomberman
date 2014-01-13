#pragma once
#include <luacppinterface.h>

#include "printlog.hpp"
#include "inputstate.hpp"
#include "map.hpp"

namespace bomberman {

class ScriptAPI
{
	Lua _lua;
    LuaTable _map;
    LuaTable _actions;
    

public:
	ScriptAPI(const std::string &script);
	InputState Resume(int x, int y, int mx, int my, const MapConstPtr &iPresentMap) const;
};

}
