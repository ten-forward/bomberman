#pragma once

#include <SDL.h>

#include "scene_interface.hpp"
#include <memory>

namespace bomberman {
    
class TouchScreenKeyScene : public SceneInterface
{
	std::shared_ptr<SDL_Texture> up;
	std::shared_ptr<SDL_Texture> down;
	std::shared_ptr<SDL_Texture> left;
	std::shared_ptr<SDL_Texture> right;
	std::shared_ptr<SDL_Texture> bomb;
    
    std::shared_ptr<SceneInterface> child;
    
	SDL_Rect uprect;
	SDL_Rect downrect;
	SDL_Rect leftrect;
	SDL_Rect rightrect;
    SDL_Rect bombrect;
	int screenWidth;
	int screenHeight;

public:
	TouchScreenKeyScene(const std::shared_ptr<SceneInterface> &child);
	virtual ~TouchScreenKeyScene();
	virtual void Init(SDL_Window* window, SDL_Renderer* renderer);
	virtual void Update(const std::vector<InputState>& inputs, Uint32 timestamp);
	virtual void Render(SDL_Renderer *renderer);
	virtual bool Running();
};
}