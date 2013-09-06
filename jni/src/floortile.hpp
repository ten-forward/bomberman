#pragma once

#include "entity.hpp"

// STL -libstdc++
#include <memory>

// Forward declarations
struct SDL_Texture;

namespace bomberman {
namespace architecture {

	class FloorTile;
	typedef std::shared_ptr<FloorTile> FloorTilePtr;

	class FloorTile : public Entity {
		public:

			static FloorTilePtr Create();
			virtual void Evolve(const std::vector<InputState>& iInputs, uint32_t iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const;
			virtual void Interact(const std::vector<InputState>& , uint32_t , const EntitySet &) {}
			virtual void Render(SDL_Renderer* iRenderer) const;
		protected:
			static void InitializeGraphicRessources(SDL_Renderer *iRenderer);
			static std::shared_ptr<SDL_Texture> _FloorTile;
	};
}
}
