#pragma once

#include "Common.h"

struct CTransform
{
	vec2 position{};
	vec2 velocity{};
	float angle;
	CTransform(const vec2& pos, const vec2& vel, float ang) 
		:position(pos), velocity(vel), angle(ang) {
	}
	
};

struct CShape
{
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

struct CCollision {
	//this acts like a bounding box/circle
	float radius{};
	CCollision(float r)
		:radius(r) {}
};

struct CInput {
	CInput() {}
	bool right{ false };
	bool left{ false };
	bool up{ false };
	bool down{ false };
	vec2 mouse_position{};

};

struct CScore
{
	int score{};
	CScore(int s):score(s) {}
	
};
struct CHealth
{
	int health{}; //should this be float? maybe
	CHealth(int initialHealth)
		:health(initialHealth) {

	}
};
