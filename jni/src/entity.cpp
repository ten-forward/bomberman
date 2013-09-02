#include "entity.hpp"

namespace bomberman
{
	Entity::Entity() :
		dx(0),
		dy(0),
		x(0),
		y(0),
		zlevel(0),
		mx(0),
		my(0),
		active(false),
		brakes(true),
		userdata(0)
	{
		//ntt->id = ids++;
		//reg[ntt->id] = ntt;
	}

	Entity::~Entity() 
	{
	}

}