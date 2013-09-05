#include <SDL_image.h>

#include "testscene.hpp"
#include "player.hpp"
#include "computer.hpp"
#include "block.hpp"
#include "floortile.hpp"
#include "bomb.hpp"
#include "explosion.hpp"
#include "printlog.hpp"

#include <algorithm>

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
	_presentMap(new Map(15, 9)),
	_futurMap(new Map(15, 9))
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

TestScene::~TestScene()
{
}

void TestScene::Init(SDL_Window* window, SDL_Renderer* renderer)
{
}

template<typename T>
void RemoveAndProcessWhere(std::list<T>* list, std::function<bool(T)> pred, std::function<void(T)> action)
{
	typename std::list<T>::iterator i = list->begin();
	while (i != list->end())
	{
		T item = *i;
		if (pred(item))
		{
			// if the item is there then erase it
			list->erase(i++);
			action(item);
		}
		else
		{
			// otherwise leave it
			++i;
		}
	}
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

	for (auto entity : entities) 
	{
		entity->Evolve(inputs, now, _presentMap, _futurMap);
	}

	entities.clear();

	_futurMap->ForeachTile([&](int x, int y, const EntitySet &iEntities)
	{	
		for (auto entity : iEntities)
		{
			entity->Interact(inputs, now, iEntities);
		}
	});

	std::swap(_presentMap, _futurMap);
}

void TestScene::Render(SDL_Renderer *renderer)
{
	std::list<EntityConstPtr> entities;

	_presentMap->ForeachEntity([&](const EntityConstPtr &entity)
	{
		entities.push_back(entity);
	});

	entities.sort([](const EntityConstPtr &left, const EntityConstPtr &right) -> bool
	{
		return left->zlevel < right->zlevel;
	});

	for (auto entity : entities) 
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
