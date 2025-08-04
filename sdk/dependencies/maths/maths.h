#pragma once
#include "../../valve/maths/q_angle.h"
#include "../../valve/maths/vector.h"
#include "../../valve/maths/matrix.h"
#include "../../valve/maths/other.h"

class game_maths
{
public:
	bool world_to_screen(const vector_3d& origin, vector_2d& screen);
	vector_3d vector_transform(vector_3d& transform, matrix_3x4& matrix);
	matrix_3x4 angle_matrix(const q_angle& angle);

	void normalize_angle(float& angle);
	void angle_vectors(const q_angle& angles, vector_3d* forward, vector_3d* right = nullptr, vector_3d* up = nullptr);
	q_angle vector_angles(const vector_3d& relative);
	q_angle calculate_angle(const vector_3d& start, const vector_3d& end);
};

extern game_maths* maths;

