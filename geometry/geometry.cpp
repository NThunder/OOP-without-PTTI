#include "geometry.h"

using namespace geometry;

double Vector::Module () const {
	return pow(x_ * x_ + y_ * y_, 0.5);
}

Vector Vector::operator+(const Vector& a) const {
	return {x_ + a.x_, y_ + a.y_};
}

bool Vector::operator==(const Vector& a) const {
	return x_ == a.x_ && y_ == a.y_;
}

Vector Vector::operator-(const Vector& a) const {
	return {x_ - a.x_, y_ - a.y_};
}

Vector Vector::operator-() const {
	return {-x_, -y_};
}

std::istream& operator>>(std::istream& is, Vector& a) {
	is >> a.x_ >> a.y_;
	return is;
}

std::ostream& operator<<(std::ostream& os, const Vector& a) {
	os << a.x_ << " " << a.y_;
	return os;
}

long long Vector::ScalarProduct(const Vector& a) const {
	return x_ * a.x_ + y_ * a.y_;
}

long long Vector::VectorProduct(const Vector& a) const {
	return x_ * a.y_ - y_ * a.x_;
}

long long Vector::AreaTriangle(const Vector& a) const {
	return std::abs(VectorProduct(a) * 0.5);
}

bool Vector::IntersectionWithLine(const Vector& B, const Vector& C, const Vector& D) const {
	Vector A = *this, AB{B - A}, N{-AB.y_, AB.x_}, AC{C - A}, AD{D - A};
	if (AD.ScalarProduct(N) * AC.ScalarProduct(N) <= 0.0) {
		return true;
	}
	return false;
}

long long Vector::Direction(const Vector& A, const Vector& B) const {
	return (*this - A).VectorProduct(B - A);
}

bool Vector::OnSegment(const Vector& A, const Vector& B) const {
	return ((std::min(A.x_, B.x_) <= this->x_ && this->x_ <= std::max(A.x_, B.x_)) &&
			(std::min(A.y_, B.y_) <= this->y_ && this->y_ <= std::max(A.y_, B.y_)));
}

bool Vector::IntersectionSegments(const Vector& B, const Vector& C, const Vector& D) const {
	Vector A = *this;
	long long d1 = A.Direction(C, D);
	long long d2 = B.Direction(C, D);
	long long d3 = C.Direction(A, B);
	long long d4 = D.Direction(A, B);
	if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) && ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) {
		return true;
	}
	if (d1 == 0 && A.OnSegment(C, D)) {
		return true;
	}
	if (d2 == 0 && B.OnSegment(C, D)) {
		return true;
	}
	if (d3 == 0 && C.OnSegment(A, B)) {
		return true;
	}
	if (d4 == 0 && D.OnSegment(A, B)) {
		return true;
	}
	return false;
}

double Vector::DistanceToSegment(const Vector& A, const Vector& B) const {
	Vector point = *this - A;
	Vector AB = B - A;
	Vector BA = -AB;
	if (point.ScalarProduct(AB) >= 0) {
		point = *this - B;
		if (point.ScalarProduct(BA) >= 0) {
			return std::abs(point.VectorProduct(BA) / BA.Module());
		} else {
			return point.Module();
		}
	} else {
		return point.Module();
	}
}

bool Vector::BelongToBeam(const Vector& a) const {
	if (VectorProduct(a) == 0) {
		if (ScalarProduct(a) >= 0) {
			return true;	
		}
	}
	return false;
}

bool Vector::BelongToSegment(const Vector& A, const Vector& B) const {
	Vector AB = B - A;
	Vector BA = A - B;
	Vector point = *this - A;
	if (A == B) {
		if (*this == A) {
			return true;
		}
		return false;
	}
	if (point.VectorProduct(AB) == 0) {
		if (point.ScalarProduct(AB) >= 0) {
			point = *this - B;
			if (point.ScalarProduct(BA) >= 0) {
				return true;
			}
		}
	}
	return false;
}

bool Vector::IntersectionRayWithSegments(const Vector& s_p, const Vector& f_m, const Vector& s_m) const {
	Vector p = s_p - *this;
	if ((f_m - *this).BelongToBeam(p)) {
		return true;
	}
	if ((s_m - *this).BelongToBeam(p)) {
		return true;
	}
	if (this->BelongToSegment(f_m, s_m)) {
		return true;
	}
	if ((f_m - *this).VectorProduct(s_m - f_m) * (p).VectorProduct(s_m - f_m) >= 0) {
		return true;
	}
	return false;
}

bool Vector::ContainedIn(Vector* m, long long n) const {
	Vector f_p = *this;
	Vector s_p{f_p.x_ + 1, f_p.y_};
	Vector p = s_p - f_p;
	for (long long i = 0; i < n; ++i) {
		if (f_p.BelongToSegment(m[i], m[(i + 1) % n])) {
			return true;
		}
	}
	long long numb_o_inters = 0;
	for (long long i = 0; i < n; ++i) {
		bool f_m = false;
		bool s_m = false;
		Vector point = m[i] - f_p;
		if (point.BelongToBeam(p)) {
			f_m = true;
		}
		point = m[(i + 1) % n]  - f_p;
		if (point.BelongToBeam(p)) {
			s_m = true;
		}
		if (f_m && s_m) {
			continue;
		} else if (f_m) {
			if (m[i].y_ > m[(i + 1) % n].y_) {
				++numb_o_inters;
			}
		} else if (s_m) {
			if (m[i].y_ < m[(i + 1) % n].y_) {
				++numb_o_inters;
			}
		} else {
			if (f_p.IntersectionWithLine(s_p, m[i], m[(i + 1) % n])) {
				if ((m[i] - f_p).VectorProduct(m[(i + 1) % n] - m[i]) * (s_p - f_p).VectorProduct(m[(i + 1) % n] - m[i]) > 0) {
					++numb_o_inters;
				}
			}
		}
	}
	if (numb_o_inters % 2 == 1) {
		return true;
	} else {
		return false;
	}
}

long double AreaPolygon(std::vector<Vector>& shell) {
    long double res = 0.0;
    for (size_t i = 1; i <= shell.size(); ++i) {
        res += shell[i - 1].VectorProduct(shell[i % shell.size()]);
    }
    return std::abs(res / 2.0);
}

Vector p0;
void ConvexHull(std::vector<Vector>& v) {
	p0 = v[0];
	for (size_t i = 1; i < v.size(); ++i) {
		if (v[i].x_ < p0.x_ || (v[i].x_ == p0.x_ && v[i].y_ < p0.y_)) {
			p0 = v[i];
		}
	}
    struct {
        bool operator() (Vector a, Vector b) const {
        	if ((a - p0).VectorProduct(b - p0) < 0) {
        		return true;
        	}
        	if ((a - p0).VectorProduct(b - p0) == 0) {
        		return (a - p0).Module() < (b - p0).Module();
        	}
            return false;
        }
    } Compare; 
	std::sort(v.begin(), v.end(), Compare);
    std::vector<Vector> shell;
    shell.push_back(p0);
    size_t index = 0;
    while (p0 == v[index] && index < v.size()) {
        ++index;
    }
    shell.push_back(v[index]);
    Vector pprev = shell[shell.size() - 2];
    Vector prev = shell[shell.size() - 1];
    for (size_t i = index + 1; i < v.size(); ++i) {
        if (v[i] == prev) {
            continue;
        }
        while (shell.size() >= 2 && (prev - pprev).VectorProduct(v[i] - pprev) > 0) {
            shell.pop_back();
            pprev = shell[shell.size() - 2];
            prev = shell[shell.size() - 1];
        }
        if ((prev - pprev).VectorProduct(v[i] - pprev) == 0) {
            shell.pop_back();
            shell.push_back(v[i]);
            prev = v[i];
        } else {
            shell.push_back(v[i]);
            pprev = prev;
            prev = v[i];
        }
    }
    printf("%lu\n", shell.size());
    for (auto& a : shell) {
        printf("%lld %lld\n", a.x_, a.y_);
    }
    printf("%0.1Lf\n", AreaPolygon(shell));
}

Point Point::operator-(const Point& p) const {
	return Point(x_ - p.x_, y_ - p.y_);
}

Point& Point::Move(const Vector& v) {
	this->x_ = this->x_ + v.x_;
	this->y_ = this->y_ + v.y_;
	return *this;
}

bool Point::ContainsPoint(const Point& p) const {
	return *this == p;
}

std::unique_ptr<Point> Point::Clone() const {
	return std::make_unique<Point>(*this);
}

std::string Point::ToString() const {
	return "Point(" + std::to_string(x_) + "," + " " + std::to_string(y_) + ")";
}

bool Point::CrossesSegment(const Segment& s) const {
	return BelongToSegment(s.first_, s.second_);
}

Segment& Segment::Move(const Vector& v) {
	first_.Move(v);
	second_.Move(v);
	return *this;
}

bool Segment::ContainsPoint(const Point& p) const {
	return p.CrossesSegment(*this);
}

bool Segment::CrossesSegment(const Segment& s) const {
	return first_.IntersectionSegments(second_, s.first_, s.second_);
}

std::unique_ptr<Segment> Segment::Clone() const {
	return std::make_unique<Segment>(*this);
}

std::string Segment::ToString() const {
	return "Segment(" + first_.ToString() + "," + " " + second_.ToString() + ")";
}

Ray& Ray::Move(const Vector& v) {
	first_.Move(v);
	return *this;
}

bool Ray::ContainsPoint(const Point& p) const {
	return (p - first_).BelongToBeam(second_);
}

bool Ray::CrossesSegment(const Segment& s) const {
	return first_.IntersectionRayWithSegments(second_ + first_, s.first_, s.second_);
}

std::unique_ptr<Ray> Ray::Clone() const {
	return std::make_unique<Ray>(*this);
}

std::string Ray::ToString() const {
	return "Ray(" + first_.ToString() + "," + " " + "Vector(" + std::to_string(second_.x_) + "," + " " + std::to_string(second_.y_) + ")" + ")";
}

Line& Line::Move(const Vector& v) {
	first_.Move(v);
	second_.Move(v);
	return *this;
}

bool Line::ContainsPoint(const Point& p) const {
	return (second_ - first_).VectorProduct(p - first_) == 0;
}

bool Line::CrossesSegment(const Segment& s) const {
	return first_.IntersectionWithLine(second_, s.first_, s.second_);
}

std::unique_ptr<Line> Line::Clone() const {
	return std::make_unique<Line>(*this);
}

std::string Line::ToString() const {
	Point r = second_ - first_;
	int64_t a = r.y_;
	int64_t b = -r.x_;
	int64_t c = first_.VectorProduct(second_);
	return "Line(" + std::to_string(a) + "," + " " + std::to_string(b) + "," + " " + std::to_string(-c) + ")";
}

Polygon& Polygon::Move(const Vector& v) {
	for (auto& a : v_) {
		a.Move(v);
	}
	return *this;
}

bool Polygon::ContainsPoint(const Point& p) const {
	Vector m[v_.size()];
	for (size_t i = 0; i < v_.size(); ++i) {
		m[i] = v_[i];
	}
	return p.ContainedIn(m, v_.size());
}

bool Polygon::CrossesSegment(const Segment& s) const {
	for (size_t i = 0; i < v_.size(); ++i) {
		if (s.first_.IntersectionSegments(s.second_, v_[i], v_[(i + 1) % v_.size()])) {
			return true;
		}
	}
	return false;
}

std::unique_ptr<Polygon> Polygon::Clone() const {
	return std::make_unique<Polygon>(*this);
}

std::string Polygon::ToString() const {
	std::string str = "Polygon(";
	for (size_t i = 0; i < v_.size(); ++i) {
		str += v_[i].ToString();
		if (i + 1 !=  v_.size()) {
			str +=  ", ";
		}
	}
	str += ")\n";
	return str;
}

Circle& Circle::Move(const Vector& v) {
	center_.Move(v);
	return *this;
}

bool Circle::ContainsPoint(const Point& p) const {
	return (p - center_).Module() <= static_cast<double>(radius_);
}

bool Circle::ContainsPoint(const Point& p, int) const {
	return (p - center_).Module() < static_cast<double>(radius_);
}

bool Circle::CrossesSegment(const Segment& s) const {
	if (ContainsPoint(s.first_, 1) && ContainsPoint(s.second_, 1)) {
		return false;
	}
	if (center_.DistanceToSegment(s.first_, s.second_) <= static_cast<double>(radius_)) {
		return true;
	}
	return false;
}

std::unique_ptr<Circle> Circle::Clone() const {
	return std::make_unique<Circle>(*this);
}

std::string Circle::ToString() const {
	return "Circle(" + center_.ToString() + ", " + std::to_string(radius_) + ")\n";
}

int main() {
	return 0;
}