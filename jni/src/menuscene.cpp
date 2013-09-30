#include "menuscene.hpp"
#include "constants.hpp"
#include "utils.hpp"

#include <algorithm>

namespace bomberman {

MenuScene::MenuScene()
{
}

void MenuScene::Init(SDL_Window* window, SDL_Renderer* renderer)
{
	auto font = utils::LoadFont("test/Gamegirl.ttf", 64);

	_selectionTwink = std::shared_ptr<Mix_Chunk>(Mix_LoadWAV("sound/select.wav"), Mix_FreeChunk);

	_background = utils::LoadTexture(renderer, "test/mainback.png");
	
	_bombcursor = utils::LoadTexture(renderer, "test/bombcursor.png");

	_newGame = utils::DrawString(renderer, font, "New Game", utils::MakeColor(0xffffffff));
	_credits = utils::DrawString(renderer, font, "Credits", utils::MakeColor(0xffffffff));

	_frameId = 0;
	_nextFrameDueTime = 0;

	selection = 0;

	_running = true;
}

void MenuScene::Update(const std::vector<InputState>& inputs, uint32_t timestamp)
{
	if (_nextFrameDueTime < timestamp)
	{
		_frameId++;
		_frameId %= 3;
		_nextFrameDueTime = timestamp + 50;
	}

	if (inputs[0].GetButtonPressed(InputState::UP))
	{	
		Mix_PlayChannel(-1, _selectionTwink.get(), 0);
		selection = std::max(0, selection - 1);
	}

	if (inputs[0].GetButtonPressed(InputState::DOWN))
	{
		Mix_PlayChannel(-1, _selectionTwink.get(), 0);
		selection = std::min(1, selection + 1);
	}

	if (inputs[0].GetButtonPressed(InputState::A))
	{
		_running = false;
	}
}

void MenuScene::Render(SDL_Renderer *renderer)
{	
	utils::DrawImage(renderer, _background, 0, 0, constants::SCREEN_WIDTH, constants::SCREEN_HEIGHT);

	utils::DrawImage(renderer, _newGame, 1200, 600);
	utils::DrawImage(renderer, _credits, 1200, 670);

	//utils::DrawImage(renderer, _bombcursor, 1136, 600, 64, 64);

	SDL_Rect sourceRect;
	sourceRect.w = 16;
	sourceRect.h = 16;
	sourceRect.x = 0 + _frameId * 16;
	sourceRect.y = 0;

	SDL_Rect r;
	r.w = 64;
	r.h = 64;
	r.x = 1136;
	r.y = 600 + selection * 70;

	SDL_RenderCopy(renderer, _bombcursor.get(), &sourceRect, &r);
}

bool MenuScene::Running()
{
	return _running;
}

}