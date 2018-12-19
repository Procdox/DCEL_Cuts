#include "pch.h"
#include "..\DCEL_CUTS\DCEL_point.h"

TEST(Point_Tests, on_segment) {
	_P p(-200, 300);
	_P a(-300, 400);
	_P b(100, 0);

	EXPECT_TRUE(_P::isOnSegment(p, a, b));
}