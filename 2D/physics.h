#pragma once
#include "Common.h"
#include "Entity.h"

//calculates the overlap between two entities using current position
vec2 collision_overlap(const std::shared_ptr<Entity> e1, const std::shared_ptr<Entity> e2, bool use_prev_position);
//{
//	auto e1_pos = e1->getComponent<CTransform>().position;
//	auto e2_pos = e2->getComponent<CTransform>().position;
//	auto e1_size = e1->getComponent<CBoundingBox>().size;
//	auto e2_size = e2->getComponent<CBoundingBox>().size;
//	
//	vec2 delta = {std::abs(e1_pos.x - e2_pos.x), std::abs(e1_pos.y - e2_pos.y)};
//	auto x_overlap = (e1_size.x/2 + e2_size.x/2) - delta.x;
//	auto y_overlap = (e1_size.y/2 + e2_size.y/2) - delta.y;
//	return {x_overlap, y_overlap};
//}
//calculate the overlap between two entities using previous position
