#pragma once

#include "scene_interface.hpp"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <memory>

namespace bomberman {

class VictoryScene : public SceneInterface
{
	public:

		VictoryScene(int victor);
		virtual ~VictoryScene() {}
		virtual void Init(SDL_Window* window, SDL_Renderer* renderer);
		virtual void Update(const std::vector<InputState>& inputs, uint32_t timestamp);
		virtual void Render(SDL_Renderer *renderer);
		virtual bool Running();

	private:
		
		std::shared_ptr<Mix_Music> _music;
		std::shared_ptr<SDL_Texture> _theVictorIs;

		bool _running;
		int _victor;
};

}
