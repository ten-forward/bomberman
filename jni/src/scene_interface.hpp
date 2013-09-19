#pragma once

#include "inputstate.hpp"
#include "sceneresult.hpp"

#include <SDL.h>

#include <vector>

namespace bomberman {

class SceneInterface
{
public:
	SceneInterface() {}
	virtual ~SceneInterface() {}
	
	virtual void Init(SDL_Window* window, SDL_Renderer* renderer) = 0;
	virtual SceneResultPtr Update(const std::vector<InputState>& inputs, Uint32 timestamp) = 0;
	virtual void Render(SDL_Renderer *renderer) = 0;
	virtual bool Running() = 0;
};
}
