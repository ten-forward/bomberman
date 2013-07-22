#ifndef TESTSCENE_HPP
#define TESTSCENE_HPP

#include <string>

#include <SDL.h>
#include <SDL_ttf.h>
#include <boost/tr1/memory.hpp>
#include "scene_interface.hpp"

class TestScene : public SceneInterface
{
	std::tr1::shared_ptr<TTF_Font> font;

	std::tr1::shared_ptr<SDL_Texture> left;
	SDL_Rect leftrect;
	std::tr1::shared_ptr<SDL_Texture> right;
	SDL_Rect rightrect;

	bool x;
public:
	TestScene();
	virtual ~TestScene();
	virtual void Update(const InputState& inputs);
	virtual void Render(SDL_Renderer *renderer);
	virtual bool Running();
};

#endif // TESTSCENE_HPP
