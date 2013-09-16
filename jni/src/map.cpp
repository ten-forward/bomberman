
#include "map.hpp"
#include "entity.hpp"
#include "printlog.hpp"

#include <iostream>
#include <boost/foreach.hpp>

namespace bomberman {

Map::Map(int w, int h) : 
	_width(w),
	_height(h),
	_map(boost::extents[_width][_height])
{
}

bool Map::IsPointWithin(int x, int y) const
{
	return x >= 0 && x < _width && y >= 0 && y < _height;
}

const EntitySet &Map::GetEntities(int x, int y) const
{
	return _map[x][y];
}

EntitySet &Map::GetEntities(int x, int y)
{
	return _map[x][y];
}

bool Map::SetEntity(const EntityPtr &ntt)
{
	int x = ntt->x;
	int y = ntt->y;

	if (!IsPointWithin(x, y))
	{
		return false;
	}
	
	_map[x][y].insert(ntt);

	return true;
}

void Map::Clear()
{
	ForeachTile([&](int, int, const EntitySet &tile)
	{
		const_cast<EntitySet &>(tile).clear();
	});
}

void Map::ForeachTile(std::function<void(int, int, const EntitySet &)> func) const
{
	for (int x = 0; x < _width; x++)
	{
		for (int y = 0; y < _height; y++)
		{
			func(x, y, _map[x][y]);
		}
	}
}

void Map::ForeachEntity(std::function<void(const EntityConstPtr &)> func) const
{
	ForeachTile([&](int, int, const EntitySet &entities)
	{
		BOOST_FOREACH (auto entity, entities)
		{
			func(entity);
		}
	});
}

void Map::ForeachEntity(std::function<void(const EntityPtr &)> func)
{
	ForeachTile([&](int, int, const EntitySet &entities)
	{
		BOOST_FOREACH (auto entity, entities)
		{
			func(entity);
		}
	});
}

}
