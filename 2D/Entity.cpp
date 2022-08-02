#include "Entity.h"

std::string Entity::Tag() const
{
	return mTag;

}
size_t Entity::ID() const
{
	return mId;
}

bool Entity::isActive() const
{
	return mActive;
}

void Entity::destroy()
{
	mActive = false;
}

Entity::Entity(const std::string& tag, size_t id)
	:mTag(tag), mId(id), mActive(true)
{
	
}
