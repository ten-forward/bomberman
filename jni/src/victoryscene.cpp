#include "victoryscene.hpp"
#include "utils.hpp"
#include "printlog.hpp"

#include <sstream>

namespace bomberman {

VictoryScene::VictoryScene(int victor) :
	_victor(victor),
	_music(Mix_LoadMUS("music/end.flac"), Mix_FreeMusic)
{
}

void VictoryScene::Init(SDL_Window* window, SDL_Renderer* renderer)
{
	auto font = utils::LoadFont("test/Gamegirl.ttf", 64);

	std::stringstream ss;
	ss << "Player " << (_victor + 1) << " wins!";
	_theVictorIs = utils::DrawString(renderer, font, ss.str(), utils::MakeColor(0xffffffff));
	
	if(Mix_PlayMusic(_music.get(), -1) == -1)
	{
		printlog("Mix_PlayMusic: %s\n", Mix_GetError());
	}

	_running = true;
}

void VictoryScene::Update(const std::vector<InputState>& inputs, uint32_t timestamp)
{
	for (int i=0;i<4;i++)
	{	
		if (inputs[i].GetAButtonJustPressed())
		{
			_running = false;
		}
	}
}

void VictoryScene::Render(SDL_Renderer *renderer)
{
	utils::DrawImageAtCenter(renderer, _theVictorIs);
}

bool VictoryScene::Running()
{
	return _running;
}

}