#pragma once
#include "Common.h"
#include "Component.h"

class Entity
{
public:
	friend class EntityManager;
	//components
	std::shared_ptr<CTransform> cTransform;
	std::shared_ptr<CShape> cShape;
	std::shared_ptr<CCollision> cCollision;
	std::shared_ptr<CInput> cInput;
	std::shared_ptr<CScore> cScore;
	std::shared_ptr<CHealth> cHealth;
	
	std::string Tag() const;
	size_t ID() const;
	bool isActive() const;
	void destroy();

private:
	std::string mTag{"Default"};
	size_t mId{0};
	bool mActive{true};
	Entity(const std::string& tag, size_t id);

};

