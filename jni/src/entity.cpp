#include "entity.hpp"

namespace bomberman
{
	Entity::Entity() :
		x(0),
		y(0),
		zlevel(0),
		elevel(0),
		mx(0),
		my(0),
		active(false),
		brakes(true)
	{   
	}

	Entity::~Entity() 
	{
	}

}
