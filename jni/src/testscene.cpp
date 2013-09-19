#include "testscene.hpp"
#include "block.hpp"
#include "bomb.hpp"
#include "softblock.hpp"
#include "floortile.hpp"
#include "printlog.hpp"
#include "constants.hpp"
#include "utils.hpp"
#include "printlog.hpp"

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
	_music(Mix_LoadMUS("music/premonition.flac"), Mix_FreeMusic),
	_presentMap(new Map(MAP_COLUMNS, MAP_ROWS)),
	_pastMaps(1024),
	_playerConfig(playerConfig)
{
}

void TestScene::Init(SDL_Window* window, SDL_Renderer* renderer)
{
	_running = true;

	struct PlayerPositions
	{
		int x,y;
	};

	PlayerPositions pos[] =
	{
		{0,0},
		{MAP_COLUMNS -1,0},
		{0,MAP_ROWS - 1},
		{MAP_COLUMNS - 1,MAP_ROWS - 1},
	};

	for (int i=0;i<4;i++)
	{
		if (_playerConfig[i].present)
		{
			auto player = Player::Create(_playerConfig[i].name, _playerConfig[i].spriteName, i, renderer, &_playerConfig[i].present);
			player->x = pos[i].x;
			player->y = pos[i].y;
			_presentMap->SetEntity(player);

			_players[i] = player;
		}
	}

	if(Mix_PlayMusic(_music.get(), -1) == -1)
	{
		printlog("Mix_PlayMusic: %s\n", Mix_GetError());
	}

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
	
	_texture = utils::LoadTexture(renderer, "test/gameback.png");

	srand(1);

	for (int i=0;i<100;i++)
	{
		int x = rand() % MAP_COLUMNS;
		int y = rand() % MAP_ROWS;

		// avoid locking a player in
		if ((x < PLAYER_SPACE && y < PLAYER_SPACE) || 
			(x < PLAYER_SPACE && y >= MAP_ROWS - PLAYER_SPACE) ||
			(x >= MAP_COLUMNS - PLAYER_SPACE && y >= MAP_ROWS-PLAYER_SPACE) ||
			(x >= MAP_COLUMNS - PLAYER_SPACE && y < PLAYER_SPACE)
			)
		{
			continue;
		}

		if (_presentMap->CheckPosition(x,y) == Map::FREE)
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

	int lastPlayer = -1;
	int count = 0;
	for (int i=0;i<4;i++)
	{
		if ( _playerConfig[i].present )
		{
			lastPlayer = i;
			count++;
		}
	}

	if (count < 2)
	{
		_running = false;
	}

	_victor = lastPlayer;
}

void TestScene::Render(SDL_Renderer *renderer)
{
	SDL_Rect safeArea = bomberman::utils::GetSafeArea1920();
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &safeArea);
	
	SDL_Rect r;
	r.w = SCREEN_WIDTH;
	r.h = SCREEN_HEIGHT;
	r.x = 0;
	r.y = 0;

	SDL_RenderCopy(renderer, _texture.get(), NULL, &r);

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
	if (!_running)
	{
		// Scene has finished! We should stop the music.
		Mix_HaltMusic();
		return false;
	}

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
