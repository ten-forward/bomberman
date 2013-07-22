#ifndef TESTSCENE_HPP
#define TESTSCENE_HPP

#include "scene_interface.hpp"

class TestScene : public SceneInterface
{
public:
	
	virtual void Update();
	virtual void Render();
	virtual bool Running();
};

#endif // TESTSCENE_HPP
