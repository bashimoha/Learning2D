#pragma once
#include "Common.h"

#include "Entity.h"
typedef std::vector<std::shared_ptr<Entity>> EntityVector;
typedef std::map<std::string, EntityVector> EntityMap;
class EntityManager
{
public:
	EntityManager();
	void update();
	std::shared_ptr<Entity> addEntity(const std::string& tag);
	EntityVector  getEntities() const;
	EntityVector getEntities(const std::string& tag) const;
		
private:
	 
	EntityVector mEntities;
	EntityMap mEntitiesMap;
	EntityVector mEntities_toAdd;
	size_t mNextEntityID{0};
};
