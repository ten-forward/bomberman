#ifndef FADESCENE_HPP
#define FADESCENE_HPP

#include "scene_interface.hpp"

#include <memory>

// Scene that fades in a child scene and then fades out when the scene ends
class FadeScene : public SceneInterface
{
	std::shared_ptr<SceneInterface> scene;
	std::shared_ptr<SDL_Texture> renderTarget;
	int w,h;
	double opacity;
	double speed;

public:
	FadeScene(std::shared_ptr<SceneInterface> scene);
	virtual ~FadeScene();
	
	virtual void Init(SDL_Window* window, SDL_Renderer* renderer);
	virtual void Update(const std::vector<InputState>& inputs, uint32_t timestamp);
	virtual void Render(SDL_Renderer *renderer);
	virtual bool Running();

	void SetSpeed(double speed);
	double GetSpeed() const;
};

#endif // FADESCENE_HPP
