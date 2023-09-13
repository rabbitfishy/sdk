#pragma once
#include <corecrt_math_defines.h>
#include <xmmintrin.h>
#include <assert.h>
#include <bit>

// degrees to radians.
#define deg_to_rad( value ) ( ( value ) * ( ( const float )M_PI / 180.f ) )

// radians to degrees.
#define rad_to_deg( value ) ( ( value ) * ( 180.f / ( const float )M_PI ) )

using matrix_3x3 = float[3][3];

class matrix_3x4
{
public:
	constexpr matrix_3x4() : data{ } { }

	constexpr matrix_3x4(
		const float m00, const float m01, const float m02, const float m03,
		const float m10, const float m11, const float m12, const float m13,
		const float m20, const float m21, const float m22, const float m23)
	{
		this->data[0][0] = m00; this->data[0][1] = m01; this->data[0][2] = m02; this->data[0][3] = m03;
		this->data[1][0] = m10; this->data[1][1] = m11; this->data[1][2] = m12; this->data[1][3] = m13;
		this->data[2][0] = m20; this->data[2][1] = m21; this->data[2][2] = m22; this->data[2][3] = m23;
	}

	constexpr matrix_3x4(const vector_3d& forward, const vector_3d& left, const vector_3d& up, const vector_3d& origin)
	{
		this->set_forward(forward);
		this->set_left(left);
		this->set_up(up);
		this->set_origin(origin);
	}

	[[nodiscard]] float* operator[](const int index) { return this->data[index]; }
	[[nodiscard]] const float* operator[](const int index) const { return this->data[index]; }

	constexpr void set_forward(const vector_3d& forward)
	{
		this->data[0][0] = forward.x;
		this->data[1][0] = forward.y;
		this->data[2][0] = forward.z;
	}

	constexpr void set_left(const vector_3d& left)
	{
		this->data[0][1] = left.x;
		this->data[1][1] = left.y;
		this->data[2][1] = left.z;
	}

	constexpr void set_up(const vector_3d& up)
	{
		this->data[0][2] = up.x;
		this->data[1][2] = up.y;
		this->data[2][2] = up.z;
	}

	constexpr void set_origin(const vector_3d& origin)
	{
		this->data[0][3] = origin.x;
		this->data[1][3] = origin.y;
		this->data[2][3] = origin.z;
	}

	[[nodiscard]] constexpr vector_3d get_forward() const { return { this->data[0][0], this->data[1][0], this->data[2][0] }; }
	[[nodiscard]] constexpr vector_3d get_left() const { return { this->data[0][1], this->data[1][1], this->data[2][1] }; }
	[[nodiscard]] constexpr vector_3d get_up() const { return { this->data[0][2], this->data[1][2], this->data[2][2] }; }
	[[nodiscard]] constexpr vector_3d get_origin() const { return { this->data[0][3], this->data[1][3], this->data[2][3] }; }

	constexpr void invalidate()
	{
		for (auto& sub_data : this->data)
		{
			for (auto& float_data : sub_data)
				float_data = std::numeric_limits<float>::infinity();
		}
	}

	// concatenate transformations of two matrices into one.
	[[nodiscard]] constexpr matrix_3x4 concat_transforms(const matrix_3x4& m) const
	{
		return
		{
			this->data[0][0] * m.data[0][0] + this->data[0][1] * m.data[1][0] + this->data[0][2] * m.data[2][0],
			this->data[0][0] * m.data[0][1] + this->data[0][1] * m.data[1][1] + this->data[0][2] * m.data[2][1],
			this->data[0][0] * m.data[0][2] + this->data[0][1] * m.data[1][2] + this->data[0][2] * m.data[2][2],
			this->data[0][0] * m.data[0][3] + this->data[0][1] * m.data[1][3] + this->data[0][2] * m.data[2][3] + this->data[0][3],

			this->data[1][0] * m.data[0][0] + this->data[1][1] * m.data[1][0] + this->data[1][2] * m.data[2][0],
			this->data[1][0] * m.data[0][1] + this->data[1][1] * m.data[1][1] + this->data[1][2] * m.data[2][1],
			this->data[1][0] * m.data[0][2] + this->data[1][1] * m.data[1][2] + this->data[1][2] * m.data[2][2],
			this->data[1][0] * m.data[0][3] + this->data[1][1] * m.data[1][3] + this->data[1][2] * m.data[2][3] + this->data[1][3],

			this->data[2][0] * m.data[0][0] + this->data[2][1] * m.data[1][0] + this->data[2][2] * m.data[2][0],
			this->data[2][0] * m.data[0][1] + this->data[2][1] * m.data[1][1] + this->data[2][2] * m.data[2][1],
			this->data[2][0] * m.data[0][2] + this->data[2][1] * m.data[1][2] + this->data[2][2] * m.data[2][2],
			this->data[2][0] * m.data[0][3] + this->data[2][1] * m.data[1][3] + this->data[2][2] * m.data[2][3] + this->data[2][3]
		};
	}

	// angles converted from this matrix.
	[[nodiscard]] q_angle angles() const
	{
		// extract the basis vectors from the matrix. since we only need the z component of the up vector, we don't get x and y.
		const vector_3d forward = this->get_forward();
		const vector_3d left = this->get_left();
		const float up_z = this->data[2][2];

		const float length_2d = forward.length_2d();
		const float pitch = rad_to_deg(std::atan2f(-forward.z, length_2d));

		// check is enough here to get angles.
		if (length_2d > 0.001f)
			return { pitch, rad_to_deg(std::atan2f(forward.y, forward.x)), rad_to_deg(std::atan2f(left.z, up_z)) };

		// forward is mostly z, gimbal lock.
		// assume no roll in this case as one degree of freedom has been lost (i.e. yaw equals roll).
		return { pitch, rad_to_deg(std::atan2f(-left.x, left.y)), 0.f };
	}

	float data[3][4];
};

class alignas(16) matrix_aligned : public matrix_3x4
{
public:
	constexpr matrix_aligned() { }

	constexpr matrix_aligned(
		const float m00, const float m01, const float m02, const float m03,
		const float m10, const float m11, const float m12, const float m13,
		const float m20, const float m21, const float m22, const float m23)
	{
		this->data[0][0] = m00; this->data[0][1] = m01; this->data[0][2] = m02; this->data[0][3] = m03;
		this->data[1][0] = m10; this->data[1][1] = m11; this->data[1][2] = m12; this->data[1][3] = m13;
		this->data[2][0] = m20; this->data[2][1] = m21; this->data[2][2] = m22; this->data[2][3] = m23;
	}

	constexpr matrix_aligned(const matrix_3x4& m)
	{
		*this = m;
	}

	constexpr matrix_aligned& operator=(const matrix_3x4& m)
	{
		this->data[0][0] = m.data[0][0]; this->data[0][1] = m.data[0][1]; this->data[0][2] = m.data[0][2]; this->data[0][3] = m.data[0][3];
		this->data[1][0] = m.data[1][0]; this->data[1][1] = m.data[1][1]; this->data[1][2] = m.data[1][2]; this->data[1][3] = m.data[1][3];
		this->data[2][0] = m.data[2][0]; this->data[2][1] = m.data[2][1]; this->data[2][2] = m.data[2][2]; this->data[2][3] = m.data[2][3];
		return *this;
	}

	// concatenate transformations of two aligned matrices into one.
	[[nodiscard]] matrix_aligned concat_transforms(const matrix_aligned& m) const
	{
		matrix_aligned output;
		assert((reinterpret_cast<std::uintptr_t>(this) & 15u) == 0 && (reinterpret_cast<std::uintptr_t>(&m) & 15u) == 0 && (reinterpret_cast<std::uintptr_t>(&output) & 15u) == 0); // matrices aren't aligned

		__m128 this_row0 = _mm_load_ps(this->data[0]);
		__m128 this_row1 = _mm_load_ps(this->data[1]);
		__m128 this_row2 = _mm_load_ps(this->data[2]);

		__m128 other_row0 = _mm_load_ps(m.data[0]);
		__m128 other_row1 = _mm_load_ps(m.data[1]);
		__m128 other_row2 = _mm_load_ps(m.data[2]);

		__m128 out_row0 = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(this_row0, this_row0, _MM_SHUFFLE(0, 0, 0, 0)), other_row0), _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(this_row0, this_row0, _MM_SHUFFLE(1, 1, 1, 1)), other_row1), _mm_mul_ps(_mm_shuffle_ps(this_row0, this_row0, _MM_SHUFFLE(2, 2, 2, 2)), other_row2)));
		__m128 out_row1 = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(this_row1, this_row1, _MM_SHUFFLE(0, 0, 0, 0)), other_row0), _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(this_row1, this_row1, _MM_SHUFFLE(1, 1, 1, 1)), other_row1), _mm_mul_ps(_mm_shuffle_ps(this_row1, this_row1, _MM_SHUFFLE(2, 2, 2, 2)), other_row2)));
		__m128 out_row2 = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(this_row2, this_row2, _MM_SHUFFLE(0, 0, 0, 0)), other_row0), _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(this_row2, this_row2, _MM_SHUFFLE(1, 1, 1, 1)), other_row1), _mm_mul_ps(_mm_shuffle_ps(this_row2, this_row2, _MM_SHUFFLE(2, 2, 2, 2)), other_row2)));

		// add in translation vector
		constexpr std::uint32_t component_mask[4] = { 0x0, 0x0, 0x0, 0xFFFFFFFF };
		out_row0 = _mm_add_ps(out_row0, _mm_and_ps(this_row0, std::bit_cast<__m128>(component_mask)));
		out_row1 = _mm_add_ps(out_row1, _mm_and_ps(this_row1, std::bit_cast<__m128>(component_mask)));
		out_row2 = _mm_add_ps(out_row2, _mm_and_ps(this_row2, std::bit_cast<__m128>(component_mask)));

		_mm_store_ps(output.data[0], out_row0);
		_mm_store_ps(output.data[1], out_row1);
		_mm_store_ps(output.data[2], out_row2);
		return output;
	}
};

class view_matrix
{
public:
	constexpr view_matrix() : data{ } { }

	constexpr view_matrix(
		const float m00, const float m01, const float m02, const float m03,
		const float m10, const float m11, const float m12, const float m13,
		const float m20, const float m21, const float m22, const float m23,
		const float m30, const float m31, const float m32, const float m33)
	{
		this->data[0][0] = m00; this->data[0][1] = m01; this->data[0][2] = m02; this->data[0][3] = m03;
		this->data[1][0] = m10; this->data[1][1] = m11; this->data[1][2] = m12; this->data[1][3] = m13;
		this->data[2][0] = m20; this->data[2][1] = m21; this->data[2][2] = m22; this->data[2][3] = m23;
		this->data[3][0] = m30; this->data[3][1] = m31; this->data[3][2] = m32; this->data[3][3] = m33;
	}

	constexpr view_matrix(const matrix_3x4& m, const vector_4d& additional_row = { })
	{
		this->data[0][0] = m.data[0][0]; this->data[0][1] = m.data[0][1]; this->data[0][2] = m.data[0][2]; this->data[0][3] = m.data[0][3];
		this->data[1][0] = m.data[1][0]; this->data[1][1] = m.data[1][1]; this->data[1][2] = m.data[1][2]; this->data[1][3] = m.data[1][3];
		this->data[2][0] = m.data[2][0]; this->data[2][1] = m.data[2][1]; this->data[2][2] = m.data[2][2]; this->data[2][3] = m.data[2][3];
		this->data[3][0] = additional_row.x; this->data[3][1] = additional_row.y; this->data[3][2] = additional_row.z; this->data[3][3] = additional_row.w;
	}

	[[nodiscard]] float* operator[](const int index) { return this->data[index]; }
	[[nodiscard]] const float* operator[](const int index) const { return this->data[index]; }

	[[nodiscard]] const matrix_3x4& as_3x4() const { return *reinterpret_cast<const matrix_3x4*>(this); }
	[[nodiscard]] matrix_3x4& as_3x4() { return *reinterpret_cast<matrix_3x4*>(this); }

	constexpr view_matrix& operator+=(const view_matrix& m)
	{
		for (std::uint8_t c = 0U; c < 4U; c++)
		{
			for (std::uint8_t r = 0U; r < 4U; r++)
				this->data[c][r] += m[c][r];
		}

		return *this;
	}

	constexpr view_matrix& operator-=(const view_matrix& m)
	{
		for (std::uint8_t c = 0U; c < 4U; c++)
		{
			for (std::uint8_t r = 0U; r < 4U; r++)
				this->data[c][r] -= m[c][r];
		}

		return *this;
	}

	[[nodiscard]] constexpr vector_4d get_row(const int index) const { return { this->data[index][0], this->data[index][1], this->data[index][2], this->data[index][3] }; }
	[[nodiscard]] constexpr vector_4d get_column(const int index) const { return { this->data[0][index], this->data[1][index], this->data[2][index], this->data[3][index] }; }

	constexpr void identity()
	{
		for (std::uint8_t c = 0U; c < 4U; c++)
		{
			for (std::uint8_t r = 0U; r < 4U; r++)
				this->data[c][r] = (c == r) ? 1.f : 0.f;
		}
	}

	// concatenate transformations of two matrices into one.
	[[nodiscard]] constexpr view_matrix concat_transforms(const view_matrix& m) const
	{
		return
		{
			this->data[0][0] * m.data[0][0] + this->data[0][1] * m.data[1][0] + this->data[0][2] * m.data[2][0] + this->data[0][3] * m.data[3][0],
			this->data[0][0] * m.data[0][1] + this->data[0][1] * m.data[1][1] + this->data[0][2] * m.data[2][1] + this->data[0][3] * m.data[3][1],
			this->data[0][0] * m.data[0][2] + this->data[0][1] * m.data[1][2] + this->data[0][2] * m.data[2][2] + this->data[0][3] * m.data[3][2],
			this->data[0][0] * m.data[0][3] + this->data[0][1] * m.data[1][3] + this->data[0][2] * m.data[2][3] + this->data[0][3] * m.data[3][3],

			this->data[1][0] * m.data[0][0] + this->data[1][1] * m.data[1][0] + this->data[1][2] * m.data[2][0] + this->data[1][3] * m.data[3][0],
			this->data[1][0] * m.data[0][1] + this->data[1][1] * m.data[1][1] + this->data[1][2] * m.data[2][1] + this->data[1][3] * m.data[3][1],
			this->data[1][0] * m.data[0][2] + this->data[1][1] * m.data[1][2] + this->data[1][2] * m.data[2][2] + this->data[1][3] * m.data[3][2],
			this->data[1][0] * m.data[0][3] + this->data[1][1] * m.data[1][3] + this->data[1][2] * m.data[2][3] + this->data[1][3] * m.data[3][3],

			this->data[2][0] * m.data[0][0] + this->data[2][1] * m.data[1][0] + this->data[2][2] * m.data[2][0] + this->data[2][3] * m.data[3][0],
			this->data[2][0] * m.data[0][1] + this->data[2][1] * m.data[1][1] + this->data[2][2] * m.data[2][1] + this->data[2][3] * m.data[3][1],
			this->data[2][0] * m.data[0][2] + this->data[2][1] * m.data[1][2] + this->data[2][2] * m.data[2][2] + this->data[2][3] * m.data[3][2],
			this->data[2][0] * m.data[0][3] + this->data[2][1] * m.data[1][3] + this->data[2][2] * m.data[2][3] + this->data[2][3] * m.data[3][3],

			this->data[3][0] * m.data[0][0] + this->data[3][1] * m.data[1][0] + this->data[3][2] * m.data[2][0] + this->data[3][3] * m.data[3][0],
			this->data[3][0] * m.data[0][1] + this->data[3][1] * m.data[1][1] + this->data[3][2] * m.data[2][1] + this->data[3][3] * m.data[3][1],
			this->data[3][0] * m.data[0][2] + this->data[3][1] * m.data[1][2] + this->data[3][2] * m.data[2][2] + this->data[3][3] * m.data[3][2],
			this->data[3][0] * m.data[0][3] + this->data[3][1] * m.data[1][3] + this->data[3][2] * m.data[2][3] + this->data[3][3] * m.data[3][3]
		};
	}

	float data[4][4];
};