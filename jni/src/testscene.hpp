#ifndef TESTSCENE_HPP
#define TESTSCENE_HPP

#include <string>

#include <SDL.h>
#include <SDL_ttf.h>
#include <memory>
#include <list>
#include "scene_interface.hpp"
#include "map.hpp"
#include "touchscreenkeyscene.hpp"

namespace bomberman {

class TestScene : public SceneInterface
{
	public:
		TestScene();
		virtual ~TestScene();
		virtual void Init(SDL_Window* window, SDL_Renderer* renderer);
		virtual void Update(const InputState& inputs, Uint32 timestamp);
		virtual void Render(SDL_Renderer *renderer);
		virtual bool Running();
	private:
		Map map;
		EntityPtr player;
		EntityPtr computer;
		
		std::list<EntityPtr> overlappingBombs;
};

}

#endif // TESTSCENE_HPP
