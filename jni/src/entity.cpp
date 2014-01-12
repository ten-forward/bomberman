#include "entity.hpp"
#include "constants_game.hpp"

namespace bomberman
{
	Entity::Entity() :
		id(0),
		dx(0),
		dy(0),
		zlevel(0),
		elevel(constants::DEFAULT_ELEVEL),
		mx(0),
		my(0),
		active(true),
		brakes(true)	{   
	}

	Entity::~Entity() 
	{
	}
}
