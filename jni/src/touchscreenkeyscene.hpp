#pragma once

#include <SDL.h>
#include <memory>

#include "scene_interface.hpp"

class TouchScreenKeyScene : public SceneInterface
{
	std::shared_ptr<SDL_Texture> up;
	std::shared_ptr<SDL_Texture> down;
	std::shared_ptr<SDL_Texture> left;
	std::shared_ptr<SDL_Texture> right;
	SDL_Rect uprect;
	SDL_Rect downrect;
	SDL_Rect leftrect;
	SDL_Rect rightrect;
	int screenWidth;
	int screenHeight;

public:
	TouchScreenKeyScene();
	virtual ~TouchScreenKeyScene();
	virtual void Init(SDL_Window* window, SDL_Renderer* renderer);
	virtual void Update(const InputState& inputs, Uint32 timestamp);
	virtual void Render(SDL_Renderer *renderer);
	virtual bool Running();
};
