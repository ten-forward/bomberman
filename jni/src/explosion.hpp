#pragma once

#include "entity.hpp"

// STL - libstdc++
#include <memory>

struct SDL_Texture;

namespace bomberman {
namespace arsenal {

	class Explosion;
	typedef std::shared_ptr<Explosion> ExplosionPtr;

	class Explosion : public Entity {
		public:
			static ExplosionPtr Create(int iTimeout);
			virtual void Evolve(const InputState& iInputs, uint32_t iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const;
			virtual void Interact(const InputState& iInputs, uint32_t iTimestamp, const EntitySet &iOthers);
			virtual void Render(SDL_Renderer*) const;
		private:

			enum Propagation 
			{
				IsoTropic,
				Up,
				Down,
				Left,
				Right
			};

			void Propagate(uint32_t iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const;

			int _timeout;
			int _stage;
			Propagation _propagation;
			static void InitializeGraphicRessources(SDL_Renderer *iRenderer);
			static std::shared_ptr<SDL_Texture> _Explosion[4];
	};
}
}
