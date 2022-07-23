#include "EntityManager.h"

void remove_dead_entity(EntityVector& entities)
{
	//I think adding all the dead entities to a vector and then deleting all of them at once is faster than deleting them one by one. maybe not? idk.
	auto erase = std::remove_if(entities.begin(), entities.end(), [](const std::shared_ptr<Entity> e) { return !e->isActive(); });
	entities.erase(erase, entities.end());
}
EntityManager::EntityManager()
	:mNextEntityID(0)
{
}
void EntityManager::update()
{
	//add everything in the toAdd list to the entity list
	//this shoud be called before the next frame
	for (auto e : mEntities_toAdd)
	{
		mEntities.push_back(e);
		mEntitiesMap[e->Tag()].push_back(e);
	}
	mEntities_toAdd.clear();
	
	//rmv dead entities from m_entities and m_entitiesMap
	remove_dead_entity(mEntities);
	for (auto& pair : mEntitiesMap)
	{
		remove_dead_entity(pair.second);
	}
}
//prevent iterator invalidation by causing a delay effect 
std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	auto entity = std::shared_ptr<Entity>(new Entity(tag, mNextEntityID++));
	//add it to the list of entities to be updated
    mEntities_toAdd.push_back(entity);
	return entity;
}

EntityVector EntityManager::getEntities() const
{
    return mEntities;
}

//given a tag, return a vector of all entities with that tag
EntityVector EntityManager::getEntities(const std::string& tag) const
{
    
    return mEntitiesMap.at(tag);
}

/*
*Iterator Invalidation:
* When you iterate over a container, you can't modify the container while you are iterating over it. why?
* Because most of the time when we append or remove an element from a container, we are modifying the container by shrinking or expanding the container. This means that the iterator is not guaranteed to point to the element that we are looking for.
* https://www.geeksforgeeks.org/iterator-invalidation-cpp/#:~:text=Iterator%20Invalidation%20Rules%3A%201.%20Insertion%20%20%20Vector,iterators%20and%20references%20to%20the%20era%20...%20

*/