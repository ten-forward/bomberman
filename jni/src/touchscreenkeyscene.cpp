#include "touchscreenkeyscene.hpp"
#include "constants_resolution.hpp"
#include <SDL_image.h>

#include "printlog.hpp"

namespace bomberman {
    TouchScreenKeyScene::TouchScreenKeyScene(const std::shared_ptr<SceneInterface> &child) : child(child)
{
}

TouchScreenKeyScene::~TouchScreenKeyScene()
{
}

void TouchScreenKeyScene::Init(SDL_Window* window, SDL_Renderer* renderer)
{
	up = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, "buttons/upup.png"), SDL_DestroyTexture);
	down = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, "buttons/downup.png"), SDL_DestroyTexture);
	left = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, "buttons/leftup.png"), SDL_DestroyTexture);
	right = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, "buttons/rightup.png"), SDL_DestroyTexture);
	bomb = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, "buttons/bombup.png"), SDL_DestroyTexture);
	
    SDL_SetTextureAlphaMod(up.get(), 100);
    SDL_SetTextureAlphaMod(down.get(), 100);
    SDL_SetTextureAlphaMod(left.get(), 100);
    SDL_SetTextureAlphaMod(right.get(), 100);
    SDL_SetTextureAlphaMod(bomb.get(), 100);
    
	//SDL_GetWindowSize(window, &screenWidth, &screenHeight);
    
    screenWidth = constants::SCREEN_WIDTH;
    screenHeight = constants::SCREEN_HEIGHT;
    
	uprect.w = downrect.w = leftrect.w = rightrect.w = bombrect.w = 50;
	uprect.h = downrect.h = leftrect.h = rightrect.h = bombrect.h = 50;
	
	uprect.x = 150;
	uprect.y = screenHeight - 200;
	
	downrect.x = 150;
	downrect.y = screenHeight - 100;

	leftrect.x = 100;
	leftrect.y = screenHeight - 150;

	rightrect.x = 200;
	rightrect.y = screenHeight - 150;
    
    bombrect.x = 20;
    bombrect.y = screenHeight - 150;
    
    child->Init(window, renderer);

}

static bool EnclosePoints(const SDL_Point * points, int count, const SDL_Rect * clip, SDL_Rect * result)
{
    SDL_Rect r;
    
    // expand the area a little for fatfingerness
    r.x = clip->x - 10;
    r.y = clip->y - 10;
    r.w = clip->w + 20;
    r.h = clip->h + 20;
    return SDL_EnclosePoints(points, count, &r, result);
}
    
void TouchScreenKeyScene::Update(const std::vector<InputState>& inputvector, Uint32 timestamp)
{
	InputState& is = (InputState&)inputvector[0];
	is.SetButtonState(InputState::UP, false);
	is.SetButtonState(InputState::DOWN, false);
	is.SetButtonState(InputState::LEFT, false);
	is.SetButtonState(InputState::RIGHT, false);
    is.SetButtonPressed(InputState::X, false);
	
	if (is.GetFingered())
	{
		SDL_Point p;
		p.x = is.GetFingerX() * screenWidth;
		p.y = is.GetFingerY() * screenHeight;

		//printlog("finger: %f %f", finger->x, finger->y);

		is.SetButtonState(InputState::UP, EnclosePoints(&p, 1, &uprect, NULL) != 0);
		is.SetButtonState(InputState::DOWN, EnclosePoints(&p, 1, &downrect, NULL) != 0);
		is.SetButtonState(InputState::LEFT, EnclosePoints(&p, 1, &leftrect, NULL) != 0);
		is.SetButtonState(InputState::RIGHT, EnclosePoints(&p, 1, &rightrect, NULL) != 0);
		is.SetButtonPressed(InputState::X, EnclosePoints(&p, 1, &bombrect, NULL) != 0);
	}
    
    child->Update(inputvector, timestamp);
}

void TouchScreenKeyScene::Render(SDL_Renderer *renderer)
{
    child->Render(renderer);
    
	SDL_RenderCopy(renderer, up.get(), NULL, &uprect);
	SDL_RenderCopy(renderer, down.get(), NULL, &downrect);
	SDL_RenderCopy(renderer, left.get(), NULL, &leftrect);
	SDL_RenderCopy(renderer, right.get(), NULL, &rightrect);
	SDL_RenderCopy(renderer, bomb.get(), NULL, &bombrect);
}

bool TouchScreenKeyScene::Running()
{
	return child->Running();
}
}