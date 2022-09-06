#pragma once
#include "EntityManager.h"
#include "Action.h"
#include <box2d.h>
class Engine;
class SceneSerializer;
class Scene
{
public:
	friend class SceneSerializer;
	Scene()=default;
	explicit Scene(Engine* engine);
	//sys 
	virtual ~Scene()= default;
	virtual void Update(sf::Clock )=0;
	virtual void Render()=0;
	virtual void DoAction(const Action&)=0;
	
	void registerAction(int key, const std::string& action);
	void doAction(const Action& action);
	void setPause(bool);
	bool Paused();
	bool Ended();
	std::map<int, std::string> ActionMap();
	//find a better way to do this i.e not exposing mActions
	//why? it fails on Engine.cpp when we try to do action on key press/release ... **this line mScenes[mCurrentScene]->doAction **...
	std::map<int, std::string> mActions{};
protected:
	Engine* mEngine{nullptr};
	EntityManager mEntities{};
	SceneSerializer* mSerializer{nullptr};
	int mCurrentFrame{0};
	bool mPause{false};
	bool mEnded{false};
	b2World* mWorld{nullptr};
};

