#pragma once

#include "entityptr.hpp"

// STL - libstdc++
#include <map>
#include <memory>
#include <unordered_set>

// boost
#include <boost/multi_array.hpp>

#define amountPerTile 8

namespace bomberman {

class Map;

typedef std::shared_ptr<Map> MapPtr;
typedef std::shared_ptr<const Map> MapConstPtr;

typedef std::unordered_set<EntityConstPtr> EntitySet;

class Map
{	
	public:
		
		enum PositionCheck
		{
			FREE,
			OCCUPIED,
			BOUNDARY
		};
		
		Map(int w, int h);

		const EntitySet &GetEntities(int x, int y) const;
		bool SetEntity(const EntityConstPtr &ntt);
		bool IsPointWithin(int x, int y) const;
		void Clear();
		
		void ForeachTile(std::function<void(int, int, const EntitySet &)> func) const;
		void ForeachEntity(std::function<void(const EntityConstPtr &)> func) const;

	private:
		int _width, _height;
		typedef boost::multi_array<EntitySet, 2> map_type;
		map_type _map;
};

}
