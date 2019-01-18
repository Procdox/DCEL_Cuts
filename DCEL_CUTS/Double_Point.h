#pragma once
#include "FLL.h"

/*

Contains defintion for an integer bound 2d vector.
DCEL_types operate independent of spacial representation. You may substitute your own depending on your needs.
DCEL_region requires this representation as currently.

*/

enum point_near_segment_state { left_of_segment, right_of_segment, before_segment, after_segment, on_start, on_end, on_segment };
enum intersect_state { no_intersect, at_start_of_a, at_start_of_b, };


struct Pdbl {
	double X;
	double Y;

	Pdbl();
	Pdbl(double x, double y);
	//Pdbl(Pdbl &&target);
	//Pdbl(Pdbl const &target);
	//Pdbl & operator=(Pdbl const & target);

	Pdbl operator+(const Pdbl &target) const;
	Pdbl operator-(const Pdbl &target) const;
	Pdbl operator*(int factor) const;
	Pdbl operator*(double factor) const;
	Pdbl operator*(const Pdbl &target) const;
	Pdbl operator/(int factor) const;
	Pdbl operator/(double factor) const;
	Pdbl operator/(const Pdbl &target) const;

	Pdbl& operator+=(const Pdbl &target);
	Pdbl& operator-=(const Pdbl &target);
	Pdbl& operator*=(int factor);
	Pdbl& operator*=(double factor);
	Pdbl& operator*=(const Pdbl &target);
	Pdbl& operator/=(int factor);
	Pdbl& operator/=(double factor);
	Pdbl& operator/=(const Pdbl &target);

	bool operator==(const Pdbl &test) const;
	bool operator!=(const Pdbl &test) const;
	double SizeSquared() const;
	double Size() const;
	void Normalize();

	double Dot(const Pdbl &b) const;

	point_near_segment_state getState(const Pdbl &start, const Pdbl &end) const;

	static bool areParrallel(const Pdbl &A_S, const Pdbl &A_E, const Pdbl &B_S, const Pdbl &B_E);

	static bool isOnSegment(const Pdbl &test, const Pdbl &a, const Pdbl &b);

	static bool inRegionCW(const Pdbl &test, const Pdbl &before, const Pdbl &corner, const Pdbl &after);

	static bool getIntersect(const Pdbl &A_S, const Pdbl &A_E, const Pdbl &B_S, const Pdbl &B_E, Pdbl &Result);

	static double area(FLL<Pdbl> const &boundary);

};

struct PBox {
	Pdbl Min;
	Pdbl Max;

	Pdbl getExtent() const {
		return (Max - Min) / 2;
	}
	Pdbl getCenter() const {
		return (Max + Min) / 2;
	}
};

