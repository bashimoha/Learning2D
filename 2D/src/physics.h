#pragma once
#include "Common.h"
#include "Entity.h"
#include <box2d.h>
//calculates the overlap between two entities using current position
bool collision_overlap(const std::shared_ptr<Entity> e1, const std::shared_ptr<Entity> e2, bool use_prev_position, vec2& overlap);
bool point_inside_entity(const vec2& point, const std::shared_ptr<Entity> e);
vec2 b2vec_to_vec2(const b2Vec2& b2vec);
b2Vec2 vec2_to_b2vec(const vec2& vec);
float to_radians(float b2angle);
float to_degrees(float b2angle);
vec2 world_to_screen(const vec2& world_pos);
vec2 screen_to_world(const vec2& screen_pos);




