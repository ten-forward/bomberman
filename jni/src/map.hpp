#ifndef MAP_HPP
#define MAP_HPP

#include <memory>
#include <boost/tr1/functional.hpp>
#include <boost/multi_array.hpp>
#include <boost/foreach.hpp>
#include <map>

#define amountPerTile 8

class Map
{	
	struct Entity
	{
		int id;
		int dx;	// speed
		int dy;
		int x;	// tile position
		int y;
		int mx;	// sub-tile position
		int my;
		bool active; // is on the map?
		bool brakes; // brake on tile?
		int userdata;
	};
public:
	typedef std::shared_ptr<Entity> entity_type;

private:
	typedef boost::multi_array<entity_type, 2> map_type;
	typedef std::map<int, std::weak_ptr<Entity> > registry_type;

	int ids;
	map_type map;
	registry_type reg;

public:

	enum PositionCheck
	{
		FREE,
		OCCUPIED,
		BOUNDARY
	};
	
	Map(int w, int h);

	~Map();

	entity_type CreateEntity();
	entity_type GetEntity(int x, int y);
	entity_type GetEntity(int id);
	bool TrySetEntity(const entity_type &ntt, int x, int y);
	entity_type RemoveEntity(int x, int y);
	PositionCheck CheckPosIsFree(int x, int y);
	void Update(int timeSteps);
	void ForeachTile(std::function<void(int,int,const std::shared_ptr<Entity> &)> func);
	void ForeachEntity(std::function<void(const std::shared_ptr<Entity> &)> func);
};

#endif // MAP_HPP
