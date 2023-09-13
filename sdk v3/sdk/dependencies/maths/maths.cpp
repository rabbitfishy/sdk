#include "maths.h"
#include "../dependency.h"

game_maths* maths = new game_maths;

bool game_maths::world_to_screen(const vector_3d& origin, vector_2d& screen)
{
    view_matrix world_to_screen = interfaces->engine_client->world_to_screen_matrix();

    float width = world_to_screen[3][0] * origin.x + world_to_screen[3][1] * origin.y + world_to_screen[3][2] * origin.z + world_to_screen[3][3];

    if (width < 0.01f)
        return false;

    float inverse = 1.f / width;

    screen = { (world_to_screen[0][0] * origin.x + world_to_screen[0][1] * origin.y + world_to_screen[0][2] * origin.z + world_to_screen[0][3]) * inverse,
               (world_to_screen[1][0] * origin.x + world_to_screen[1][1] * origin.y + world_to_screen[1][2] * origin.z + world_to_screen[1][3]) * inverse };

    screen = { render->screen.w * 0.5f + screen.x * render->screen.w * 0.5f, render->screen.h * 0.5f - screen.y * render->screen.h * 0.5f };

    return true;
}

vector_3d game_maths::vector_transform(vector_3d& transform, matrix_3x4& matrix)
{
    return vector_3d(transform.dot(matrix[0]) + matrix[0][3],
        transform.dot(matrix[1]) + matrix[1][3],
        transform.dot(matrix[2]) + matrix[2][3]);
}

void game_maths::transform_vector(vector_3d& start, matrix_3x4& matrix, vector_3d& end)
{
    end = { start.dot(matrix[0]) + matrix[0][3],
            start.dot(matrix[1]) + matrix[1][3],
            start.dot(matrix[2]) + matrix[2][3] };
}

void game_maths::normalize_angle(float& angle)
{
    // bad number.
    if (!std::isfinite(angle))
    {
        angle = 0.f;
        return;
    }

    // no need to normalize this angle.
    if (angle >= -180.f && angle <= 180.f)
        return;

    // get amount of rotations needed.
    float rotate = std::round(std::abs(angle / 360.f));

    // normalize.
    angle = (angle < 0.f) ? angle + (360.f * rotate) : angle - (360.f * rotate);
}

void game_maths::angle_vectors(const q_angle& angles, vector_3d* forward, vector_3d* right, vector_3d* up)
{
    float cos_x = std::cos(deg_to_rad(angles.x)), sin_x = std::sin(deg_to_rad(angles.x));
    float cos_y = std::cos(deg_to_rad(angles.y)), sin_y = std::sin(deg_to_rad(angles.y));
    float cos_z = std::cos(deg_to_rad(angles.z)), sin_z = std::sin(deg_to_rad(angles.z));

    if (forward)
    {
        forward->x = cos_x * cos_y;
        forward->y = cos_x * sin_y;
        forward->z = -sin_x;
    }

    if (right)
    {
        right->x = -1.f * sin_z * sin_x * cos_y + -1.f * cos_z * -sin_y;
        right->y = -1.f * sin_z * sin_x * sin_y + -1.f * cos_z * cos_y;
        right->z = -1.f * sin_z * cos_x;
    }

    if (up)
    {
        up->x = cos_z * sin_x * cos_y + -sin_z * -sin_y;
        up->y = cos_z * sin_x * sin_y + -sin_z * cos_y;
        up->z = cos_z * cos_x;
    }
}

void game_maths::vector_angles(const vector_3d& forward, q_angle& angles)
{
    float   pitch   = { }, 
            yaw     = { };

    if (forward.x == 0.f && forward.y == 0.f)
    {
        pitch = (forward.z > 0.f) ? 270.f : 90.f;
        yaw = 0.f;
    }
    else
    {
        pitch = rad_to_deg(std::atan2f(-forward.z, forward.length_2d()));
        yaw = rad_to_deg(std::atan2f(forward.y, forward.x));

        normalize_angle(pitch);
        normalize_angle(yaw);
    }

    angles = { pitch, yaw, 0.f };
}

// credit: https://www.unknowncheats.me/forum/1146758-post1.html
q_angle game_maths::calculate_angle(const vector_3d& start, const vector_3d& end)
{
    q_angle angle = { };
    vector_3d delta = (start - end).normalized();
    vector_angles(delta, angle);
    return angle;
}