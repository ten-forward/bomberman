#pragma once

#include <SDL.h>
#include "inputstate.hpp"

#include <vector>

class SceneInterface
{
public:
	SceneInterface() {}
	virtual ~SceneInterface() {}
	
	virtual void Init(SDL_Window* window, SDL_Renderer* renderer) = 0;
	virtual void Update(const std::vector<InputState>& inputs, Uint32 timestamp) = 0;
	virtual void Render(SDL_Renderer *renderer) = 0;
	virtual bool Running() = 0;
};
