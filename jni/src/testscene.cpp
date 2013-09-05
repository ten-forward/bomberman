#include <SDL_image.h>

#include "testscene.hpp"
#include "player.hpp"
#include "computer.hpp"
#include "block.hpp"
#include "floortile.hpp"
#include "bomb.hpp"
#include "explosion.hpp"
#include "printlog.hpp"
#include "utils.hpp"

#include <algorithm>
#include <boost/foreach.hpp>

#define EXLOSION 5

#define EXPLOSIONTIMER 200
#define EXPLOSIONSTAGES 4

using bomberman::bestiary::Player;
using bomberman::bestiary::Computer;
using bomberman::architecture::Block;
using bomberman::architecture::FloorTile;
using bomberman::arsenal::Bomb;
using bomberman::arsenal::Explosion;

namespace bomberman {

TestScene::TestScene() : 
	_presentMap(new Map(MAP_COLUMNS, MAP_ROWS)),
	_futurMap(new Map(MAP_COLUMNS, MAP_ROWS))
{
	auto player = Player::Create("El Tuco");
	player->x = 0;
	player->y = 0;
	_presentMap->SetEntity(player);

	auto computer = Computer::Create();
	computer->x = 14;
	computer->y = 0;
	_presentMap->SetEntity(computer);

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

void TestScene::Update(const InputState& inputs, uint32_t now)
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
		BOOST_FOREACH (auto entity, entities) 
		{
			entity->Interact(inputs, now, iEntities);
		}
	});

	std::swap(_presentMap, _futurMap);
}

void TestScene::Render(SDL_Renderer *renderer)
{
	SDL_Rect safeArea = GetSafeArea1920();
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
