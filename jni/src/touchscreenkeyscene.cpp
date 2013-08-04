#include "touchscreenkeyscene.hpp"
#include <SDL_image.h>

#include "printlog.hpp"

TouchScreenKeyScene::TouchScreenKeyScene()
{
}

TouchScreenKeyScene::~TouchScreenKeyScene()
{
}

void TouchScreenKeyScene::Init(SDL_Window* window, SDL_Renderer* renderer)
{
	up = std::tr1::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, "buttons/upup.png"), SDL_DestroyTexture);
	down = std::tr1::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, "buttons/downup.png"), SDL_DestroyTexture);
	left = std::tr1::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, "buttons/leftup.png"), SDL_DestroyTexture);
	right = std::tr1::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, "buttons/rightup.png"), SDL_DestroyTexture);
	
	SDL_GetWindowSize(window, &screenWidth, &screenHeight);
	uprect.w = downrect.w = leftrect.w = rightrect.w = 50;
	uprect.h = downrect.h = leftrect.h = rightrect.h = 50;
	
	uprect.x = screenWidth - 150;
	uprect.y = screenHeight - 250;
	
	downrect.x = screenWidth - 150;
	downrect.y = screenHeight - 150;

	leftrect.x = screenWidth - 200;
	leftrect.y = screenHeight - 200;

	rightrect.x = screenWidth - 100;
	rightrect.y = screenHeight - 200;

}

void TouchScreenKeyScene::Update(const InputState& inputs, Uint32 timestamp)
{
	InputState& is = (InputState&)inputs;
	is.SetUpButtonState(false);
	is.SetDownButtonState(false);
	is.SetLeftButtonState(false);
	is.SetRightButtonState(false);
	
	if (inputs.GetFingered())
	{
		SDL_Point p;
		p.x = inputs.GetFingerX() * screenWidth;
		p.y = inputs.GetFingerY() * screenHeight;

		//printlog("finger: %f %f", finger->x, finger->y);

		is.SetUpButtonState(SDL_EnclosePoints(&p, 1, &uprect, NULL) != 0);
		is.SetDownButtonState(SDL_EnclosePoints(&p, 1, &downrect, NULL) != 0);
		is.SetLeftButtonState(SDL_EnclosePoints(&p, 1, &leftrect, NULL) != 0);
		is.SetRightButtonState(SDL_EnclosePoints(&p, 1, &rightrect, NULL) != 0);
	}
}

void TouchScreenKeyScene::Render(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, up.get(), NULL, &uprect);
	SDL_RenderCopy(renderer, down.get(), NULL, &downrect);
	SDL_RenderCopy(renderer, left.get(), NULL, &leftrect);
	SDL_RenderCopy(renderer, right.get(), NULL, &rightrect);
}

bool TouchScreenKeyScene::Running()
{
	return true;
}
