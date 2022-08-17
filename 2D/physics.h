#pragma once
#include "Common.h"
#include "Entity.h"

//calculates the overlap between two entities using current position
bool collision_overlap(const std::shared_ptr<Entity> e1, const std::shared_ptr<Entity> e2, bool use_prev_position, vec2& overlap);

bool point_inside_entity(const vec2& point, const std::shared_ptr<Entity> e);