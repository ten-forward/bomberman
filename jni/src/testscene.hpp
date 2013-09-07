#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

#include <string>
#include <memory>
#include <list>

#include "scene_interface.hpp"
#include "map.hpp"

namespace bomberman {

class TestScene : public SceneInterface
{
	public:
		TestScene();
		virtual ~TestScene() {}
		virtual void Init(SDL_Window* window, SDL_Renderer* renderer);
		virtual void Update(const std::vector<InputState>& inputs, uint32_t timestamp);
		virtual void Render(SDL_Renderer *renderer);
		virtual bool Running();
	private:
		MapPtr _presentMap, _futurMap;
};

}
