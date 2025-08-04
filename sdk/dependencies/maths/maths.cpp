#include "maths.h"
#include "../dependency.h"

game_maths* maths = new game_maths;

bool game_maths::world_to_screen(const vector_3d& origin, vector_2d& screen)
{
    const view_matrix& world_to_screen = interfaces->engine_client->world_to_screen_matrix();

    const float width = world_to_screen[3][0] * origin.x + world_to_screen[3][1] * origin.y + world_to_screen[3][2] * origin.z + world_to_screen[3][3];

    if (width < 0.001f)
        return false;

    const float inverse = 1.f / width;

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

matrix_3x4 game_maths::angle_matrix(const q_angle& angle)
{
    float cos_x = std::cos(deg_to_rad(angle.x)), sin_x = std::sin(deg_to_rad(angle.x));
    float cos_y = std::cos(deg_to_rad(angle.y)), sin_y = std::sin(deg_to_rad(angle.y));
    float cos_z = std::cos(deg_to_rad(angle.z)), sin_z = std::sin(deg_to_rad(angle.z));

    return {
        cos_x * cos_y, sin_x * sin_z * cos_y - cos_z * sin_y, (sin_x * cos_z * cos_y + sin_z * sin_y), 0.f,
        cos_x * sin_y, sin_x * sin_z * sin_y + cos_z * cos_y, (sin_x * cos_z * sin_y - sin_z * cos_y), 0.f,
        -sin_x, sin_z * cos_x, cos_z * cos_x, 0.f
    };
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

// this the 'corrected' calculation for vector to angles conversion.
// credit: https://www.unknowncheats.me/forum/1147000-post18.html
q_angle game_maths::vector_angles(const vector_3d& relative)
{
    q_angle angle;
    float   pitch, yaw;

    if (relative.x == 0.f && relative.y == 0.f)
    {
        pitch = (relative.z > 0.f) ? 270.f : 90.f;
        yaw = 0.f;
    }
    else
    {
        // the hypotenuse is the magnitude of a vector.
        float magnitude = relative.length();

        // now remember pitch is the Y of our vector, and yaw is the X of our vector.
        // we have an X and Y, so we can use arctan, or inverse tangent, to find the yaw.
        yaw = rad_to_deg(std::atan2(relative.y, relative.x));
        this->normalize_angle(yaw);

        // we have Z and the hypotenuse, so we can use arcsin, or inverse sine, to find the pitch.
        pitch = rad_to_deg(std::asin(relative.z / magnitude)) * -1.f;
        this->normalize_angle(pitch);
    }

    // now you can just add this resulting angle to the current viewangles.
    angle.x = pitch, angle.y = yaw;
    return angle;
}

// this calculation is wrong but still helpful resource.
// credit: https://www.unknowncheats.me/forum/1146758-post1.html
q_angle game_maths::calculate_angle(const vector_3d& start, const vector_3d& end)
{
    // subtract our enemies position from our origin position to get the relative angle.
    vector_3d relative = end - start;
    return this->vector_angles(relative);
}

