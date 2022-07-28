#pragma once
#include "EntityManager.h"
#include "Action.h"
class Engine;
class Scene
{
public:
	Scene();
	Scene(Engine* engine);
	//sys 
	virtual void Update()=0;
	virtual void Render()=0;
	virtual void DoAction(const Action&)=0;
	
	void regiserAction(int key, int action);
	void doAction(const Action& action);
	void setPause(bool);
	bool Paused();
	bool Ended();
	std::map<int, std::string> ActionMap();
	
private:
	Engine* mGame{nullptr};
	std::map<int, std::string> mActions; 
    EntityManager mEnities;
	
	int mCurrentFrame{};
	bool mPause;
	bool mEnded;

};

