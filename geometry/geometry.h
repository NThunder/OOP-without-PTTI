#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>
#include <cstdio>
#include <vector>
#include <memory>
#include <string>
#include <cmath>
#include <algorithm>

namespace geometry {

class Point;
class Segment;
class Ray;
class Line;
class Circle;
class Polygon;

class Vector {

public:
	long long x_ = 0;
	long long y_ = 0;

	Vector() = default;

	Vector(long long f, long long s) : x_(f), y_(s) {};

	double Module () const;

	friend std::istream& operator>>(std::istream&, Vector&);
	friend std::ostream& operator<<(std::ostream&, const Vector&);

	Vector operator+(const Vector&) const;
	Vector operator-(const Vector&) const;
	Vector operator-() const;

	long long ScalarProduct(const Vector&) const;
	long long VectorProduct(const Vector&) const;
	long long AreaTriangle(const Vector&) const;

	bool operator==(const Vector&) const;

	double DistanceToSegment(const Vector&, const Vector&) const;

	bool BelongToBeam(const Vector&) const;
	bool BelongToSegment(const Vector&, const Vector&) const;

	bool IntersectionWithLine(const Vector&, const Vector&, const Vector&) const;
	bool IntersectionRayWithSegments(const Vector&, const Vector&, const Vector&) const;
	bool IntersectionSegments(const Vector&, const Vector&, const Vector&) const;

	bool ContainedIn(Vector*, long long) const;
	long long Direction(const Vector&, const Vector&) const;

	bool OnSegment(const Vector&, const Vector&) const;
};

class Point : public Vector {
public:
	Point() = default;
	Point(int64_t x, int64_t y) : Vector(x, y) {}
	Point& Move(const Vector&);
	bool ContainsPoint(const Point&) const;
	bool CrossesSegment(const Segment&) const;
	std::unique_ptr<Point> Clone() const;
	std::string ToString() const;
	Point operator-(const Point&) const;
};


class Segment {
public:
	Point first_;
	Point second_;
	Segment() : first_(), second_() {}
	Segment(const Point& f, const Point& s) : first_(f), second_(s) {}
	Segment& Move(const Vector&);
	bool ContainsPoint(const Point&) const;
	bool CrossesSegment(const Segment&) const;
	std::unique_ptr<Segment> Clone() const;
	std::string ToString() const;
};

class Ray {
	Point first_;
	Point second_;

public:
	Ray() : first_(), second_() {}
	Ray(const Point& f, const Point& s) : first_(f), second_(s - f) {}
	Ray& Move(const Vector&);
	bool ContainsPoint(const Point&) const;
	bool CrossesSegment(const Segment&) const;
	std::unique_ptr<Ray> Clone() const;
	std::string ToString() const;
};


class Line {
	Point first_;
	Point second_;

public:
	Line() : first_(), second_() {}
	Line(const Point& f, const Point& s) : first_(f), second_(s) {}
	Line& Move(const Vector&);
	bool ContainsPoint(const Point&) const;
	bool CrossesSegment(const Segment&) const;
	std::unique_ptr<Line> Clone() const;
	std::string ToString() const;
};

class Polygon {
	std::vector<Point> v_;

public:
	Polygon() : v_() {}
	Polygon(std::vector<Point>&& v) : v_(std::move(v)) {}
	Polygon& Move(const Vector&);
	bool ContainsPoint(const Point&) const;
	bool CrossesSegment(const Segment&) const;
	std::unique_ptr<Polygon> Clone() const;
	std::string ToString() const;
};


class Circle {
	Point center_;
	uint64_t radius_;

public:
	Circle() : center_() {}
	Circle(const Point& c, uint64_t r) : center_(c), radius_(r) {}
	Circle& Move(const Vector&);
	bool ContainsPoint(const Point&) const;
	bool ContainsPoint(const Point&, int) const;
	bool CrossesSegment(const Segment&) const;
	std::unique_ptr<Circle> Clone() const;
	std::string ToString() const;
};

}  // namespace geometry

#endif