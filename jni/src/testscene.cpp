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
	map(15, 9),
	player(Player::Create("El Tuco")),
	computer(Computer::Create())
{
	player->x = 0;
	player->y = 0;

	computer->x = 14;
	computer->y = 0;

	Map& theMap = map;
	map.ForeachTile([&](int x, int y, const EntityPtr &tile) {
		bool placeObstacle = (x & 1) & (y & 1);		
		auto blockEntity = Block::Create(placeObstacle ? Block::Obstacle : Block::Floor);
		theMap.TrySetEntity(blockEntity, x, y);
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

template<typename T>
void RemoveWhere(std::list<T>* list, std::function<bool(T)> pred)
{
	RemoveAndProcessWhere<T>(list, pred, [&](T item){});
}

void CountWhile(int max, std::function<bool(int)> pred, std::function<void(int)> action)
{
	for (int i=1;i<=max;i++)
	{
		if (pred(i))
		{
			action(i);
		}
		else
		{
			break;
		}
	}
}

void TestScene::Update(const InputState& inputs, Uint32 now)
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
	else if (inputs.GetAButtonState())
	{
		// make sure there isn't already a bomb there
		/*bool alreadyBombed = false;
		BOOST_FOREACH(bombInfoPair bip, bombs)
		{
			if (player->x == bip.first->x && player->y == bip.first->y)
			{
				alreadyBombed = true;
				break;
			}
		};

		if (!alreadyBombed)
		{
			auto newBomb = Bomb::Create();
			newBomb->x = player->x;
			newBomb->y = player->y;
			newBomb->userdata = BOMBID;

			BombInfo info;
			info.strength = 2;
			info.timeout = now + BOMBTIMER;

			overlappingBombs.push_back(newBomb);
			bombs.push_back(bombInfoPair(newBomb,info));
		}*/
	}

	// check if player has left the position where a bomb was
	RemoveWhere<EntityPtr>(&overlappingBombs, [&](EntityPtr bomb)->bool
	{
		return map.TrySetEntity(bomb, bomb->x, bomb->y);
	});
	
	// BLOW THE BOMBS UP
	Map& theMap = map;
	/*RemoveAndProcessWhere<bombInfoPair>(&bombs, 
		[&](bombInfoPair bip)->bool
		{
			return now > bip.second.timeout;
		},

		[&](bombInfoPair bip)
		{
			// bomb's timer has run out. Erase bomb and add explosion
			if (theMap.GetEntity(bip.first->x, bip.first->y)->userdata == BOMBID)
			{
				theMap.RemoveEntity(bip.first->x, bip.first->y);
			}
			else
			{
				RemoveWhere<EntityPtr>(&overlappingBombs, [&](EntityPtr bomb)->bool
				{
					return bomb->x == bip.first->x && bomb->y == bip.first->y;
				});
			}

			// we don't add explosions to the map. rather we keep
			// an array of them around. anthing that overlaps with them
			// dies.
			ExplosionInfo explosion;
			explosion.stage = 0;
			explosion.timeout = now + EXPLOSIONTIMER;
			explosion.x = bip.first->x;
			explosion.y = bip.first->y;
			explosions.push_back(explosion);

			Map& aMap = theMap;
			std::function<bool(TestScene::ExplosionInfo)> isVulnerable = [&](TestScene::ExplosionInfo ex)->bool
			{
				auto status = aMap.CheckPosIsFree(ex.x, ex.y);
				return 
					(status == Map::OCCUPIED && typeid(aMap.GetEntity(ex.x, ex.y).get()) != typeid(Block)) ||
					(status == Map::FREE);
			};

			std::function<ExplosionInfo(int)> left  = [&](int dist)->ExplosionInfo { explosion.y = bip.first->y; explosion.x = bip.first->x - dist; return explosion; };
			std::function<ExplosionInfo(int)> right = [&](int dist)->ExplosionInfo { explosion.y = bip.first->y; explosion.x = bip.first->x + dist; return explosion; };
			std::function<ExplosionInfo(int)> up    = [&](int dist)->ExplosionInfo { explosion.x = bip.first->x; explosion.y = bip.first->y - dist; return explosion; };
			std::function<ExplosionInfo(int)> down  = [&](int dist)->ExplosionInfo { explosion.x = bip.first->x; explosion.y = bip.first->y + dist; return explosion; };
			
			std::list<ExplosionInfo>& theExplosions = explosions;
			CountWhile(bip.second.strength, 
				[&](int dist)->bool{ return isVulnerable(left(dist)); }, 
				[&](int dist) { theExplosions.push_back(left(dist)); });
			
			CountWhile(bip.second.strength, 
				[&](int dist)->bool{ return isVulnerable(right(dist)); }, 
				[&](int dist) { theExplosions.push_back(right(dist)); });

			CountWhile(bip.second.strength, 
				[&](int dist)->bool{ return isVulnerable(up(dist)); }, 
				[&](int dist) { theExplosions.push_back(up(dist)); });

			CountWhile(bip.second.strength, 
				[&](int dist)->bool{ return isVulnerable(down(dist)); }, 
				[&](int dist) { theExplosions.push_back(down(dist)); });
		});*/

	// process explosions.
	/*BOOST_FOREACH(ExplosionInfo& explosion, explosions)
	{
		if (now > explosion.timeout)
		{
			explosion.timeout = now + EXPLOSIONTIMER;
			explosion.stage++;
		}

		auto ntt = map.GetEntity(explosion.x, explosion.y);
		if (ntt)
		{
			// kill it!
		}
	};

	// remove expired explosions
	RemoveWhere<ExplosionInfo>(&explosions, [&](ExplosionInfo explosion)->bool
	{
		return explosion.stage > EXPLOSIONSTAGES;
	});


	// random "AI" player
	if (computer->mx == 0 && computer->my == 0)
	{
		computer->dx = rand() % 3 - 1;
		computer->dy = rand() % 3 - 1;
	}*/

	static Uint32 lastUpdate = SDL_GetTicks();
	if (now - lastUpdate > 20)
	{
		//printlog("drawing! %d", now);
		map.Update(1);
		lastUpdate = now;
	}
}

void TestScene::Render(SDL_Renderer *renderer)
{
	auto &entities(Entity::GetAllEntities());

	std::vector<Entity *> entityArray(entities.begin(), entities.end());
	std::sort(entityArray.begin(), entityArray.end(), [](const Entity *left, const Entity *right) -> bool
	{
		return left->zlevel < right->zlevel;
	});

	for (auto entity : entityArray) 
	{
		entity->Render(renderer);
	}
}

bool TestScene::Running()
{
	return true;
}

}