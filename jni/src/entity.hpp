#pragma once

#include "entityptr.hpp"
#include "map.hpp"

#include <unordered_set>
#include <memory>

#include <SDL.h>
class InputState;

namespace bomberman
{
	struct Entity
	{
		EntityID id;
		int x;	// tile position
		int y;
		int zlevel;
		int elevel;
		int mx;	// sub-tile position
		int my;
		int dx;
		int dy;

		bool active; // is on the map?
		bool brakes; // brake on tile?

		virtual ~Entity();

		virtual void Evolve(const std::vector<InputState>& iInputs, Uint32 iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const = 0;
		virtual void Interact(const std::vector<InputState>& iInputs, Uint32 iTimestamp, const EntitySet &iOthers) = 0;
		virtual void Render(SDL_Renderer* iRenderer) const = 0;

	protected:
		Entity();
	};
}
