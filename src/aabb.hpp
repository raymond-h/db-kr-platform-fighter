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

void calculateOverlap(AABB a, AABB b, coord_t &outX, coord_t &outY)
{
	auto overlapX1 = std::max<Fixed>(a.right - b.left, 0);
	auto overlapX2 = std::min<Fixed>(a.left - b.right, 0);
	outX = abs(overlapX1) < abs(overlapX2) ? overlapX1 : overlapX2;

	auto overlapY1 = std::max<Fixed>(a.bottom - b.top, 0);
	auto overlapY2 = std::min<Fixed>(a.top - b.bottom, 0);
	outY = abs(overlapY1) < abs(overlapY2) ? overlapY1 : overlapY2;
}

bool isOverlap(AABB a, AABB b)
{
	return
		(a.left <= b.right) &&
		(a.top <= b.bottom) &&
		(a.right >= b.left) &&
		(a.bottom >= b.top);
}

#endif // __AABB_HPP__