#ifndef SCENE_INTERFACE_HPP
#define SCENE_INTERFACE_HPP

class SceneInterface
{
public:
	SceneInterface() {}
	virtual ~SceneInterface() {}
	
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual bool Running() = 0;
};

#endif // SCENE_INTERFACE_HPP
