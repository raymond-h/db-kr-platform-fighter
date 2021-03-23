#ifndef __AABB_HPP__
#define __AABB_HPP__

#include <algorithm>
#include <cstdint>

#include "fixed.hpp"

struct AABB
{
	Fixed left, top, right, bottom;
};

void calculateOverlap(const AABB &a, const AABB &b, Fixed &outX, Fixed &outY);

#endif// __AABB_HPP__