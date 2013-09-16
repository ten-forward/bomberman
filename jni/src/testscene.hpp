#pragma once

#include "scene_interface.hpp"
#include "map.hpp"

#include <SDL.h>
#include <SDL_ttf.h>

#include <string>
#include <memory>
#include <list>
#include <array>
#include <map>

#include <boost/circular_buffer.hpp>

namespace bomberman {

struct PlayerConfig
{
	bool present;
	std::string spriteName;
	std::string name;
};

class TestScene : public SceneInterface
{
	typedef std::array<PlayerConfig, 4> PlayerConfigArray;
	public:
		TestScene(PlayerConfigArray playerConfig);
		virtual ~TestScene() {}
		virtual void Init(SDL_Window* window, SDL_Renderer* renderer);
		virtual void Update(const std::vector<InputState>& inputs, uint32_t timestamp);
		virtual void Render(SDL_Renderer *renderer);
		virtual bool Running();

		void BackThroughTime(SDL_Renderer *renderer, uint32_t now);

	private:
		MapPtr _presentMap;
		boost::circular_buffer<std::pair<uint32_t, MapPtr>> _pastMaps;
		PlayerConfigArray _playerConfig;
};

}
