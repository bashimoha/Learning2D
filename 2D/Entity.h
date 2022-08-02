#pragma once
#include "Common.h"
#include "Component.h"

class EntityManager; 

//tuple of components maybe a better way?
using ComponentTuple = std::tuple<CTransform, CGravity, CHealth,CInput, CBoundingBox, CState, CAnimation>
;

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
		auto& component = std::get<T>(mComponents);
		return component.hasComponent;
	}
	template <typename T>
	T& getComponent()
	{
		return std::get<T>(mComponents);
	}
	template <typename T, typename... Targs>
	T& addComponent(Targs&&... args)
	{
		auto& component = getComponent<T>();
		component = T(std::forward<Targs>(args)...);
		component.hasComponent = true;
		return component;
	}
	
private:
	std::string mTag{"Default"};
	size_t mId{0};
	bool mActive{true};
	ComponentTuple mComponents;
	Entity(const std::string& tag, size_t id);
};

