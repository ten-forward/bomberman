
#include "map.hpp"
#include "entity.hpp"
#include "printlog.hpp"

#include <iostream>

namespace bomberman {

Map::Map(int w, int h) : map(boost::extents[w][h])
{
}

const EntitySet &Map::GetEntities(int x, int y) const
{
	return map[x][y];
}

bool Map::SetEntity(const EntityPtr &ntt)
{
	typedef map_type::index Index;
	Index xMin = map.index_bases()[0];
	Index xMax = xMin + map.shape()[0] - 1;
	Index yMin = map.index_bases()[1];
	Index yMax = yMin + map.shape()[1] - 1;

	int x = ntt->x;
	int y = ntt->y;

	// boundaries
	if (x < xMin || x > xMax || y < yMin || y > yMax)
	{
		return false;
	}
	
	map[x][y].insert(ntt);

	return true;
}

/*Map::PositionCheck Map::CheckPosIsFree(int x, int y)
{
	typedef map_type::index index;
	index xMin = map.index_bases()[0];
	index xMax = xMin + map.shape()[0] - 1;
	index yMin = map.index_bases()[1];
	index yMax = yMin + map.shape()[1] - 1;

	// boundaries
	if (x < xMin || x > xMax || y < yMin || y > yMax)
	{
		return BOUNDARY;
	}

	// occupied?
	if (map[x][y])
	{
		return OCCUPIED;
	}

	return FREE;
}*/

int sign(int x)
{
	return (x > 0) - (x < 0);
}

#define PREVENT_DIAGONAL_MOVEMENT \
		if (ntt->mx != 0 && ntt->dy != 0)\
		{\
			ntt->dy = 0;\
		}\
		\
		if (ntt->my != 0 && ntt->dx != 0)\
		{\
			ntt->dx = 0;\
		}\
		\
		if (ntt->mx == 0 && ntt->my == 0)\
		{\
			if (ntt->dx && ntt->dy)\
			{\
				ntt->dy = 0;\
				ntt->dx = 0;\
				continue;\
			}\
		}

/*void Map::Update(int timeSteps)
{
	BOOST_FOREACH(registry_type::value_type iter, reg)
	{
		auto ntt = iter.second.lock();
		if (!ntt)
		{
			continue;
		}

		if (!ntt->active)
		{
			// entity is not on the map
			continue;
		}

		PREVENT_DIAGONAL_MOVEMENT;

		int dx = ntt->dx;
		int dy = ntt->dy;

		if (dx == 0 && dy == 0)
		{
			// object not moving, no need to simulate
			continue;
		}
		
		if (ntt->mx == 0 && ntt->my == 0)
		{
			// object moving from flush
			int xprime = ntt->x + sign(dx);
			int yprime = ntt->y + sign(dy);

			auto result = CheckPosIsFree(xprime, yprime);

			if (result == FREE)
			{
				int mxprime = ntt->mx + sign(dx);
				int myprime = ntt->my + sign(dy);

				// occupy 2 tiles at once.
				map[xprime][yprime] = ntt;

				ntt->mx = mxprime;
				ntt->my = myprime;
			}
			else
			{
				if (result == OCCUPIED)
				{
					//printlog("collision of %d with %d!", ntt->id, map[xprime][yprime]->id);
				}

				// cannot move!
				ntt->dx = 0;
				ntt->dy = 0;
			}
		}
		else
		{
			// check if we just reversed!
			// if we did we need to fix
			// the values to avoid
			// leaving block trails on the map
			if (sign(dx) != sign(ntt->mx))
			{
				ntt->x += sign(ntt->mx);
				ntt->mx = -sign(ntt->mx) * (amountPerTile - abs(ntt->mx));
			}

			if (sign(dy) != sign(ntt->my))
			{
				ntt->y += sign(ntt->my);
				ntt->my = -sign(ntt->my) * (amountPerTile - abs(ntt->my));
			}

			// calculate where we will be
			int mxprime = ntt->mx + sign(dx);
			int myprime = ntt->my + sign(dy);

			if (abs(mxprime) >= amountPerTile || abs(myprime) >= amountPerTile)
			{
				// completing transition
				int xprime = ntt->x + sign(ntt->mx);
				int yprime = ntt->y + sign(ntt->my);
				
				// clear previous tile
				map[ntt->x][ntt->y] = std::shared_ptr<Entity>();

				ntt->x = xprime;
				ntt->y = yprime;
				ntt->mx = 0;
				ntt->my = 0;

				if (ntt->brakes)
				{
					ntt->dx = 0;
					ntt->dy = 0;
				}
			}
			else
			{

				// simple move
				ntt->mx = mxprime;
				ntt->my = myprime;
			}

		}
	}
}*/

void Map::ForeachTile(std::function<void(int, int, const EntitySet &)> func) const
{
	typedef map_type::index Index;
	Index xMin = map.index_bases()[0];
	Index xMax = xMin + map.shape()[0];
	Index yMin = map.index_bases()[1];
	Index yMax = yMin + map.shape()[1];
	for (int x = xMin; x < xMax; x++)
	{
		for (int y = yMin; y < yMax; y++)
		{
			func(x, y, map[x][y]);
		}
	}
}

void Map::ForeachEntity(std::function<void(const EntityPtr &)> func) const
{
	for(auto xbucket : map)
	{
		for (auto xybucket : xbucket)
		{
			for (auto entity : xybucket)
			{
				func(entity);
			}
		}
	}
}

}