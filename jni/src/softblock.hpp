#pragma once

#include "entity.hpp"

// STL -libstdc++
#include <memory>

// Forward declarations
struct SDL_Texture;

namespace bomberman {
namespace architecture {

	class SoftBlock;
	typedef std::shared_ptr<SoftBlock> SoftBlockPtr;


	class SoftBlock : public Entity {
		bool isAlive;
		public:
			static SoftBlockPtr Create(double iBonusProbability);
			virtual void Evolve(const std::vector<InputState>& iInputs, Uint32 iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const;
			virtual void Interact(const std::vector<InputState>& , Uint32 , const EntitySet &) {}
			virtual void Render(SDL_Renderer* iRenderer) const;

			virtual void Kill();
		protected:
			static void InitializeGraphicRessources(SDL_Renderer *iRenderer);
			static std::shared_ptr<SDL_Texture> _SoftBlock;
			double _bonusProbability;
	};
}
}
