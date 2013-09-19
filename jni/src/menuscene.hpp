#pragma once

#include "scene_interface.hpp"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <memory>

namespace bomberman {

class MenuScene : public SceneInterface
{
	public:

		enum Selection
		{
			NEWGAME,
			CREDITS
		};

		MenuScene();
		virtual ~MenuScene() {}
		virtual void Init(SDL_Window* window, SDL_Renderer* renderer);
		virtual void Update(const std::vector<InputState>& inputs, uint32_t timestamp);
		virtual void Render(SDL_Renderer *renderer);
		virtual bool Running();

		Selection GetSelection() const
		{
			return (Selection)selection;
		}

	private:
		bool _running;
		std::shared_ptr<SDL_Texture> _background;

		std::shared_ptr<SDL_Texture> _bombcursor;

		std::shared_ptr<SDL_Texture> _newGame;
		std::shared_ptr<SDL_Texture> _credits;

		std::shared_ptr<Mix_Chunk> _selectionTwink;

		int _frameId;
		uint32_t _nextFrameDueTime;

		int selection;
};

}
