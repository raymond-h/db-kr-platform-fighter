#ifndef __AABB_HPP__
#define __AABB_HPP__

#include <cstdint>
#include <algorithm>

#include "components.hpp"

struct AABB
{
	int32_t left, top, right, bottom;
};

AABB boxForEntity(Position &pos, CollisionBox &cbox)
{
	return {
		pos.x - cbox.width / 2, pos.y - cbox.height / 2,
		pos.x + cbox.width / 2, pos.y + cbox.height / 2};
}

void calculateOverlap(AABB a, AABB b, int32_t &outX, int32_t &outY)
{
	auto overlapX1 = std::max(a.right - b.left, 0);
	auto overlapX2 = std::min(a.left - b.right, 0);
	outX = std::abs(overlapX1) < std::abs(overlapX2) ? overlapX1 : overlapX2;

	auto overlapY1 = std::max(a.bottom - b.top, 0);
	auto overlapY2 = std::min(a.top - b.bottom, 0);
	outY = std::abs(overlapY1) < std::abs(overlapY2) ? overlapY1 : overlapY2;
}

#endif // __AABB_HPP__
