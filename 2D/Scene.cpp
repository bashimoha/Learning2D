#include "Scene.h"


Scene::Scene()
	:mGame(nullptr)
{
}

Scene::Scene(Engine* engine)
{
	mGame = engine;
}


void Scene::regiserAction(int key, int action)
{
	mActions[key] = action;
}


void Scene::doAction(const Action& action)
{
	DoAction(action);
}

void Scene::setPause(bool pause)
{
	mPause = pause;
}
bool Scene::Paused()
{
	return mPause;
}

bool Scene::Ended()
{
	return mEnded;
}

std::map<int, std::string> Scene::ActionMap() 
{
	return mActions;
}
