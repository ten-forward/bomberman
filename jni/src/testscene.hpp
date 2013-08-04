#ifndef TESTSCENE_HPP
#define TESTSCENE_HPP

#include <string>

#include <SDL.h>
#include <SDL_ttf.h>
#include <boost/tr1/memory.hpp>
#include <list>
#include "scene_interface.hpp"
#include "map.hpp"
#include "touchscreenkeyscene.hpp"

class TestScene : public SceneInterface
{
	struct BombInfo
	{
		int timeout;
		int strength;
	};

	struct ExplosionInfo
	{
		int x,y;
		int timeout;
		int stage;
	};

	Map map;
	Map::entity_type player;
	Map::entity_type computer;
	TouchScreenKeyScene keys;
	
	std::tr1::shared_ptr<SDL_Texture> block;
	std::tr1::shared_ptr<SDL_Texture> bomb;
	std::tr1::shared_ptr<SDL_Texture> bomberman;
	std::tr1::shared_ptr<SDL_Texture> bombergirl;
	std::tr1::shared_ptr<SDL_Texture> floortile;

	std::tr1::shared_ptr<SDL_Texture> explosionSprite[4];
	
	typedef std::pair<Map::entity_type,BombInfo> bombInfoPair;
	
	std::list<ExplosionInfo> explosions;
	std::list<bombInfoPair> bombs;
	std::list<Map::entity_type> overlappingBombs;



public:
	TestScene();
	virtual ~TestScene();
	virtual void Init(SDL_Window* window, SDL_Renderer* renderer);
	virtual void Update(const InputState& inputs, Uint32 timestamp);
	virtual void Render(SDL_Renderer *renderer);
	virtual bool Running();
};

#endif // TESTSCENE_HPP
