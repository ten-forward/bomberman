#ifndef SCENE_INTERFACE_HPP
#define SCENE_INTERFACE_HPP

#include "inputstate.hpp"

class SceneInterface
{
public:
	SceneInterface() {}
	virtual ~SceneInterface() {}
	
	virtual void Update(const InputState& inputs) = 0;
	virtual void Render(SDL_Renderer *renderer) = 0;
	virtual bool Running() = 0;
};

#endif // SCENE_INTERFACE_HPP
