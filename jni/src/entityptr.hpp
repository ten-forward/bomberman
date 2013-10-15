#pragma once

#include <memory>

namespace bomberman
{
	struct Entity;
	typedef std::shared_ptr<Entity> EntityPtr;
	typedef std::shared_ptr<Entity> EntityConstPtr;
	typedef unsigned int EntityID;
}