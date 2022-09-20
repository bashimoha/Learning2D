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

bool point_inside_entity(const vec2& point, const std::shared_ptr<Entity> e)
{
	auto& t = e->getComponent<CTransform>();
	auto bb = e->getComponent<CBoundingBox>().size;
	sf::RectangleShape rect;
	rect.setPosition(t.position.x, t.position.y);
	rect.setSize(sf::Vector2f(bb.x, bb.y));
	rect.setOrigin(
		bb.x / 2,
		bb.y / 2);
	return rect.getGlobalBounds().contains(point.x, point.y);
}

//box 2d conversion stuff
vec2 b2vec_to_vec2(const b2Vec2& b2vec)
{
	return vec2(b2vec.x, b2vec.y);
}
b2Vec2 vec2_to_b2vec(const vec2& vec)
{
	return b2Vec2(vec.x, vec.y);
}
float to_radians(float b2angle)
{
	return b2angle * b2_pi / 180.0f;
}
float to_degrees(float b2angle)
{
	return b2angle * 180.0f / b2_pi;
}
//convert a b2 world position to a screen position
vec2 world_to_screen(const vec2& world_pos)
{
	return vec2(world_pos.x * PIXEL_SCALE_FACTOR, world_pos.y * PIXEL_SCALE_FACTOR);
}
//convert a screen position to a b2 world position
vec2 screen_to_world(const vec2& screen_pos)
{
	return vec2(screen_pos.x / PIXEL_SCALE_FACTOR, screen_pos.y / PIXEL_SCALE_FACTOR);
}
