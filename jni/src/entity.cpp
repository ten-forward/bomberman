#include "entity.hpp"

namespace bomberman
{
	Entity::Entity() :
		dx(0),
		dy(0),
		x(0),
		y(0),
		zlevel(0),
		elevel(0),
		mx(0),
		my(0),
		active(true),
		brakes(true)
	{   
	}

	Entity::~Entity() 
	{
	}

}
