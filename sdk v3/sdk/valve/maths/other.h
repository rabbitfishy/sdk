#pragma once

// this is a 2d vector made only for drawing menus and not actual calculation.
class vector
{
public:
	constexpr vector() : x{ }, y{ } { }
	constexpr vector(int x, int y) : x{ x }, y{ y } { }
	constexpr vector(const vector& v) : x{ v.x }, y{ v.y } { }

	// equality operators.
	bool operator==(const vector& v) const { return v.x == this->x && v.y == this->y; }
	bool operator!=(const vector& v) const { return v.x != this->x || v.y != this->y; }

	// copy assignment.
	constexpr vector& operator=(const vector& v)
	{
		this->x = v.x; this->y = v.y;
		return *this;
	}

	// negation-operator.
	constexpr vector operator-() const { return vector{ -this->x, -this->y }; }

	// arithmetic operators.
	vector operator+(const vector& v) const { return { this->x + v.x, this->y + v.y }; }
	vector operator-(const vector& v) const { return { this->x - v.x, this->y - v.y }; }
	vector operator*(const vector& v) const { return { this->x * v.x, this->y * v.y }; }
	vector operator/(const vector& v) const { return { this->x / v.x, this->y / v.y }; }

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