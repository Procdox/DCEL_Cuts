#include "Double_Point.h"
#include <cmath>
#include <limits>

Pdbl::Pdbl() {
	X = 0;
	Y = 0;
}
Pdbl::Pdbl(double x, double y) {
	X = x;
	Y = y;
}

Pdbl Pdbl::operator+(const Pdbl &target) const {
	return Pdbl(X + target.X, Y + target.Y);
}
Pdbl Pdbl::operator-(const Pdbl &target) const {
	return Pdbl(X - target.X, Y - target.Y);
}
Pdbl Pdbl::operator*(int factor) const
{
	return Pdbl(X * factor, Y * factor);
}
Pdbl Pdbl::operator*(double factor) const {
	return Pdbl(X * factor, Y * factor);
}
Pdbl Pdbl::operator*(const Pdbl &target) const {
	return Pdbl(X * target.X, Y * target.Y);
}
Pdbl Pdbl::operator/(int factor) const
{
	return Pdbl(X / factor, Y / factor);
}
Pdbl Pdbl::operator/(double factor) const {
	return Pdbl(X / factor, Y / factor);
}
Pdbl Pdbl::operator/(const Pdbl &target) const {
	return Pdbl(X / target.X, Y / target.Y);
}

Pdbl& Pdbl::operator+=(const Pdbl &target) {
	X += target.X;
	Y += target.Y;
	return *this;
}
Pdbl& Pdbl::operator-=(const Pdbl &target) {
	X -= target.X;
	Y -= target.Y;
	return *this;
}
Pdbl & Pdbl::operator*=(int factor)
{
	X *= factor;
	Y *= factor;

	return *this;
}
Pdbl& Pdbl::operator*=(double factor) {
	X *= factor;
	Y *= factor;

	return *this;
}
Pdbl& Pdbl::operator*=(const Pdbl &target) {
	X *= target.X;
	Y *= target.Y;
	return *this;
}
Pdbl & Pdbl::operator/=(int factor)
{
	X /= factor;
	Y /= factor;

	return *this;
}
Pdbl& Pdbl::operator/=(double factor) {
	//check(X % div == 0);
	//check(Y % div == 0);
	X /= factor;
	Y /= factor;
	return *this;
}
Pdbl& Pdbl::operator/=(const Pdbl &target) {
	//check(X % div.X == 0);
	//check(Y % div.Y == 0);
	X /= target.X;
	Y /= target.Y;
	return *this;
}

bool Pdbl::operator==(const Pdbl &test) const {
	bool absolute = (abs(test.X - X) < 1.0) && (abs(test.Y - Y) < 1.0);
	bool relative = (abs(test.X - X) < fmax(test.X, X) * DBL_EPSILON) &&
		(abs(test.Y - Y) < fmax(test.Y, Y) * DBL_EPSILON);
	return absolute || relative;
}
bool Pdbl::operator!=(const Pdbl &test) const {
	return !operator==(test);
}
double Pdbl::SizeSquared() const {
	return X * X + Y * Y;
}
double Pdbl::Size() const {
	return sqrt(SizeSquared());
}

void Pdbl::Normalize() {
	const double size = Size();
	if (size == 0) {
		return;
	}
	X /= size;
	Y /= size;
}

double Pdbl::Dot(const Pdbl &b) const {
	return X * b.X + Y * b.Y;
}

//DOUBLE BOOLEAN OPS
point_near_segment_state Pdbl::getState(const Pdbl &start, const Pdbl &end) const {
	if (*this == start) {
		return on_start;
	}
	if (*this == end) {
		return on_end;
	}

	const double TWAT = (X * start.Y - Y * start.X) + (start.X * end.Y - start.Y * end.X) + (end.X * Y - end.Y * X);
	if (TWAT > 0.0) {
		return left_of_segment;
	}
	if (TWAT < 0.0) {
		return right_of_segment;
	}

	const double target_size = (start - end).SizeSquared();
	if ((*this - end).SizeSquared() > target_size) {
		return before_segment;
	}
	if ((*this - start).SizeSquared() > target_size) {
		return after_segment;
	}

	return on_segment;
}

//DOUBLE BOOLEAN OPS
bool Pdbl::areParrallel(const Pdbl &A_S, const Pdbl &A_E, const Pdbl &B_S, const Pdbl &B_E) {
	const Pdbl A = A_E - A_S;
	const Pdbl A_R = A + B_S;

	//if A lies on B or after it they are parrallel
	auto state = A_R.getState(B_S, B_E);
	return (state == on_segment || state == on_end || state == after_segment);

}

bool Pdbl::isOnSegment(const Pdbl &test, const Pdbl &a, const Pdbl &b) {
	auto state = test.getState(a, b);
	return (state == on_segment || state == on_start || state == on_end);
}

bool Pdbl::inRegionCW(const Pdbl &test, const Pdbl &before, const Pdbl &corner, const Pdbl &after) {
	return (test.getState(before, corner) == right_of_segment && test.getState(corner, after) == right_of_segment);
}

//DOUBLE BOOLEAN OPS
bool Pdbl::getIntersect(const Pdbl &A_S, const Pdbl &A_E, const Pdbl &B_S, const Pdbl &B_E, Pdbl &Result) {
	const auto A = A_E - A_S;
	const auto B = B_E - B_S;

	const auto D = A_S - B_S;

	const auto denom = A.X * B.Y - A.Y * B.X;

	if (denom == 0) { //REFACTOR
		return false;
	}

	const auto s = (A.X * D.Y - A.Y * D.X) / denom;
	const auto t = (B.X * D.Y - B.Y * D.X) / denom;

	if (s >= 0.0 && s <= 1.0 && t >= 0.0 && t <= 1.0) {
		Result = (A * t) + A_S;
		return true;
	}

	return false;
}

double Pdbl::area(FLL<Pdbl> const & boundary) {
	double total = 0;

	Pdbl A = boundary.last();

	for (auto B : boundary) {

		double width = B.X - A.X;
		double avg_height = (A.Y + B.Y) / 2;

		total += width * avg_height;

		A = B;
	}

	return total;
}