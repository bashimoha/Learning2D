#pragma once
#include "Common.h"
#include "Component.h"

class EntityManager; 

//tuple of components maybe a better way?
typedef std::tuple<CTransform, CShape, CHealth,CInput, CCollision, CScore>
ComponentTuple;

class Entity
{
public:
	friend class EntityManager;
	
	std::string Tag() const;
	size_t ID() const;
	bool isActive() const;
	void destroy();
	template <typename T>
	bool hasComponent() const
	{
		return getComponent<T>().has;
	}
	template <typename T>
	T& getComponent()
	{
		std::get<T>(mComponents);
	}
	template <typename T, typename... Targs>
	T& addComponent(Targs&&... args)
	{
		auto& component = getComponent<T>();
		component = T( std::forward<args>(args)... );
		component.has = true;
		return component;
	}
	

private:
	std::string mTag{"Default"};
	size_t mId{0};
	bool mActive{true};
	std::shared_ptr<ComponentTuple> mComponents;
	Entity(const std::string& tag, size_t id);
};

