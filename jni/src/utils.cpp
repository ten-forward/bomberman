#include "constants.hpp"
#include "utils.hpp"

namespace bomberman {
namespace utils {
	SDL_Rect GetSafeArea1920()
	{
		using namespace bomberman::constants;

		SDL_Rect rect;

		rect.w = SAFE_WIDTH;
		rect.h = SAFE_HEIGHT;
		rect.x = SAFE_X;
		rect.y = SAFE_Y;

		return rect;
	}
	
	std::shared_ptr<SDL_Texture> LoadTexture(SDL_Renderer* renderer, std::string filename)
	{
		auto surface = IMG_Load(filename.c_str());
		auto result = LoadTexture(renderer, surface);
		SDL_FreeSurface(surface);
		return result;
	}

	std::shared_ptr<SDL_Texture> LoadTexture(SDL_Renderer* renderer, SDL_Surface* surface)
	{
		auto result = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, surface), SDL_DestroyTexture);
		return result;
	}

	std::shared_ptr<TTF_Font> LoadFont(std::string fontName, int ptSize)
	{
		std::shared_ptr<TTF_Font> font(TTF_OpenFont(fontName.c_str(), ptSize), TTF_CloseFont);
		return font;
	}

	std::shared_ptr<SDL_Texture> DrawString(SDL_Renderer* renderer, std::shared_ptr<TTF_Font> font, std::string text, SDL_Color fg)
	{
		auto surface = TTF_RenderUTF8_Solid(font.get(), text.c_str(), fg);
		auto result = LoadTexture(renderer, surface);
		SDL_FreeSurface(surface);
		return result;
	}

	SDL_Color MakeColor(Uint32 color)
	{
		SDL_Color c;
		c.r = color >> 24;
		c.g = (color >> 16) & 0xff;
		c.b = (color >> 8) & 0xff;
		c.a = color & 0xff;
		return c;
	}

	void DrawImage(SDL_Renderer* renderer, std::shared_ptr<SDL_Texture> image, int x, int y, int w, int h)
	{	
		SDL_Rect r;
		SDL_QueryTexture(image.get(), NULL, NULL, &r.w, &r.h);
		r.x = x;
		r.y = y;
		r.w = w == -1 ? r.w : w;
		r.h = h == -1 ? r.h : h;

		SDL_RenderCopy(renderer, image.get(), NULL, &r);
	}
	
	void DrawImageAtCenter(SDL_Renderer* renderer, std::shared_ptr<SDL_Texture> image)
	{	
		SDL_Rect r;
		SDL_QueryTexture(image.get(), NULL, NULL, &r.w, &r.h);
		r.x = (constants::SCREEN_WIDTH - r.w) / 2;
		r.y = (constants::SCREEN_HEIGHT - r.h) / 2;

		SDL_RenderCopy(renderer, image.get(), NULL, &r);
	}
}
}

