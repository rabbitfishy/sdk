#pragma once

// this is vector 2d but renamed it from 'vector' to 'point' for less confusion.
// its made only for drawing 2d objects and not for actual calculation.
class point
{
public:
	constexpr point() : x{ }, y{ } { }
	constexpr point(int x, int y) : x{ x }, y{ y } { }
	constexpr point(const point& p) : x{ p.x }, y{ p.y } { }

	// equality operators.
	bool operator==(const point& p) const { return p.x == this->x && p.y == this->y; }
	bool operator!=(const point& p) const { return p.x != this->x || p.y != this->y; }

	// copy assignment.
	constexpr point& operator=(const point& p)
	{
		this->x = p.x; this->y = p.y;
		return *this;
	}

	// negation-operator.
	constexpr point operator-() const { return point{ -this->x, -this->y }; }

	// arithmetic operators.
	point operator+(const point& p) const { return { this->x + p.x, this->y + p.y }; }
	point operator-(const point& p) const { return { this->x - p.x, this->y - p.y }; }
	point operator*(const point& p) const { return { this->x * p.x, this->y * p.y }; }
	point operator/(const point& p) const { return { this->x / p.x, this->y / p.y }; }

	int x, y;
};

class rect
{
public:
	constexpr rect() : x{ }, y{ }, w{ }, h{ } { }
	constexpr rect(int x, int y, int w, int h) : x{ x }, y{ y }, w{ w }, h{ h } { }
	int x, y, w, h;
};

class dimension
{
public:
	constexpr dimension() : w{ }, h{ } { }
	constexpr dimension(int w, int h) : w{ w }, h{ h } { }
	int w, h;
};
