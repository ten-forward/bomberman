#include "gamescene.hpp"
#include "player.hpp"
#include "block.hpp"
#include "bomb.hpp"
#include "propbomb.hpp"
#include "softblock.hpp"
#include "floortile.hpp"
#include "printlog.hpp"
#include "constants_resolution.hpp"
#include "constants_game.hpp"
#include "utils.hpp"
#include "printlog.hpp"
#include "computer.hpp"
#include "umpire.hpp"

#include <iomanip>

//SDL
#include <SDL_image.h>

//Boost
#include <boost/foreach.hpp>

using bomberman::bestiary::Player;
using bomberman::bestiary::PlayerPtr;
using bomberman::bestiary::Computer;
using bomberman::architecture::SoftBlock;
using bomberman::arsenal::Bomb;
using bomberman::arsenal::PropBomb;
using bomberman::architecture::Block;
using bomberman::architecture::FloorTile;

using namespace bomberman::constants;

namespace bomberman {

GameScene::GameScene(const PlayerConfigArray &playerConfig) : 
	_music(Mix_LoadMUS("music/premonition.flac"), Mix_FreeMusic),
	_presentMap(new Map(MAP_COLUMNS, MAP_ROWS)),
	_playerConfig(playerConfig),
	_pastMaps(10),
	_font(utils::LoadFont("drawable/Gamegirl.ttf", 40))
    
{
    
	if(!_music)
	{
		printlog("Mix_LoadMUS: %s\n", Mix_GetError());
	}
}

void GameScene::Init(SDL_Window* window, SDL_Renderer* renderer)
{
    _running = true;

	//if(Mix_PlayMusic(_music.get(), -1) == -1)
	{
		printlog("Mix_PlayMusic: %s\n", Mix_GetError());
	}
	_background = utils::LoadTexture(renderer, "drawable/gameback.png");

	InitPlayers(renderer);
	InitBlocks(renderer);
}

void GameScene::InitPlayers(SDL_Renderer* renderer)
{

	auto umpire = Umpire::Create();
	_presentMap->SetEntity(umpire);

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

	PlayerId firstPlayerId = 132;

	for (int i=0;i<4;i++)
	{
		PlayerId id = firstPlayerId + i;
		if (_playerConfig[i].present)
		{
			if (_playerConfig[i].isComputer)
			{
				auto player = Computer::Create(id,_playerConfig[i].name, _playerConfig[i].spriteName, _playerConfig[i].aiScript, i, renderer);
				player->SetX(pos[i].x);
				player->SetY(pos[i].y);
				_presentMap->SetEntity(player);
			}
			else
			{
				auto player = Player::Create(id, _playerConfig[i].name, _playerConfig[i].spriteName, i, renderer);
                player->SetX(pos[i].x);
				player->SetY(pos[i].y);
				_presentMap->SetEntity(player);
			}
			umpire->NotifyPlayerBorn(id);
			_playerIds[i] = id;
		}
		else
		{
			_playerIds[i] = 0;
		}
	}
}

void GameScene::InitBlocks(SDL_Renderer* renderer)
{

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
		blockEntity->SetX(x);
		blockEntity->SetY(y);
		_presentMap->SetEntity(blockEntity);
	});

	for (int i=0; i < 100; i++)
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
			auto softblock = SoftBlock::Create(0.8);
			softblock->SetX(x);
			softblock->SetY(y);
			_presentMap->SetEntity(softblock);
		}
	}
}

void GameScene::Update(const std::vector<InputState>& inputs, uint32_t now)
{
	if (inputs[0].GetButtonState(InputState::START)) 
	{
		return BackThroughTime();
	}

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

	auto umpire = std::static_pointer_cast<Umpire>(_presentMap->GetEntity(constants::UMPIRE));
	if (umpire->HasGameEnded())
	{
		if (umpire->GetPlayersRemaining() == 1)
		{
			_victor = std::static_pointer_cast<Player>(_presentMap->GetEntity(umpire->GetRemainingPlayer()))->GetPlayerIndex();
		}
		else if (umpire->GetPlayersRemaining() == 0)
		{
			_victor = constants::NO_PLAYER;
		}

		_running = false;
	}
}

void GameScene::Render(SDL_Renderer *renderer)
{

        
	SDL_Rect safeArea = bomberman::utils::GetSafeArea1920();
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &safeArea);
	
	SDL_Rect r;
	r.w = SCREEN_WIDTH;
	r.h = SCREEN_HEIGHT;
	r.x = 0;
	r.y = 0;

	SDL_RenderCopy(renderer, _background.get(), NULL, &r);

	for (int i = 0; i < 4; ++i)
	{
		auto player = std::dynamic_pointer_cast<Player>(_presentMap->GetEntity(_playerIds[i]));
		if (player)
		{
			RenderPlayerDashBoard(player, i, renderer);
		}
	} 

	std::list<EntityConstPtr> entities;

	_presentMap->ForeachEntity([&](const EntityConstPtr &entity)
	{
		entities.push_back(entity);
	});

	entities.sort([](const EntityConstPtr &left, const EntityConstPtr &right) -> bool
	{
		bool bomb = typeid(*left) == typeid(Bomb) || typeid(*left) == typeid(PropBomb);
		return left->zlevel == right->zlevel ? 
					(left->GetY() == right->GetY() ?
						bomb && typeid(*right) == typeid(Player) 
					: 	left->GetY() < right->GetY())
				: 	left->zlevel < right->zlevel;
	});

	BOOST_FOREACH (auto entity, entities) 
	{
		if (entity->active)
		{
			entity->Render(renderer);
		}
	}
    
}

void GameScene::RenderPlayerDashBoard(const PlayerPtr &iPlayer, int pos, SDL_Renderer* renderer)
{
	SDL_Rect dashboard;
	dashboard.w = PLAYER_DASHBOARD_WIDTH;
	dashboard.h = PLAYER_DASHBOARD_HEIGHT;
	dashboard.x = PLAYER_DASHBOARD_X + pos * (dashboard.w + PLAYER_DASHBOARD_PADDING);
	dashboard.y = PLAYER_DASHBOARD_Y;

	//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	//SDL_RenderDrawRect(renderer, &dashboard);

	SDL_Rect avatar;
	avatar.w = PLAYER_WIDTH;
	avatar.h = PLAYER_HEIGHT;
	avatar.x = dashboard.x + PLAYER_DASHBOARD_AVATAR_X;
	avatar.y = dashboard.y - PLAYER_DASHBOARD_AVATAR_Y;

	iPlayer->Render(renderer, avatar);

	auto umpire = std::static_pointer_cast<Umpire>(_presentMap->GetEntity(constants::UMPIRE));

	std::stringstream ss;
	ss << std::setw( 2 ) << std::setfill( '0' ) << (iPlayer->GetAllowedNumberOfBombs() - umpire->GetBombCount(iPlayer->id));
	auto nbBombsImg = utils::DrawString(renderer, _font, ss.str(), utils::MakeColor(0xffffffff));

	SDL_Rect nbBombsRect;
	nbBombsRect.w = 100;
	nbBombsRect.h = PLAYER_DASHBOARD_HEIGHT;
	nbBombsRect.x = avatar.x + avatar.w + PLAYER_DASHBOARD_PADDING;
	nbBombsRect.y = dashboard.y + PLAYER_DASHBOARD_PADDING;

	SDL_RenderCopy(renderer, nbBombsImg.get(), NULL, &nbBombsRect);
}

bool GameScene::Running()
{
	if (!_running)
	{
		// Scene has finished! We should stop the music.
		Mix_HaltMusic();
		return false;
	}

	return true;
}
	

void GameScene::BackThroughTime()
{
	if (!_pastMaps.empty())
	{
		auto gameState = _pastMaps.front();
		_pastMaps.pop_front();
		_presentMap = gameState.second;	
	}
}

}
