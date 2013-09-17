#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <string>
#include <memory>
#include <list>
#include <array>

#include "scene_interface.hpp"
#include "map.hpp"

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

	private:
		std::shared_ptr<Mix_Music> music;
		std::shared_ptr<SDL_Texture> texture;
		MapPtr _presentMap, _futurMap;
		PlayerConfigArray _playerConfig;
};

}
