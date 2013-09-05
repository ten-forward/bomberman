#pragma once

#include "entity.hpp"

// STL -libstdc++
#include <memory>

// Forward declarations
struct SDL_Texture;

namespace bomberman {
namespace architecture {

	class Block;
	typedef std::shared_ptr<Block> BlockPtr;

	class Block : public Entity {
		public:
			static BlockPtr Create();
			virtual void Evolve(const InputState& iInputs, Uint32 iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const;
			virtual void Interact(const InputState& , Uint32 , const EntitySet &) {}
			virtual void Render(SDL_Renderer* iRenderer) const;
		protected:
			static void InitializeGraphicRessources(SDL_Renderer *iRenderer);
			static std::shared_ptr<SDL_Texture> _Block;
	};
}
}
