#include "Entity.h"

std::string Entity::Tag() const
{
	return mTag;

}
void Entity::SetTag(const std::string& tag)
{
	mTag = tag;
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
void Entity::CopyComponentFrom(std::shared_ptr<Entity> src, std::shared_ptr<Entity> dest)
{
	/*std::tuple<CTransform, CGravity, CHealth, CInput, CBoundingBox, CState, CAnimation, CDraggable>;*/
	if( src->hasComponent<CTransform>() ) {dest->addComponent<CTransform>(src->getComponent<CTransform>());}
	if( src->hasComponent<CGravity>() ) {dest->addComponent<CGravity>(src->getComponent<CGravity>());}
	if( src->hasComponent<CHealth>() ) {dest->addComponent<CHealth>(src->getComponent<CHealth>());}
	if( src->hasComponent<CInput>() ) {dest->addComponent<CInput>(src->getComponent<CInput>());}
	if( src->hasComponent<CBoundingBox>() ) {dest->addComponent<CBoundingBox>(src->getComponent<CBoundingBox>());}
	if( src->hasComponent<CState>() ) {dest->addComponent<CState>(src->getComponent<CState>());}
	if( src->hasComponent<CAnimation>() ) {dest->addComponent<CAnimation>(src->getComponent<CAnimation>());}
	if( src->hasComponent<CDraggable>() ) {dest->addComponent<CDraggable>(src->getComponent<CDraggable>());}
	
}

Entity::Entity(const std::string& tag, size_t id)
	:mTag(tag), mId(id), mActive(true)
{
	
}
