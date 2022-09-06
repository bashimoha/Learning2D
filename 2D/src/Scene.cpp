#include "Scene.h"



Scene::Scene(Engine* engine)
	:mEngine(engine)
{

}


void Scene::registerAction(int key, const std::string& action)
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
