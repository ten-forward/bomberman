#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <string>
#include <memory>

namespace bomberman {
namespace utils {
	SDL_Rect GetSafeArea1920();
	std::shared_ptr<SDL_Texture> LoadTexture(SDL_Renderer* renderer, std::string filename);
	std::shared_ptr<SDL_Texture> LoadTexture(SDL_Renderer* renderer, SDL_Surface* surface);

	std::shared_ptr<TTF_Font> LoadFont(std::string fontName, int ptSize);
	std::shared_ptr<SDL_Texture> DrawString(SDL_Renderer* renderer, std::shared_ptr<TTF_Font> font, std::string text, SDL_Color fg);

	SDL_Color MakeColor(Uint32 color);

	void DrawImage(SDL_Renderer* renderer, std::shared_ptr<SDL_Texture> image, int x, int y, int w = -1, int h = -1);
	void DrawImageAtCenter(SDL_Renderer* renderer, std::shared_ptr<SDL_Texture> image);
}
}
