#pragma once

#include "scene_interface.hpp"

namespace bomberman {
class BattleSetupScene : public SceneInterface 
{
public:
	virtual void Init(SDL_Window* /*window*/, SDL_Renderer* /*renderer*/) {};
};
}