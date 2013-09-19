#pragma once

#include "scene_interface.hpp"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "testscene.hpp"

namespace bomberman {

class SetupScene : public SceneInterface
{
	public:

		SetupScene();
		virtual ~SetupScene() {}
		virtual void Init(SDL_Window* window, SDL_Renderer* renderer);
		virtual void Update(const std::vector<InputState>& inputs, uint32_t timestamp);
		virtual void Render(SDL_Renderer *renderer);
		virtual bool Running();

		PlayerConfigArray GetConfig() const { return players; }

	private:
		bool _running;
		std::shared_ptr<SDL_Texture> _background;

		std::shared_ptr<SDL_Texture> _playerReady;

		std::shared_ptr<Mix_Chunk> _selectionTwink;

		PlayerConfigArray players;

		int _playerCount;
};

}
