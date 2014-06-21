#ifndef COMPOSITESCENE_HPP
#define COMPOSITESCENE_HPP

#include "scene_interface.hpp"

#include <boost/tr1/memory.hpp>

#include <forward_list>

// Scene that displays a dialog
class CompositeScene : public bomberman::SceneInterface
{
	std::forward_list<std::tr1::shared_ptr<SceneInterface>> scenes;
	SDL_Window* window;
	SDL_Renderer* renderer;

public:
	CompositeScene();
	virtual ~CompositeScene();
	
	virtual void Init(SDL_Window* window, SDL_Renderer* renderer);
	virtual void Update(const std::vector<InputState>& inputs, Uint32 timestamp);
	virtual void Render(SDL_Renderer *renderer);
	virtual bool Running() const;

	virtual void AddScene(std::tr1::shared_ptr<SceneInterface> scene);

};

#endif // COMPOSITESCENE_HPP
