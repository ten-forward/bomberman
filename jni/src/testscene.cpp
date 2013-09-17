#include "testscene.hpp"
#include "player.hpp"
#include "block.hpp"
#include "bomb.hpp"
#include "softblock.hpp"
#include "floortile.hpp"
#include "printlog.hpp"
#include "constants.hpp"
#include "utils.hpp"

//SDL
#include <SDL_image.h>

//Boost
#include <boost/foreach.hpp>

using bomberman::bestiary::Player;
using bomberman::architecture::SoftBlock;
using bomberman::arsenal::Bomb;
using bomberman::architecture::Block;
using bomberman::architecture::FloorTile;

using namespace bomberman::constants;

namespace bomberman {

TestScene::TestScene(const PlayerConfigArray &playerConfig) : 
	_presentMap(new Map(MAP_COLUMNS, MAP_ROWS)),
	_pastMaps(1024),
	_playerConfig(playerConfig)
{
}

void TestScene::Init(SDL_Window* window, SDL_Renderer* renderer)
{
	auto player1 = Player::Create("Athos", "test/SaturnBomberman-BlackBomberman.PNG", 0, renderer);
	player1->x = 0;
	player1->y = 0;
	_presentMap->SetEntity(player1);

	auto player2 = Player::Create("Porthos", "test/honey.png", 1, renderer);
	player2->x = MAP_COLUMNS -1 ;
	player2->y = 0;
	_presentMap->SetEntity(player2);
	
	auto player3 = Player::Create("Aramis", "test/manji.png", 2, renderer);
	player3->x = 0;
	player3->y = MAP_ROWS - 1;
	_presentMap->SetEntity(player3);

	auto player4 = Player::Create("D'Artagnan", "test/whitebbman.png", 3, renderer);
	player4->x = MAP_COLUMNS -1 ;
	player4->y = MAP_ROWS - 1;
	_presentMap->SetEntity(player4);

	_presentMap->ForeachTile([&](int x, int y, const EntitySet &)
	{
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

	srand(1);

	for (int i=0;i<100;i++)
	{
		int x = rand() % MAP_COLUMNS;
		int y = rand() % MAP_ROWS;

		bool place = true;
		auto ntts = _presentMap->GetEntities(x,y);
		BOOST_FOREACH(auto ntt, ntts)
		{
			if (typeid(*ntt) != typeid(FloorTile))
			{
				place = false;
				break;
			}
		}
		if (place)
		{
			auto softblock = SoftBlock::Create();
			softblock->x = x;
			softblock->y = y;
			_presentMap->SetEntity(softblock);
		}
	}
}

void TestScene::Update(const std::vector<InputState>& inputs, uint32_t now)
{
	MapPtr futurMap(new Map(MAP_COLUMNS, MAP_ROWS));

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
		entity->Evolve(inputs, now, _presentMap, futurMap);
	}

	entities.clear();

	futurMap->ForeachTile([&](int x, int y, const EntitySet &iEntities)
	{	
		BOOST_FOREACH (auto entity, iEntities) 
		{
			entity->Interact(inputs, now, iEntities);
		}
	});

	_pastMaps.push_front(std::make_pair(now, _presentMap));
	_presentMap = futurMap;
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
		return left->zlevel == right->zlevel ? 

			(left->y == right->y ? (typeid(*left) == typeid(Bomb) && typeid(*right) == typeid(Player)) : left->y < right->y)
			
			: left->zlevel < right->zlevel;
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
	

void TestScene::BackThroughTime(SDL_Renderer *renderer, uint32_t now)
{
	while (!_pastMaps.empty())
	{
		auto gameState = _pastMaps.front();
		_pastMaps.pop_front();

		uint32_t duration = now - gameState.first;

		_presentMap = gameState.second;

		if (duration > 12)
		{
			now = gameState.first;
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			SDL_RenderClear(renderer);
			Render(renderer);
			SDL_RenderPresent(renderer);
		}
	}
}

}
