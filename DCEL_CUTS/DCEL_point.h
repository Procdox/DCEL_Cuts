#pragma once
#include "DCEL_fll.h"

/*

Contains defintion for an integer bound 2d vector.
DCEL_types operate independent of spacial representation. You may substitute your own depending on your needs.
DCEL_region requires this representation as currently.

*/

enum point_near_segment_state { left_of_segment, right_of_segment, before_segment, after_segment, on_start, on_end, on_segment };

struct Pint {
	int X;
	int Y;

	Pint();
	Pint(int x, int y);

	Pint operator+(const Pint &add) const;
	Pint operator-(const Pint &sub) const;
	Pint operator*(int mul) const;
	Pint operator*(const Pint &mul) const;
	Pint operator/(int div) const;
	Pint operator/(const Pint &div) const;

	Pint& operator+=(const Pint &add);
	Pint& operator-=(const Pint &sub);
	Pint& operator*=(float mul);
	Pint& operator*=(const Pint &mul);
	Pint& operator/=(int div);
	Pint& operator/=(const Pint &div);

	bool operator==(const Pint &test) const;
	bool operator!=(const Pint &test) const;
	int SizeSquared() const;
	float Size() const;
	//void Normalize() {
	//	const int sizeSq = SizeSquared();
	//	const int sizeSq = SizeSquared();
	//	if (sizeSq == 0) {
	//		return;
	//	}
	//	X /= size;
	//	Y /= size;
	//}

	int Dot(const Pint &b) const;

	point_near_segment_state getState(const Pint &start, const Pint &end) const;

	Pint decompose(int &factor);

	static bool areParrallel(const Pint &A_S, const Pint &A_E, const Pint &B_S, const Pint &B_E);

	static bool isOnSegment(const Pint &test, const Pint &a, const Pint &b);

	static bool inRegionCW(const Pint &test, const Pint &before, const Pint &corner, const Pint &after);

	static int getIntersect(const Pint &A_S, const Pint &A_E, const Pint &B_S, const Pint &B_E, Pint &Result);

	static double area(FLL<Pint> const &boundary);

};

struct PBox {
	Pint Min;
	Pint Max;

	Pint getExtent() {
		return (Max - Min) / 2;
	}
	Pint getCenter() {
		return (Max + Min) / 2;
	}
};

