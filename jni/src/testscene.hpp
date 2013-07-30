#ifndef TESTSCENE_HPP
#define TESTSCENE_HPP

#include <string>

#include <SDL.h>
#include <SDL_ttf.h>
#include <boost/tr1/memory.hpp>
#include "scene_interface.hpp"
#include "map.hpp"

class TestScene : public SceneInterface
{
	Map map;
	Map::entity_type player;

public:
	TestScene();
	virtual ~TestScene();
	virtual void Init(SDL_Window* window, SDL_Renderer* renderer);
	virtual void Update(const InputState& inputs);
	virtual void Render(SDL_Renderer *renderer);
	virtual bool Running();
};

#endif // TESTSCENE_HPP
