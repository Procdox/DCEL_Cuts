// DCEL.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include "DCEL_point.h"

int gcd(int a, int b) {
	if (a == 0)
		return b;
	return gcd(b % a, a);
}

_P::_P() {
	X = 0;
	Y = 0;
}
_P::_P(int x, int y) {
	X = x;
	Y = y;
}

_P _P::operator+(const _P &add) const {
	return _P(X + add.X, Y + add.Y);
}
_P _P::operator-(const _P &sub) const {
	return _P(X - sub.X, Y - sub.Y);
}
_P _P::operator*(int mul) const {
	return _P(X * mul, Y * mul);
}
_P _P::operator*(const _P &mul) const {
	return _P(X * mul.X, Y * mul.Y);
}
_P _P::operator/(int div) const {
	//check(X % div == 0);
	//check(Y % div == 0);
	return _P(X / div, Y / div);
}
_P _P::operator/(const _P &div) const {
	//check(X % div.X == 0);
	//check(Y % div.Y == 0);
	return _P(X / div.X, Y / div.Y);
}

_P& _P::operator+=(const _P &add) {
	X += add.X;
	Y += add.Y;
	return *this;
}
_P& _P::operator-=(const _P &sub) {
	X -= sub.X;
	Y -= sub.Y;
	return *this;
}
_P& _P::operator*=(float mul) {
	X *= mul;
	Y *= mul;
	return *this;
}
_P& _P::operator*=(const _P &mul) {
	X *= mul.X;
	Y *= mul.Y;
	return *this;
}
_P& _P::operator/=(int div) {
	//check(X % div == 0);
	//check(Y % div == 0);
	X /= div;
	Y /= div;
	return *this;
}
_P& _P::operator/=(const _P &div) {
	//check(X % div.X == 0);
	//check(Y % div.Y == 0);
	X /= div.X;
	Y /= div.Y;
	return *this;
}

bool _P::operator==(const _P &test) const {
	return test.X == X && test.Y == Y;
}
bool _P::operator!=(const _P &test) const {
	return test.X != X || test.Y != Y;
}
int _P::SizeSquared() const {
	return X * X + Y * Y;
}
float _P::Size() const {
	//return FMath::Sqrt(SizeSquared()); //MARK FLOAT USED
	return 0;
}
//void Normalize() {
//	const int sizeSq = SizeSquared();
//	const int sizeSq = SizeSquared();
//	if (sizeSq == 0) {
//		return;
//	}
//	X /= size;
//	Y /= size;
//}

int _P::Dot(const _P &b) const {
	return X * b.X + Y * b.Y;
}

point_near_segment_state _P::getState(const _P &start, const _P &end) const {
	if (*this == start) {
		return on_start;
	}
	if (*this == end) {
		return on_end;
	}

	const int TWAT = (X * start.Y - Y * start.X) + (start.X * end.Y - start.Y * end.X) + (end.X * Y - end.Y * X);
	if (TWAT > 0) {
		return left_of_segment;
	}
	if (TWAT < 0) {
		return right_of_segment;
	}

	const int target_size = (start - end).SizeSquared();
	if ((*this - end).SizeSquared() > target_size) {
		return before_segment;
	}
	if ((*this - start).SizeSquared() > target_size) {
		return after_segment;
	}

	return on_segment;
}

_P _P::decompose(int &factor) {
	//we need to find every common factor of x and y and remove it
	factor = gcd(this->X, this->Y);
	_P product = *this;
	product /= (factor);
	return product;
}

bool _P::areParrallel(const _P &A_S, const _P &A_E, const _P &B_S, const _P &B_E) {
	const _P A = A_E - A_S;
	const _P A_R = A + B_S;

	//if A lies on B or after it they are parrallel
	auto state = A_R.getState(B_S, B_E);
	return (state == on_segment || state == on_end || state == after_segment);

}

bool _P::isOnSegment(const _P &test, const _P &a, const _P &b) {
	auto state = test.getState(a, b);
	return (state == on_segment || state == on_start);
}

bool _P::inRegionCW(const _P &test, const _P &before, const _P &corner, const _P &after) {
	return (test.getState(before, corner) == right_of_segment && test.getState(corner, after) == right_of_segment);
}

int _P::getIntersect(const _P &A_S, const _P &A_E, const _P &B_S, const _P &B_E, _P &Result) {
	const auto A = A_E - A_S;
	const auto B = B_E - B_S;

	const auto D = A_S - B_S;

	const auto denom = A.X * B.Y - A.Y * B.X;

	if (denom == 0) { //REFACTOR
		return -1;
	}

	const auto s = (A.X * D.Y - A.Y * D.X); // denom;
	const auto t = (B.X * D.Y - B.Y * D.X); // denom;

	if (denom > 0) {
		if (s >= 0 && s <= denom && t >= 0 && t <= denom) {
			Result = (A_E - A_S) * t / denom + A_S;
			return (A_S - Result).SizeSquared();
		}
	}
	else {
		if (s <= 0 && s >= denom && t <= 0 && t >= denom) {
			Result = (A_E - A_S) * t / denom + A_S;
			return (A_S - Result).SizeSquared();
		}
	}

	return -1;
}
