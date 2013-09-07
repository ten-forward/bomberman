#include "testscene.hpp"
#include "player.hpp"
#include "block.hpp"
#include "floortile.hpp"
#include "printlog.hpp"
#include "utils.hpp"

//SDL
#include <SDL_image.h>

//Boost
#include <boost/foreach.hpp>

using bomberman::bestiary::Player;
using bomberman::architecture::Block;
using bomberman::architecture::FloorTile;

using namespace bomberman::constants;

namespace bomberman {

TestScene::TestScene() : 
	_presentMap(new Map(MAP_COLUMNS, MAP_ROWS)),
	_futurMap(new Map(MAP_COLUMNS, MAP_ROWS))
{
	auto player1 = Player::Create("Athos", 0);
	player1->x = 0;
	player1->y = 0;
	_presentMap->SetEntity(player1);

	auto player2 = Player::Create("Porthos", 1);
	player2->x = MAP_COLUMNS -1 ;
	player2->y = 0;
	_presentMap->SetEntity(player2);
	
	auto player3 = Player::Create("Aramis", 2);
	player3->x = 0;
	player3->y = MAP_ROWS - 1;
	_presentMap->SetEntity(player3);

	auto player4 = Player::Create("D'Artagnan", 3);
	player4->x = MAP_COLUMNS -1 ;
	player4->y = MAP_ROWS - 1;
	_presentMap->SetEntity(player4);

	_presentMap->ForeachTile([&](int x, int y, const EntitySet &) {
		bool placeObstacle = (x & 1) & (y & 1);		
		EntityPtr blockEntity;
		if (placeObstacle)
		{
			blockEntity = Block::Create();
		}
		else 
		{
		 	blockEntity = FloorTile::Create();
		}
		blockEntity->x = x;
		blockEntity->y = y;
		_presentMap->SetEntity(blockEntity);
	});
}

void TestScene::Init(SDL_Window* window, SDL_Renderer* renderer)
{
}

void TestScene::Update(const std::vector<InputState>& inputs, uint32_t now)
{
	_futurMap->Clear();

	std::list<EntityConstPtr> entities;

	_presentMap->ForeachEntity([&](const EntityConstPtr &entity)
	{
		entities.push_back(entity);
	});

	entities.sort([](const EntityConstPtr &left, const EntityConstPtr &right) -> bool
	{
		return left->elevel < right->elevel;
	});

	BOOST_FOREACH (auto entity, entities) 
	{
		entity->Evolve(inputs, now, _presentMap, _futurMap);
	}

	entities.clear();

	_futurMap->ForeachTile([&](int x, int y, const EntitySet &iEntities)
	{	
		BOOST_FOREACH (auto entity, iEntities) 
		{
			entity->Interact(inputs, now, iEntities);
		}
	});

	std::swap(_presentMap, _futurMap);
}

void TestScene::Render(SDL_Renderer *renderer)
{
	SDL_Rect safeArea = bomberman::utils::GetSafeArea1920();
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &safeArea);

	std::list<EntityConstPtr> entities;

	_presentMap->ForeachEntity([&](const EntityConstPtr &entity)
	{
		entities.push_back(entity);
	});

	entities.sort([](const EntityConstPtr &left, const EntityConstPtr &right) -> bool
	{
		return left->zlevel < right->zlevel;
	});

	BOOST_FOREACH (auto entity, entities) 
	{
		if (entity->active)
		{
			entity->Render(renderer);
		}
	}
}

bool TestScene::Running()
{
	return true;
}
	
}
