#ifndef TESTSCENE_HPP
#define TESTSCENE_HPP

#include <string>

#include <SDL.h>
#include <SDL_ttf.h>
#include <boost/tr1/memory.hpp>
#include "scene_interface.hpp"
#include "map.hpp"
#include "touchscreenkeyscene.hpp"

class TestScene : public SceneInterface
{
	Map map;
	Map::entity_type player;
	Map::entity_type computer;
	TouchScreenKeyScene keys;
	
	std::tr1::shared_ptr<SDL_Texture> block;
	std::tr1::shared_ptr<SDL_Texture> bomb;
	std::tr1::shared_ptr<SDL_Texture> bomberman;
	std::tr1::shared_ptr<SDL_Texture> bombergirl;
	std::tr1::shared_ptr<SDL_Texture> floortile;

public:
	TestScene();
	virtual ~TestScene();
	virtual void Init(SDL_Window* window, SDL_Renderer* renderer);
	virtual void Update(const InputState& inputs);
	virtual void Render(SDL_Renderer *renderer);
	virtual bool Running();
};

#endif // TESTSCENE_HPP
