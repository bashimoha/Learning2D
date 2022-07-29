#pragma once
#include "EntityManager.h"
#include "Action.h"
class Engine;
class Scene
{
public:
	Scene()=default;
	explicit Scene(Engine* engine);
	//sys 
	virtual void Update()=0;
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
	Engine* mGame{nullptr};
	EntityManager mEnities{};
	
	int mCurrentFrame{0};
	bool mPause{false};
	bool mEnded{false};

};

