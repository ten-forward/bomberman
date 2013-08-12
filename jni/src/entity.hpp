#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <unordered_set>
#include <memory>

struct SDL_Renderer;

namespace bomberman
{

	struct Entity;

	typedef std::shared_ptr<Entity> EntityPtr;

	struct Entity
	{

		int id;
		int dx;	// speed
		int dy;
		int x;	// tile position
		int y;
		int zlevel;
		int mx;	// sub-tile position
		int my;
		bool active; // is on the map?
		bool brakes; // brake on tile?
		int userdata;

		virtual ~Entity();

		virtual void Render(SDL_Renderer* iRenderer) const = 0;

		typedef std::unordered_set<Entity *> EntitySet;
		static EntitySet &GetAllEntities() {
			return _Entities;
		}

	protected:
		Entity();

	private:
		
		static EntitySet _Entities;
	};
}

#endif