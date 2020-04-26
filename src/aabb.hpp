#ifndef __AABB_HPP__
#define __AABB_HPP__

#include <cstdint>
#include <algorithm>

#include "fixed.hpp"

typedef Fixed coord_t;

struct AABB
{
	coord_t left, top, right, bottom;
};

void calculateOverlap(AABB a, AABB b, coord_t& outX, coord_t& outY);

bool isOverlap(AABB a, AABB b);

#endif // __AABB_HPP__