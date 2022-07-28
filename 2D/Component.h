#pragma once

#include "Common.h"
class Component
{
public:
	bool hasComponent = false;
};
class CTransform: Component
{
public:

	vec2 position{};
	vec2 velocity{};
	float angle;
	CTransform(const vec2& pos, const vec2& vel, float ang) 
		:position(pos), velocity(vel), angle(ang) {
	}
	
};

class CShape : Component
{
public:
	sf::CircleShape shape{};
	CShape(float radius, int pointCount, const sf::Color& fill,  const sf::Color outline, float thickness)
		:shape(radius, pointCount)
	{
		//reset pos to radius
		shape.setOrigin(radius, radius);
		shape.setFillColor(fill);
		shape.setOutlineColor(outline);
		shape.setOutlineThickness(thickness);
	}
};

class CCollision : Component
{
	//this acts like a bounding box/circle
public:
	float radius{};
	CCollision(float r)
		:radius(r) {}
};

class CInput : Component
{
public:
	CInput() {}
	bool right{ false };
	bool left{ false };
	bool up{ false };
	bool down{ false };

};

class CScore: Component
{
public:
	int score{};
	CScore(int s):score(s) {}
	
};
class CHealth: Component
{
public:
	int health{}; //should this be float? maybe
	CHealth(int initialHealth)
		:health(initialHealth) {

	}
};
