#include "bomb.hpp"

// SDL
#include <SDL_image.h>

namespace bomberman {
namespace arsenal {

	BombPtr Bomb::Create(int iTimeout, int iStrength) {
		auto bomb = std::make_shared<Bomb>();
		bomb->_timeout = iTimeout;
		bomb->_strength = iStrength;
		bomb->zlevel = 1;
		return bomb;
	}

	std::shared_ptr<SDL_Texture>  Bomb::_Bomb;

	void Bomb::InitializeGraphicRessources(SDL_Renderer *iRenderer) 
	{
		_Bomb = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(iRenderer, "test/bomb.png"), SDL_DestroyTexture);
	}

	void Bomb::Evolve(const InputState& /*iInputs*/, uint32_t /*iTimestamp*/, const MapConstPtr &/*iPresentMap*/, const MapPtr &/*iFutureMap*/) const
	{
		// BLOW THE BOMBS UP
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
	}

	void Bomb::Render(SDL_Renderer *iRenderer) const 
	{
		if (!_Bomb) 
		{
			InitializeGraphicRessources(iRenderer);
		}

		SDL_Rect r;
		r.w = 64;
		r.h = 64;
		r.x = x * r.w + mx * 8 + 20;
		r.y = y * r.h + my * 8 + 20;

		SDL_RenderCopy(iRenderer, _Bomb.get(), NULL, &r);
	}
}
}