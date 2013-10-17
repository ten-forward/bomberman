#include "fadescene.hpp"

namespace bomberman {

FadeScene::FadeScene(const std::shared_ptr<SceneInterface> &scene)
	: _scene(scene), _speed(10)
{
}

FadeScene::~FadeScene()
{
}
	
void FadeScene::Init(SDL_Window* window, SDL_Renderer* renderer)
{
	_scene->Init(window, renderer);

	SDL_GetWindowSize(window, &_w, &_h);
	_renderTarget = std::shared_ptr<SDL_Texture>(
		SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, _w, _h),
		SDL_DestroyTexture);

	// make it so that the texture can be transparent
	SDL_SetTextureBlendMode(_renderTarget.get(), SDL_BLENDMODE_BLEND);

	_opacity = 0;
}

void FadeScene::Update(const std::vector<InputState>& inputs, uint32_t timestamp)
{
	if (_scene->Running())
	{
		_scene->Update(inputs, timestamp);
	}
}

void FadeScene::Render(SDL_Renderer *renderer)
{
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = _w;
	rect.h = _h;

	auto target = SDL_GetRenderTarget(renderer);

	int res = SDL_SetRenderTarget(renderer, _renderTarget.get());
	_scene->Render(renderer);
	res = SDL_SetRenderTarget(renderer, target);
	
	if (_scene->Running() && _opacity != 255)
	{
		_opacity = std::min(_opacity + _speed, 255.0);
	}
	else if (!_scene->Running() && _opacity != 0)
	{
		_opacity = std::max(_opacity - _speed, 0.0);
	}

	res = SDL_SetTextureAlphaMod(_renderTarget.get(), _opacity);

	SDL_RenderCopy(renderer, _renderTarget.get(), &rect, &rect);
}

bool FadeScene::Running()
{
	return _scene->Running() || _opacity != 0;
}

void FadeScene::SetSpeed(double _speed)
{
	this->_speed = _speed;
}

double FadeScene::GetSpeed() const
{
	return _speed;
}

}
