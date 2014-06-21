#pragma once

#include "scene_interface.hpp"
#include <memory>

namespace bomberman {

// Scene that fades in a child scene and then fades out when the scene ends
class FadeScene : public SceneInterface
{
public:
	FadeScene(const std::shared_ptr<SceneInterface> &scene);
	virtual ~FadeScene();
	
	virtual void Init(SDL_Window* window, SDL_Renderer* renderer);
	virtual void Update(const std::vector<InputState>& inputs, uint32_t timestamp);
	virtual void Render(SDL_Renderer *renderer);
	virtual bool Running();

	void SetSpeed(double speed);
	double GetSpeed() const;

private:
	std::shared_ptr<SceneInterface> _scene;
	std::shared_ptr<SDL_Texture> _renderTarget;
	int _w, _h;
	double _opacity;
	double _speed;
};

}
