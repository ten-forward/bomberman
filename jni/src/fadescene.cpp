#include "fadescene.hpp"

FadeScene::FadeScene(std::shared_ptr<SceneInterface> scene)
	: scene(scene), speed(10)
{
}

FadeScene::~FadeScene()
{
}
	
void FadeScene::Init(SDL_Window* window, SDL_Renderer* renderer)
{
	scene->Init(window, renderer);

	SDL_GetWindowSize(window, &w, &h);
	renderTarget = std::shared_ptr<SDL_Texture>(
		SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, w, h),
		SDL_DestroyTexture);

	// make it so that the texture can be transparent
	SDL_SetTextureBlendMode(renderTarget.get(), SDL_BLENDMODE_BLEND);

	opacity = 0;
}

void FadeScene::Update(const std::vector<InputState>& inputs, uint32_t timestamp)
{
	if (scene->Running())
	{
		scene->Update(inputs, timestamp);
	}
}

void FadeScene::Render(SDL_Renderer *renderer)
{
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = w;
	rect.h = h;

	auto target = SDL_GetRenderTarget(renderer);

	int res = SDL_SetRenderTarget(renderer, renderTarget.get());
	scene->Render(renderer);
	res = SDL_SetRenderTarget(renderer, target);
	
	if (scene->Running() && opacity != 255)
	{
		opacity = std::min(opacity + speed, 255.0);
	}
	else if (!scene->Running() && opacity != 0)
	{
		opacity = std::max(opacity - speed, 0.0);
	}

	res = SDL_SetTextureAlphaMod(renderTarget.get(), opacity);

	SDL_RenderCopy(renderer, renderTarget.get(), &rect, &rect);
}

bool FadeScene::Running()
{
	return scene->Running() || opacity != 0;
}

void FadeScene::SetSpeed(double speed)
{
	this->speed = speed;
}

double FadeScene::GetSpeed() const
{
	return speed;
}
