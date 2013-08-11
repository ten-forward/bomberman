#ifndef MAP_HPP
#define MAP_HPP

#include "entity.hpp"

// STL - libstdc++
#include <map>
#include <memory>

// boost
#include <boost/tr1/functional.hpp>
#include <boost/multi_array.hpp>
#include <boost/foreach.hpp>

#define amountPerTile 8

namespace bomberman {

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

		EntityPtr GetEntity(int x, int y);
		EntityPtr GetEntity(int id);
		bool TrySetEntity(const EntityPtr &ntt, int x, int y);
		EntityPtr RemoveEntity(int x, int y);
		PositionCheck CheckPosIsFree(int x, int y);
		void Update(int timeSteps);
		void ForeachTile(std::function<void(int,int,const EntityPtr &)> func);
		void ForeachEntity(std::function<void(const EntityPtr &)> func);

	private:
		typedef boost::multi_array<EntityPtr, 2> map_type;
		typedef std::map<int, std::weak_ptr<Entity> > registry_type;

		int ids;
		map_type map;
		registry_type reg;
};

}

#endif // MAP_HPP
