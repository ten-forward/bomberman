#include <SDL_image.h>

#include "testscene.hpp"

#include "printlog.hpp"

#define BLOCKID 999
#define BOMBID 3
#define EXLOSION 5

#define BOMBTIMER 3000
#define EXPLOSIONTIMER 200
#define EXPLOSIONSTAGES 4

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

	explosionSprite[0] = std::tr1::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, "test/explosion1.png"), SDL_DestroyTexture);
	explosionSprite[1] = std::tr1::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, "test/explosion2.png"), SDL_DestroyTexture);
	explosionSprite[2] = std::tr1::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, "test/explosion3.png"), SDL_DestroyTexture);
	explosionSprite[3] = std::tr1::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, "test/explosion4.png"), SDL_DestroyTexture);
}

template<typename T>
void RemoveAndProcessWhere(std::list<T>* list, std::tr1::function<bool(T)> pred, std::tr1::function<void(T)> action)
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
void RemoveWhere(std::list<T>* list, std::tr1::function<bool(T)> pred)
{
	RemoveAndProcessWhere<T>(list, pred, [&](T item){});
}

void CountWhile(int max, std::tr1::function<bool(int)> pred, std::tr1::function<void(int)> action)
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
#ifdef ANDROID
	keys.Update(inputs, now);
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
	else if (inputs.GetAButtonState())
	{
		// make sure there isn't already a bomb there
		bool alreadyBombed = false;
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
			auto newBomb = map.CreateEntity();
			newBomb->x = player->x;
			newBomb->y = player->y;
			newBomb->userdata = BOMBID;

			BombInfo info;
			info.strength = 2;
			info.timeout = now + BOMBTIMER;

			overlappingBombs.push_back(newBomb);
			bombs.push_back(bombInfoPair(newBomb,info));
		}
	}

	// check if player has left the position where a bomb was
	RemoveWhere<Map::entity_type>(&overlappingBombs, [&](Map::entity_type bomb)->bool
	{
		return map.TrySetEntity(bomb, bomb->x, bomb->y);
	});

	//std::list<Map::entity_type>::iterator i = overlappingBombs.begin();
	//while (i != overlappingBombs.end())
	//{
	//	auto bomb = *i;
	//	if (map.TrySetEntity(bomb, bomb->x, bomb->y))
	//	{
	//		// if the player has left the tile, we can erase it from this list
	//		overlappingBombs.erase(i++);
	//	}
	//	else
	//	{
	//		// otherwise leave it there until it explodes
	//		++i;
	//	}
	//}
	
	// BLOW THE BOMBS UP
	Map& theMap = map;
	RemoveAndProcessWhere<bombInfoPair>(&bombs, 
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
				RemoveWhere<Map::entity_type>(&overlappingBombs, [&](Map::entity_type bomb)->bool
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
			std::tr1::function<bool(TestScene::ExplosionInfo)> isVulnerable = [&](TestScene::ExplosionInfo ex)->bool
			{
				auto status = aMap.CheckPosIsFree(ex.x, ex.y);
				return 
					(status == Map::OCCUPIED && aMap.GetEntity(ex.x, ex.y)->userdata != BLOCKID) ||
					(status == Map::FREE);
			};

			std::tr1::function<ExplosionInfo(int)> left  = [&](int dist)->ExplosionInfo { explosion.y = bip.first->y; explosion.x = bip.first->x - dist; return explosion; };
			std::tr1::function<ExplosionInfo(int)> right = [&](int dist)->ExplosionInfo { explosion.y = bip.first->y; explosion.x = bip.first->x + dist; return explosion; };
			std::tr1::function<ExplosionInfo(int)> up    = [&](int dist)->ExplosionInfo { explosion.x = bip.first->x; explosion.y = bip.first->y - dist; return explosion; };
			std::tr1::function<ExplosionInfo(int)> down  = [&](int dist)->ExplosionInfo { explosion.x = bip.first->x; explosion.y = bip.first->y + dist; return explosion; };
			
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
		});

	// process explosions.
	BOOST_FOREACH(ExplosionInfo& explosion, explosions)
	{
		if (now > explosion.timeout)
		{
			explosion.timeout = now + EXPLOSIONTIMER;
			explosion.stage++;
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
	}

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
	
	// draw the bombs first so they appear underneath people
	BOOST_FOREACH(bombInfoPair bip, bombs)
	{
		SDL_Rect r;
		r.w = 64;
		r.h = 64;
		r.x = bip.first->x * r.w + bip.first->mx * 8 + 20;
		r.y = bip.first->y * r.h + bip.first->my * 8 + 20;

		SDL_RenderCopy(renderer, bomb.get(), NULL, &r);
	};

	// draw the people and other things
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

	// draw the explosions
	BOOST_FOREACH(ExplosionInfo explosion, explosions)
	{
		SDL_Rect r;
		r.w = 64;
		r.h = 64;
		r.x = explosion.x * r.w + 20;
		r.y = explosion.y * r.h + 20;
		
		SDL_RenderCopy(renderer, explosionSprite[explosion.stage].get(), NULL, &r);
	};

	keys.Render(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

}

bool TestScene::Running()
{
	return true;
}
