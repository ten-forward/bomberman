#include <SDL_image.h>

#include "testscene.hpp"

#include "printlog.hpp"

#define BLOCKID 0xffff

TestScene::TestScene() : 
	map(15, 9),
	player(map.CreateEntity()),
	computer(map.CreateEntity())
{
	map.TrySetEntity(player, 0, 0);

	map.TrySetEntity(computer, 14, 0);

	Map& theMap = map;
	map.ForeachTile([&](int x, int y, const Map::entity_type &tile) {
		if (x % 2 != 0 && y % 2 != 0)
		{
			auto blockEntity = theMap.CreateEntity();
			blockEntity->userdata = BLOCKID;
			theMap.TrySetEntity(blockEntity, x, y);
		}
	});
}

TestScene::~TestScene()
{
}

void TestScene::Init(SDL_Window* window, SDL_Renderer* renderer)
{
	keys.Init(window, renderer);

	block = std::tr1::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, "test/block.png"), SDL_DestroyTexture);
	bomb = std::tr1::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, "test/bomb.png"), SDL_DestroyTexture);
	bombergirl = std::tr1::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, "test/bombergirl.png"), SDL_DestroyTexture);
	bomberman = std::tr1::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, "test/bomberman.png"), SDL_DestroyTexture);
	floortile = std::tr1::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, "test/floor.png"), SDL_DestroyTexture);
}

void TestScene::Update(const InputState& inputs)
{
#ifdef ANDROID
	keys.Update(inputs);
#endif
	if (inputs.GetLeftButtonState())
	{
		player->dx = -1;
	}
	else if (inputs.GetRightButtonState())
	{
		player->dx = 1;
	}
	else if (inputs.GetDownButtonState())
	{
		player->dy = 1;
	}
	else if (inputs.GetUpButtonState())
	{
		player->dy = -1;
	}

	if (computer->mx == 0 && computer->my == 0)
	{
		computer->dx = rand() % 3 - 1;
		computer->dy = rand() % 3 - 1;
	}

	static Uint32 lastUpdate = SDL_GetTicks();
	Uint32 now = SDL_GetTicks();
	if (now - lastUpdate > 50)
	{
		//printlog("drawing! %d", now);
		map.Update(1);
		lastUpdate = now;
	}
}

void TestScene::Render(SDL_Renderer *renderer)
{
	map.ForeachTile([&](int x, int y, const Map::entity_type &ntt)
	{
		SDL_Rect r;
		r.w = 64;
		r.h = 64;
		r.x = x * r.w + 20;	// <- just for overscan
		r.y = y * r.h + 20;
		
		SDL_RenderCopy(renderer, floortile.get(), NULL, &r);

		if (ntt)
		{
			if(ntt->userdata == BLOCKID)
			{
				SDL_RenderCopy(renderer, block.get(), NULL, &r);
			}
			//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			//SDL_RenderFillRect(renderer, &r);
		}
		else
		{
			//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			//SDL_RenderDrawRect(renderer, &r);
		}
	});

	map.ForeachEntity([&](const Map::entity_type &ntt)
	{
		SDL_Rect r;
		r.w = 64;
		r.h = 64;
		r.x = ntt->x * r.w + ntt->mx * 8 + 20;
		r.y = ntt->y * r.h + ntt->my * 8 + 20;

		if (ntt->id == player->id)
		{
			SDL_RenderCopy(renderer, bomberman.get(), NULL, &r);
		}
		else if (ntt->id == computer->id)
		{
			SDL_RenderCopy(renderer, bombergirl.get(), NULL, &r);
		}

		//SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		//SDL_RenderFillRect(renderer, &r);
	});

	keys.Render(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

}

bool TestScene::Running()
{
	return true;
}
