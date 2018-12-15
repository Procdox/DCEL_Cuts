#pragma once

#include "DCEL_fll.h"

enum point_near_segment_state { left_of_segment, right_of_segment, before_segment, after_segment, on_start, on_end, on_segment };

/*int gcd(int a, int b) {
	if (a == 0)
		return b;
	return gcd(b % a, a);
}

class Rational {
	friend Rational_Set;
	int value;
public:
	Rational(Rational_Set* f, int n, int d = 1) {
		f->consider(this, d);
	}
	asFloat()
};


class Rational_Set {
	int scale;
	FLL<Rational> members;

public:
	void consider(Rational* pledge, int d) {
		members.Push(pledge);
		pledge->value *= scale;

		int divisor = gcd(d, scale);
		int parts = d / divisor;
		if (parts > 1) {
			scale *= parts;
			//UPDATE
		}
	}
	int get() {
		return scale;
	}
};

class Rational {
	Rational_Set* field;
	int value;
public:
	Rational(Rational_Set* f, int n, int d = 1) {
		field = f;
		field->consider(d);
		value = n * field->get();
	}
	asFloat()
};*/

struct _P {
	int X;
	int Y;

	_P();
	_P(int x, int y);

	_P operator+(const _P &add) const;
	_P operator-(const _P &sub) const;
	_P operator*(int mul) const;
	_P operator*(const _P &mul) const;
	_P operator/(int div) const;
	_P operator/(const _P &div) const;

	_P& operator+=(const _P &add);
	_P& operator-=(const _P &sub);
	_P& operator*=(float mul);
	_P& operator*=(const _P &mul);
	_P& operator/=(int div);
	_P& operator/=(const _P &div);

	bool operator==(const _P &test) const;
	bool operator!=(const _P &test) const;
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

	int Dot(const _P &b) const;

	point_near_segment_state getState(const _P &start, const _P &end) const;

	_P decompose(int &factor);

	static bool areParrallel(const _P &A_S, const _P &A_E, const _P &B_S, const _P &B_E);

	static bool isOnSegment(const _P &test, const _P &a, const _P &b);

	static bool inRegionCW(const _P &test, const _P &before, const _P &corner, const _P &after);

	static int getIntersect(const _P &A_S, const _P &A_E, const _P &B_S, const _P &B_E, _P &Result);

};

struct PBox {
	_P Min;
	_P Max;

	_P getExtent() {
		return (Max - Min) / 2;
	}
	_P getCenter() {
		return (Max + Min) / 2;
	}
};

