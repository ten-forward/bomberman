#include "testscene.hpp"


TestScene::TestScene() : 
	font(TTF_OpenFont("led_display-7.ttf", 60), TTF_CloseFont)
{
}

TestScene::~TestScene()
{

}

void TestScene::Update(const InputState& inputs)
{
	if (inputs.GetLeftButtonState())
	{
		x = true;
	}else
	{
		x = false;
	}
}

void TestScene::Render(SDL_Renderer *renderer)
{
	if (!left)
	{
		SDL_Color color = {255,255,255};

		SDL_Surface* surf = TTF_RenderText_Solid(font.get(), "left", color);
		left = std::tr1::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, surf), SDL_DestroyTexture);

		leftrect.x = 50;
		leftrect.y = 50;
		leftrect.w = surf->w;
		leftrect.h = surf->h;

		SDL_FreeSurface(surf);

		surf = TTF_RenderText_Solid(font.get(), "right", color);
		right = std::tr1::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, surf), SDL_DestroyTexture);

		rightrect.x = 50;
		rightrect.y = 50;
		rightrect.w = surf->w;
		rightrect.h = surf->h;

		SDL_FreeSurface(surf);
	}

	if (x)
	{
		SDL_RenderCopy(renderer, left.get(), NULL, &leftrect);
	}
	else
	{
		SDL_RenderCopy(renderer, right.get(), NULL, &rightrect);
	}
}

bool TestScene::Running()
{
	return true;
}
