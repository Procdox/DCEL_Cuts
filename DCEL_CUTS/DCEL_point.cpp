#include "DCEL_Point.h"

int gcd(int a, int b) {
	if (a == 0)
		return b;
	return gcd(b % a, a);
}

Pint::Pint() {
	X = 0;
	Y = 0;
}
Pint::Pint(int x, int y) {
	X = x;
	Y = y;
}

Pint Pint::operator+(const Pint &add) const {
	return Pint(X + add.X, Y + add.Y);
}
Pint Pint::operator-(const Pint &sub) const {
	return Pint(X - sub.X, Y - sub.Y);
}
Pint Pint::operator*(int mul) const {
	return Pint(X * mul, Y * mul);
}
Pint Pint::operator*(const Pint &mul) const {
	return Pint(X * mul.X, Y * mul.Y);
}
Pint Pint::operator/(int div) const {
	//check(X % div == 0);
	//check(Y % div == 0);
	return Pint(X / div, Y / div);
}
Pint Pint::operator/(const Pint &div) const {
	//check(X % div.X == 0);
	//check(Y % div.Y == 0);
	return Pint(X / div.X, Y / div.Y);
}

Pint& Pint::operator+=(const Pint &add) {
	X += add.X;
	Y += add.Y;
	return *this;
}
Pint& Pint::operator-=(const Pint &sub) {
	X -= sub.X;
	Y -= sub.Y;
	return *this;
}
Pint& Pint::operator*=(float mul) {
	X *= mul;
	Y *= mul;
	return *this;
}
Pint& Pint::operator*=(const Pint &mul) {
	X *= mul.X;
	Y *= mul.Y;
	return *this;
}
Pint& Pint::operator/=(int div) {
	//check(X % div == 0);
	//check(Y % div == 0);
	X /= div;
	Y /= div;
	return *this;
}
Pint& Pint::operator/=(const Pint &div) {
	//check(X % div.X == 0);
	//check(Y % div.Y == 0);
	X /= div.X;
	Y /= div.Y;
	return *this;
}

bool Pint::operator==(const Pint &test) const {
	return test.X == X && test.Y == Y;
}
bool Pint::operator!=(const Pint &test) const {
	return test.X != X || test.Y != Y;
}
int Pint::SizeSquared() const {
	return X * X + Y * Y;
}
float Pint::Size() const {
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

int Pint::Dot(const Pint &b) const {
	return X * b.X + Y * b.Y;
}

point_near_segment_state Pint::getState(const Pint &start, const Pint &end) const {
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

Pint Pint::decompose(int &factor) {
	//we need to find every common factor of x and y and remove it
	factor = gcd(this->X, this->Y);
	Pint product = *this;
	product /= (factor);
	return product;
}

bool Pint::areParrallel(const Pint &A_S, const Pint &A_E, const Pint &B_S, const Pint &B_E) {
	const Pint A = A_E - A_S;
	const Pint A_R = A + B_S;

	//if A lies on B or after it they are parrallel
	auto state = A_R.getState(B_S, B_E);
	return (state == on_segment || state == on_end || state == after_segment);

}

bool Pint::isOnSegment(const Pint &test, const Pint &a, const Pint &b) {
	auto state = test.getState(a, b);
	return (state == on_segment || state == on_start);
}

bool Pint::inRegionCW(const Pint &test, const Pint &before, const Pint &corner, const Pint &after) {
	return (test.getState(before, corner) == right_of_segment && test.getState(corner, after) == right_of_segment);
}

int Pint::getIntersect(const Pint &A_S, const Pint &A_E, const Pint &B_S, const Pint &B_E, Pint &Result) {
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

double Pint::area(FLL<Pint> const &boundary) {
	auto focus = boundary.getHead();

	if (focus == nullptr) {
		return 0;
	}

	auto next = focus->getNext();

	if (next == nullptr) {
		return 0;
	}

	double total;

	do {
		Pint A = focus->getValue();
		Pint B = next->getValue();

		double width = B.X - A.X;
		double avg_height = (A.Y + B.Y) / 2;

		total += width * avg_height;

		focus = next;
		next = next->getNext();
	} while (next != nullptr);

	Pint A = focus->getValue();
	Pint B = boundary.getHead()->getValue();

	double width = B.X - A.X;
	double avg_height = (A.Y + B.Y) / 2;

	return total;
}