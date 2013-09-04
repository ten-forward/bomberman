#pragma once

#include "entityptr.hpp"
#include "map.hpp"

#include <unordered_set>
#include <memory>

struct SDL_Renderer;
class InputState;

namespace bomberman
{

	struct Entity
	{

		int id;
		int x;	// tile position
		int y;
		int zlevel;
		int elevel;
		int mx;	// sub-tile position
		int my;
		bool active; // is on the map?
		bool brakes; // brake on tile?

		virtual ~Entity();

		virtual void Evolve(const InputState& iInputs, uint32_t iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const = 0;

		virtual void Render(SDL_Renderer* iRenderer) const = 0;

	protected:
		Entity();
	};
}
