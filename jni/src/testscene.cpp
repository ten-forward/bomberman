#include "testscene.hpp"


TestScene::TestScene() : 
	map(10, 10),
	player(map.CreateEntity())
{
	map.TrySetEntity(player, 0, 0);
}

TestScene::~TestScene()
{

}

void TestScene::Init(SDL_Window* window, SDL_Renderer* renderer)
{

}

void TestScene::Update(const InputState& inputs)
{
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

	static Uint32 lastUpdate = SDL_GetTicks();
	Uint32 now = SDL_GetTicks();
	if (now - lastUpdate > 50)
	{
		map.Update(1);
		lastUpdate = now;
	}
}

void TestScene::Render(SDL_Renderer *renderer)
{
	map.ForeachTile([&](int x, int y, Map::entity_type tile)
	{
		SDL_Rect r;
		r.w = 20;
		r.h = 20;
		r.x = x * r.w + 20;	// <- just for overscan
		r.y = y * r.h + 20;

		if (!tile)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
			SDL_RenderDrawRect(renderer, &r);
		}
		else
		{
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
			SDL_RenderFillRect(renderer, &r);
		}
	});

	map.ForeachEntity([&](Map::entity_type ntt)
	{
		SDL_Rect r;
		r.w = 20;
		r.h = 20;
		r.x = ntt->x * r.w + ntt->mx * 2 + 20;
		r.y = ntt->y * r.h + ntt->my * 2 + 20;

		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);
		SDL_RenderFillRect(renderer, &r);
	});

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

bool TestScene::Running()
{
	return true;
}
