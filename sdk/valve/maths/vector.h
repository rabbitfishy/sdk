#pragma once
#include <Windows.h>
#include <cmath>
#include <limits>

class vector_2d
{
public:
	constexpr vector_2d() : x{ }, y{ } { }
	constexpr vector_2d(const float x, const float y) : x{ x }, y{ y } { }

	// index operators.
	[[nodiscard]] float& operator[](const std::size_t index) { return reinterpret_cast<float*>(this)[index]; }
	[[nodiscard]] const float& operator[](const std::size_t index) const { return reinterpret_cast<const float*>(this)[index]; }

	// equality operators.
	bool operator==(const vector_2d& v) const { return v.x == this->x && v.y == this->y; }
	bool operator!=(const vector_2d& v) const { return v.x != this->x || v.y != this->y; }

	// copy assignment.
	constexpr vector_2d& operator=(const vector_2d& v)
	{
		this->x = v.x; this->y = v.y;
		return *this;
	}

	// negation-operator.
	constexpr vector_2d operator-() const { return vector_2d{ -this->x, -this->y }; }

	// arithmetic operators.
	vector_2d operator+(const vector_2d& v) const { return { this->x + v.x, this->y + v.y }; }
	vector_2d operator-(const vector_2d& v) const { return { this->x - v.x, this->y - v.y }; }
	vector_2d operator*(const vector_2d& v) const { return { this->x * v.x, this->y * v.y }; }
	vector_2d operator/(const vector_2d& v) const { return { this->x / v.x, this->y / v.y }; }

	// compound assignment operators.
	vector_2d operator+=(const vector_2d& v)
	{
		this->x += v.x; this->y += v.y;
		return *this;
	}

	vector_2d operator-=(const vector_2d& v)
	{
		this->x -= v.x; this->y -= v.y;
		return *this;
	}

	vector_2d operator*=(const vector_2d& v)
	{
		this->x *= v.x; this->y *= v.y;
		return *this;
	}

	vector_2d operator/=(const vector_2d& v)
	{
		this->x /= v.x; this->y /= v.y;
		return *this;
	}

	// arithmetic operators w/ float.
	vector_2d operator+(const float f) const { return { this->x + f, this->y + f }; }
	vector_2d operator-(const float f) const { return { this->x - f, this->y - f }; }
	vector_2d operator*(const float f) const { return { this->x * f, this->y * f }; }
	vector_2d operator/(const float f) const { return { this->x / f, this->y / f }; }

	// compound assignment operators w/ float.
	vector_2d& operator+=(float f)
	{
		this->x += f; this->y += f;
		return *this;
	}

	vector_2d& operator-=(float f)
	{
		this->x -= f; this->y -= f;
		return *this;
	}

	vector_2d& operator*=(float f)
	{
		this->x *= f; this->y *= f;
		return *this;
	}

	vector_2d& operator/=(float f)
	{
		this->x /= f; this->y /= f;
		return *this;
	}

	// extentions.
	[[nodiscard]] bool zero() const { return (this->x == 0.f && this->y == 0.f); }

	[[nodiscard]] constexpr float length_sqr() const { return (this->x * this->x + this->y * this->y); }
	[[nodiscard]] float length() const { return std::sqrtf(this->length_sqr()); }

	float x, y;
};

class vector_3d
{
public:
	constexpr vector_3d() : x{ }, y{ }, z{ } { }
	constexpr vector_3d(const float x, const float y, const float z) : x{ x }, y{ y }, z{ z } { }
	constexpr vector_3d(const float* data) : x{ data[0] }, y{ data[1] }, z{ data[2] } { }

	// index operators.
	[[nodiscard]] float& operator[](const std::size_t index) { return reinterpret_cast<float*>(this)[index]; }
	[[nodiscard]] const float& operator[](const std::size_t index) const { return reinterpret_cast<const float*>(this)[index]; }

	// equality operators.
	bool operator==(const vector_3d& v) const { return v.x == this->x && v.y == this->y && v.z == this->z; }
	bool operator!=(const vector_3d& v) const { return v.x != this->x || v.y != this->y || v.z != this->z; }

	// copy assignment.
	constexpr vector_3d& operator=(const vector_3d& v)
	{
		this->x = v.x; this->y = v.y; this->z = v.z;
		return *this;
	}

	// negation-operator.
	constexpr vector_3d operator-() const { return vector_3d{ -this->x, -this->y, -this->z }; }

	// arithmetic operators.
	vector_3d operator+(const vector_3d& v) const { return { this->x + v.x, this->y + v.y, this->z + v.z }; }
	vector_3d operator-(const vector_3d& v) const { return { this->x - v.x, this->y - v.y, this->z - v.z }; }
	vector_3d operator*(const vector_3d& v) const { return { this->x * v.x, this->y * v.y, this->z * v.z }; }
	vector_3d operator/(const vector_3d& v) const { return { this->x / v.x, this->y / v.y, this->z / v.z }; }

	// compound assignment operators.
	vector_3d operator+=(const vector_3d& v)
	{
		this->x += v.x; this->y += v.y; this->z += v.z;
		return *this;
	}

	vector_3d operator-=(const vector_3d& v)
	{
		this->x -= v.x; this->y -= v.y; this->z -= v.z;
		return *this;
	}

	vector_3d operator*=(const vector_3d& v)
	{
		this->x *= v.x; this->y *= v.y; this->z *= v.z;
		return *this;
	}

	vector_3d operator/=(const vector_3d& v)
	{
		this->x /= v.x; this->y /= v.y; this->z /= v.z;
		return *this;
	}

	// arithmetic operators w/ float.
	vector_3d operator+(const float f) const { return { this->x + f, this->y + f, this->z + f }; }
	vector_3d operator-(const float f) const { return { this->x - f, this->y - f, this->z - f }; }
	vector_3d operator*(const float f) const { return { this->x * f, this->y * f, this->z * f }; }
	vector_3d operator/(const float f) const { return { this->x / f, this->y / f, this->z / f }; }

	// compound assignment operators w/ float.
	vector_3d& operator+=(float f)
	{
		this->x += f; this->y += f; this->z += f;
		return *this;
	}

	vector_3d& operator-=(float f)
	{
		this->x -= f; this->y -= f; this->z -= f;
		return *this;
	}

	vector_3d& operator*=(float f)
	{
		this->x *= f; this->y *= f; this->z *= f;
		return *this;
	}

	vector_3d& operator/=(float f)
	{
		this->x /= f; this->y /= f; this->z /= f;
		return *this;
	}

	// modifier algorithms.
	float normalize()
	{
		const float length = this->length();
		const float radius = 1.f / (length + std::numeric_limits<float>::epsilon());

		this->x *= radius;
		this->y *= radius;
		this->z *= radius;

		return length;
	}

	[[nodiscard]] vector_3d normalized()
	{
		vector_3d normalized = *this;
		normalized.normalize();
		return normalized;
	}

	/*
	* Reminder!
	* function: std::pow(n, exponent -> sqaured, cubed or whatever exponential power inputted);
	* std::pow function basically does n ^ p.
	* where n = number that u want to sqaure or whatever, p = power is where the exponent value goes.
	*/

	// extentions.
	[[nodiscard]] bool zero() const { return (this->x == 0.f && this->y == 0.f && this->z == 0.f); }

	[[nodiscard]] constexpr float length_sqr() const { return this->dot(*this); }
	[[nodiscard]] float length() const { return std::sqrtf(this->length_sqr()); }

	[[nodiscard]] constexpr float length_2d_sqr() const { return (this->x * this->x + this->y * this->y); }
	[[nodiscard]] float length_2d() const { return std::sqrtf(this->length_2d_sqr()); }

	[[nodiscard]] constexpr float dot(const vector_3d& v) const { return (this->x * v.x + this->y * v.y + this->z * v.z); }
	[[nodiscard]] constexpr vector_3d cross(const vector_3d& v) const { return { this->y * v.z - this->z * v.y, this->z * v.x - this->x * v.z, this->x * v.y - this->y * v.x }; }

	float x, y, z;
};

class vector_4d
{
public:
	constexpr vector_4d() : x{ }, y{ }, z{ }, w{ } { }
	constexpr vector_4d(const float x, const float y, const float z, const float w) : x{ x }, y{ y }, z{ z }, w{ w } { }
	float x, y, z, w;
};

class alignas(16) vector_aligned : public vector_3d
{
public:
	vector_aligned() : w{ } { }
	explicit vector_aligned(const vector_3d& v) { this->x = v.x, this->y = v.y, this->z = v.z, this->w = 0.f; }

	constexpr vector_aligned& operator=(const vector_3d& v)
	{
		this->x = v.x, this->y = v.y, this->z = v.z, this->w = 0.f;
		return *this;
	}

	float w;
};

// NOTE: it must be float values for it to render otherwise shit won't render anything in directx.
// that is why i put it here to use vector 2d's to render so it must be float value!!!!
class vertex
{
public:
	vertex() : position{ }, coordinate{ }, colour{ } { }
	vertex(const vector_2d& position, const vector_2d& coordinate, const DWORD colour)
	{
		this->position = position;
		this->coordinate = coordinate;
		this->colour = colour;
	}

	vector_2d	position;
	vector_2d	coordinate;
	DWORD		colour;
};