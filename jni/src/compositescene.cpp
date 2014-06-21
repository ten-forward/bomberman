#include "compositescene.hpp"

#include <boost/foreach.hpp>

CompositeScene::CompositeScene()
{

}

CompositeScene::~CompositeScene()
{

}
	
void CompositeScene::Init(SDL_Window* window, SDL_Renderer* renderer)
{
	BOOST_FOREACH(auto scene, scenes)
	{
		scene->Init(window, renderer);
	};

	this->window = window;
	this->renderer = renderer;
}

void CompositeScene::Update(const std::vector<InputState>& inputs, Uint32 timestamp)
{
	BOOST_FOREACH(auto scene, scenes)
	{
		scene->Update(inputs, timestamp);
	};

	scenes.remove_if([](std::tr1::shared_ptr<SceneInterface> scene) { return !scene->Running(); });
}

void CompositeScene::Render(SDL_Renderer *renderer)
{
	BOOST_FOREACH(auto scene, scenes)
	{
		scene->Render(renderer);
	};
}

bool CompositeScene::Running() const
{
	return !scenes.empty();
}

void CompositeScene::AddScene(std::tr1::shared_ptr<SceneInterface> scene)
{
	scenes.push_front(scene);
}
