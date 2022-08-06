#pragma once
#include "Common.h"
#include "Entity.h"
#include "physics.h"
//calculates the overlap between two entities using th position
vec2 collision_overlap(const std::shared_ptr<Entity> e1, const std::shared_ptr<Entity> e2, bool use_prev_position)
{
	vec2 e1_pos = use_prev_position ? e1->getComponent<CTransform>().prev_position: e1->getComponent<CTransform>().position;
	vec2 e2_pos = use_prev_position ? e2->getComponent<CTransform>().prev_position: e2->getComponent<CTransform>().position;
	auto e1_size = e1->getComponent<CBoundingBox>().size;
	auto e2_size = e2->getComponent<CBoundingBox>().size;

	vec2 delta = { std::abs(e1_pos.x - e2_pos.x), std::abs(e1_pos.y - e2_pos.y) };
	auto x_overlap = delta.x - (e1_size.x / 2 + e2_size.x / 2);
	auto y_overlap = delta.y - (e1_size.y / 2 + e2_size.y / 2);
	return {(x_overlap), (y_overlap) };
}
