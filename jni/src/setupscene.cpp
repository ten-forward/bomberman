#include "setupscene.hpp"
#include "constants_resolution.hpp"
#include "utils.hpp"

#include <algorithm>

namespace bomberman {

SetupScene::SetupScene()
{
}

void SetupScene::Init(SDL_Window* window, SDL_Renderer* renderer)
{

	_selectionTwink = std::shared_ptr<Mix_Chunk>(Mix_LoadWAV("sound/select.wav"), Mix_FreeChunk);

	_background = utils::LoadTexture(renderer, "drawable/setupback.png");
	_playerReady =  utils::LoadTexture(renderer, "drawable/playerstatus.png");
	
	for (int i=0;i<4;i++)
	{
		players[i].present = false;
		players[i].isComputer = false;
		players[i].aiScript = "aiscripts/example.lua";
	}

	players[0].name = "Athos";
	players[0].spriteName = "drawable/miku2.png";

	players[1].name = "Porthos";
	players[1].spriteName = "drawable/duckie.png";

	players[2].name = "Aramis";
	players[2].spriteName = "drawable/manji.png";

	players[3].name = "D'Artagnan";
	players[3].spriteName = "drawable/whitebbman.png";
	
	_running = true;
}

void SetupScene::Update(const std::vector<InputState>& inputs, uint32_t timestamp)
{
	
	for (int i=0;i<4;i++)
	{
		if (inputs[i].GetButtonPressed(InputState::A))
		{
			Mix_PlayChannel(-1, _selectionTwink.get(), 0);
			players[i].present = !players[i].present;
			players[i].isComputer = false;
		}

		if (inputs[i].GetButtonPressed(InputState::L1))
		{
			Mix_PlayChannel(-1, _selectionTwink.get(), 0);
			players[0].isComputer = !players[0].isComputer;
			players[0].present = players[0].isComputer;
		}

		if (inputs[i].GetButtonPressed(InputState::L2))
		{
			Mix_PlayChannel(-1, _selectionTwink.get(), 0);
			players[2].isComputer = !players[2].isComputer;
			players[2].present = players[2].isComputer;
		}

		if (inputs[i].GetButtonPressed(InputState::R1))
		{
			Mix_PlayChannel(-1, _selectionTwink.get(), 0);
			players[1].isComputer = !players[1].isComputer;
			players[1].present = players[1].isComputer;
		}

		if (inputs[i].GetButtonPressed(InputState::R2))
		{
			Mix_PlayChannel(-1, _selectionTwink.get(), 0);
			players[3].isComputer = !players[3].isComputer;
			players[3].present = players[3].isComputer;
		}
	}
	
	int playerCount = 0;
	for (int i=0;i<4;i++)
	{
		if (players[i].present)
		{
			playerCount++;
		}
	}
		
	for (int i=0;i<4;i++)
	{
		if (inputs[i].GetButtonPressed(InputState::START) && playerCount > 1)
		{
			_running = false;
		}
	}
}

struct Positions
{
	int x,y;
};

Positions pos[] =
{
	{33,45},
	{243,45},
	{33,159},
	{243,159}
};

void SetupScene::Render(SDL_Renderer *renderer)
{	
	utils::DrawImage(renderer, _background, 0, 0, constants::SCREEN_WIDTH, constants::SCREEN_HEIGHT);
	
	for (int i=0;i<4;i++)
	{
		SDL_Rect sourceRect;
		sourceRect.w = 205;
		sourceRect.h = 60;
		sourceRect.x = 0;
		sourceRect.y = 0 + 60 * (players[i].present << players[i].isComputer);

		SDL_Rect r;
		r.w = 205 * constants::SPRITE_MAGNIFICATION;
		r.h = 60 * constants::SPRITE_MAGNIFICATION;
		r.x = pos[i].x * constants::SPRITE_MAGNIFICATION;
		r.y = pos[i].y * constants::SPRITE_MAGNIFICATION;
		SDL_RenderCopy(renderer, _playerReady.get(), &sourceRect, &r);
	}
}

bool SetupScene::Running()
{
	return _running;
}

}