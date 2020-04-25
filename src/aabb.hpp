#ifndef __AABB_HPP__
#define __AABB_HPP__

#include <cstdint>
#include <algorithm>

#include "components.hpp"

struct AABB
{
	coord_t left, top, right, bottom;
};

AABB boxForEntity(Position& pos, CollisionBox& cbox);

void calculateOverlap(AABB a, AABB b, coord_t& outX, coord_t& outY);

#endif // __AABB_HPP__