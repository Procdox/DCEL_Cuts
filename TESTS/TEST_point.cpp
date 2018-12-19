#include "pch.h"
#include "..\DCEL_CUTS\DCEL_point.h"

TEST(Point_Tests, on_segment) {
	Pint p(-200, 300);
	Pint a(-300, 400);
	Pint b(100, 0);

	EXPECT_TRUE(Pint::isOnSegment(p, a, b));
}