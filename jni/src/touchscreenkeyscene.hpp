#ifndef TOUCHSCREENKEYSCENE_HPP
#define TOUCHSCREENKEYSCENE_HPP

#include <SDL.h>
#include <boost/tr1/memory.hpp>

#include "scene_interface.hpp"

class TouchScreenKeyScene : public SceneInterface
{
	std::tr1::shared_ptr<SDL_Texture> up;
	std::tr1::shared_ptr<SDL_Texture> down;
	std::tr1::shared_ptr<SDL_Texture> left;
	std::tr1::shared_ptr<SDL_Texture> right;
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
	virtual void Update(const InputState& inputs);
	virtual void Render(SDL_Renderer *renderer);
	virtual bool Running();
};

#endif // TOUCHSCREENKEYSCENE_HPP
