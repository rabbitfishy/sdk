#pragma once
#include <algorithm>
#include <cmath>
#include <limits>

class q_angle
{
public:
	constexpr q_angle() : x{ }, y{ }, z{ } { }
	constexpr q_angle(const float x, const float y, const float z) : x{ x }, y{ y }, z{ z } { }

	// index operators.
	[[nodiscard]] float& operator[](const std::size_t index) { return reinterpret_cast<float*>(this)[index]; }
	[[nodiscard]] const float& operator[](const std::size_t index) const { return reinterpret_cast<const float*>(this)[index]; }

	// equality operators.
	bool operator==(const q_angle& q) const { return q.x == this->x && q.y == this->y && q.z == this->z; }
	bool operator!=(const q_angle& q) const { return q.x != this->x || q.y != this->y || q.z != this->z; }

	// copy assignment.
	constexpr q_angle& operator=(const q_angle& q)
	{
		this->x = q.x; this->y = q.y; this->z = q.z;
		return *this;
	}

	// negation-operator.
	constexpr q_angle operator-() const { return q_angle{ -this->x, -this->y, -this->z }; }

	// arithmetic operators.
	q_angle operator+(const q_angle& q) const { return { this->x + q.x, this->y + q.y, this->z + q.z }; }
	q_angle operator-(const q_angle& q) const { return { this->x - q.x, this->y - q.y, this->z - q.z }; }
	q_angle operator*(const q_angle& q) const { return { this->x * q.x, this->y * q.y, this->z * q.z }; }
	q_angle operator/(const q_angle& q) const { return { this->x / q.x, this->y / q.y, this->z / q.z }; }

	// compound assignment operators.
	q_angle operator+=(const q_angle& q)
	{
		this->x += q.x; this->y += q.y; this->z += q.z;
		return *this;
	}

	q_angle operator-=(const q_angle& q)
	{
		this->x -= q.x; this->y -= q.y; this->z -= q.z;
		return *this;
	}

	q_angle operator*=(const q_angle& q)
	{
		this->x *= q.x; this->y *= q.y; this->z *= q.z;
		return *this;
	}

	q_angle operator/=(const q_angle& q)
	{
		this->x /= q.x; this->y /= q.y; this->z /= q.z;
		return *this;
	}

	// arithmetic operators w/ float.
	q_angle operator+(const float f) const { return { this->x + f, this->y + f, this->z + f }; }
	q_angle operator-(const float f) const { return { this->x - f, this->y - f, this->z - f }; }
	q_angle operator*(const float f) const { return { this->x * f, this->y * f, this->z * f }; }
	q_angle operator/(const float f) const { return { this->x / f, this->y / f, this->z / f }; }

	// compound assignment operators w/ float.
	q_angle& operator+=(float f)
	{
		this->x += f; this->y += f; this->z += f;
		return *this;
	}

	q_angle& operator-=(float f)
	{
		this->x -= f; this->y -= f; this->z -= f;
		return *this;
	}

	q_angle& operator*=(float f)
	{
		this->x *= f; this->y *= f; this->z *= f;
		return *this;
	}

	q_angle& operator/=(float f)
	{
		this->x /= f; this->y /= f; this->z /= f;
		return *this;
	}

	// modifier algorithms.
	void normalize()
	{
		this->x = std::isfinite(this->x) ? std::remainderf(this->x, 360.f) : 0.f;
		this->y = std::isfinite(this->y) ? std::remainderf(this->y, 360.f) : 0.f;
		this->z = std::clamp(this->z, -50.f, 50.f);
	}

	q_angle normalized()
	{
		q_angle normalized = *this;
		normalized.normalize();
		return normalized;
	}

	void clamp()
	{
		this->x = std::clamp(this->x, -89.f, 89.f);
		this->y = std::clamp(this->y, -180.f, 180.f);
		this->z = std::clamp(this->z, -50.f, 50.f);
	}

	q_angle clamped()
	{
		q_angle clamped = *this;
		clamped.clamp();
		return clamped;
	}

	q_angle sanitize() const
	{
		q_angle cleaned = *this;
		cleaned.normalized().clamped();
		return cleaned;
	}

	// extentions.
	[[nodiscard]] bool zero() const { return (this->x == 0.f && this->y == 0.f && this->z == 0.f); }

	[[nodiscard]] constexpr float length_sqr() const { return this->dot(*this); }
	[[nodiscard]] float length() const { return std::sqrtf(this->length_sqr()); }

	[[nodiscard]] constexpr float length_2d_sqr() const { return (this->x * this->x + this->y * this->y); }
	[[nodiscard]] float length_2d() const { return std::sqrtf(this->length_2d_sqr()); }

	[[nodiscard]] constexpr float dot(const q_angle& q) const { return (this->x * q.x + this->y * q.y + this->z * q.z); }

	float x, y, z;
};