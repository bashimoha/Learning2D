#pragma once

#include "Common.h"
#include "Animation.h"
#include <box2d.h>

class Component
{
public:
	Component() = default;
	bool hasComponent = false;
};
class CTransform: public Component
{
public:
	CTransform() = default;
	vec2 position{}, velocity{}, prev_position{}, scale{};
	float angle;
	CTransform(const vec2& pos, const vec2& vel, float ang, vec2 _scale = vec2{1.f, 1.f})
		:position(pos), velocity(vel), prev_position(pos), scale(_scale), angle(ang){}
};
class CInput : public Component
{
public:
	CInput() {}
	bool right{ false },left{ false },up{ false } ,down{ false };
};

class CHealth:public Component
{
public:
	int health{}; 
	CHealth(int initialHealth):health(initialHealth) {}
	CHealth():health(100){}
};

class CGravity : public Component
{
public:
	CGravity() {}
	CGravity(float gravity, float velocity = 1.f) :gravity(gravity), gravity_velocity(velocity) {}
	float gravity{ 0.f };
	float gravity_velocity{ 0.f };
};

class CBoundingBox :public Component
{
public:
	vec2 size{};
	bool collidable{ false };
	b2AABB aabb{};
	CBoundingBox(float w, float h) :size(w, h) {
		aabb.lowerBound = b2Vec2(0.f, 0.f);
		aabb.upperBound = b2Vec2(w, h);
	}
	CBoundingBox(const vec2& _size) {
		CBoundingBox( _size.x, _size.y);
	}
	CBoundingBox() = default;
};
class CAnimation :public Component
{
public:
	Animation animation;
	bool repeat{ false };
	CAnimation(const Animation& _animation, bool rep) 
		:animation (_animation), repeat(rep) {}
	CAnimation() {
		animation = Animation();
		repeat = false;
	}
};
class CState :public Component
{
	public:
		CState() = default;
		std::string state{ "standing" };
};
class CDraggable: public Component
{
	public:
	CDraggable() = default;
	bool dragging{ false };
	bool duplicate{ false };
};
class CRigidBody : public Component
{
public:
	CRigidBody() = default;
	CRigidBody(b2Body* _body)
	{
		body = _body;
	}
	b2Body* body{ nullptr };
	b2Fixture* fixture{ nullptr };
};
class CRenderable : public Component
{
public:
	sf::CircleShape shape{};
	CRenderable(float radius, int pointCount, const sf::Color& fill, const sf::Color outline, float thickness)
		:shape(radius, pointCount)
	{
		//reset pos to radius
		shape.setOrigin(radius, radius);
		shape.setFillColor(fill);
		shape.setOutlineColor(outline);
		shape.setOutlineThickness(thickness);
	}
	CRenderable(const sf::CircleShape& _shape) :shape(_shape) {}
	CRenderable() = default;

};
