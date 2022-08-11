#pragma once
#include "Common.h"
#include "Entity.h"
#include "physics.h"
//calculates the overlap between two entities using their position or previous postion is  use_prev_position is true
bool collision_overlap(const std::shared_ptr<Entity> e1, const std::shared_ptr<Entity> e2, bool use_prev_position, vec2& overlap)
{
	vec2 pos1 = use_prev_position ? e1->getComponent<CTransform>().prev_position : e1->getComponent<CTransform>().position;
	vec2 pos2 = use_prev_position ? e2->getComponent<CTransform>().prev_position : e2->getComponent<CTransform>().position;

	vec2 size1 = e1->getComponent<CBoundingBox>().size;
	vec2 size2 = e2->getComponent<CBoundingBox>().size;

	auto delta = pos1 - pos2;
	
	auto x_overlap = size1.x/2 + size2.x/2 - std::abs(delta.x);
	auto y_overlap = size1.y/2 + size2.y/2 - std::abs(delta.y);	
	overlap = { (x_overlap), (y_overlap) };
	return (x_overlap > 0 && y_overlap > 0);
}
