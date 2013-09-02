#include <SDL_image.h>

#include "testscene.hpp"
#include "player.hpp"
#include "computer.hpp"
#include "block.hpp"
#include "bomb.hpp"
#include "explosion.hpp"
#include "printlog.hpp"

#include <algorithm>

#define BOMBID 3
#define EXLOSION 5

#define BOMBTIMER 3000
#define EXPLOSIONTIMER 200
#define EXPLOSIONSTAGES 4

using bomberman::bestiary::Player;
using bomberman::bestiary::Computer;
using bomberman::architecture::Block;
using bomberman::arsenal::Bomb;
using bomberman::arsenal::Explosion;

namespace bomberman {

TestScene::TestScene() : 
	_presentMap(new Map(15, 9)),
	_futurMap(new Map(15, 9))
{
	auto player = Player::Create("El Tuco"));
	player->x = 0;
	player->y = 0;
	_presentMap->SetEntity(player);

	auto computer = Computer::Create();
	computer->x = 14;
	computer->y = 0;
	_presentMap->SetEntity(computer);

	_presentMap->ForeachTile([&](int x, int y, const EntitySet &) {
		bool placeObstacle = (x & 1) & (y & 1);		
		auto blockEntity = Block::Create(placeObstacle ? Block::Obstacle : Block::Floor);
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
	_presentMap->ForeachEntity([&](const EntityPtr &entity)
	{
		entity->Evolve(inputs, now, __presentMap, _futurMap);
	});

	std::swap(_presentMap, _futurMap);
}

void TestScene::Render(SDL_Renderer *renderer)
{
	std::list<EntityPtr> entities;

	_presentMap->ForeachEntity([&](const EntityPtr &entity)
	{
		entities.push_back(entity);
	});

	entities.sort([](const EntityPtr &left, const EntityPtr &right) -> bool
	{
		return left->zlevel < right->zlevel;
	});

	for (auto entity : entities) 
	{
		entity->Render(renderer);
	}
}

bool TestScene::Running()
{
	return true;
}
	
}
