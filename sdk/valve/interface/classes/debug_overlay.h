#pragma once

class game_overlay_text;

class game_debug_overlay
{
public:
	virtual void				add_entity_text_overlay(int entity_index, int line_offset, float duration, int r, int g, int b, int a, const char* fmt, ...) = 0;
	virtual void				add_box_overlay(const vector_3d& origin, const vector_3d& abs_min, const vector_3d& abs_max, const q_angle& orientation, int r, int g, int b, int a, float duration) = 0;
	virtual void				add_sphere_overlay(const vector_3d& origin, float radius, int theta, int phi, int r, int g, int b, int a, float duration) = 0;
	virtual void				add_triangle_overlay(const vector_3d& p1, const vector_3d& p2, const vector_3d& p3, int r, int g, int b, int a, bool no_depth_test, float duration) = 0;
	virtual void				add_line_overlay(const vector_3d& origin, const vector_3d& vecdest, int r, int g, int b, bool no_depth_test, float duration) = 0;
	virtual void				add_text_overlay(const vector_3d& origin, float duration, const char* fmt, ...) = 0;
	virtual void				add_text_overlay(const vector_3d& origin, int line_offset, float duration, const char* fmt, ...) = 0;
	virtual void				add_screen_text_overlay(float x_pos, float y_pos, float duration, int r, int g, int b, int a, const char* text) = 0;
	virtual void				add_swept_box_overlay(const vector_3d& start, const vector_3d& end, const vector_3d& mins, const vector_3d& maxs, const q_angle& angles, int r, int g, int b, int a, float duration) = 0;
	virtual void				add_grid_overlay(const vector_3d& origin) = 0;
	virtual void				add_coord_frame_overlay(const matrix_3x4& frame, float scale, int color_table[3][3] = nullptr) = 0;
	virtual int					screen_position(const vector_3d& point, vector_3d& screen) = 0;
	virtual int					screen_position(float x_pos, float y_pos, vector_3d& screen) = 0;
	virtual game_overlay_text*	get_first() = 0;
	virtual game_overlay_text*	get_next(game_overlay_text* current) = 0;
	virtual void				clear_dead_overlays() = 0;
	virtual void				clear_all_overlays() = 0;
	virtual void				add_text_overlay_rgb(const vector_3d& origin, int line_offset, float duration, float r, float g, float b, float a, const char* fmt, ...) = 0;
	virtual void				add_text_overlay_rgb(const vector_3d& origin, int line_offset, float duration, int r, int g, int b, int a, const char* fmt, ...) = 0;
	virtual void				add_line_overlay_alpha(const vector_3d& origin, const vector_3d& dest, int r, int g, int b, int a, bool no_depth_test, float duration) = 0;
	virtual void				add_box_overlay2(const vector_3d& origin, const vector_3d& abs_min, const vector_3d& abs_max, const q_angle& orientation, const color& face_color, const color& edge_color, float duration) = 0;
	virtual void				add_line_overlay(const vector_3d& origin, const vector_3d& vecdest, int r, int g, int b, int a, float thickness, float duration) = 0;
	virtual void				purge_text_overlays() = 0;
	virtual void				add_capsule_overlay(const vector_3d& abs_min, const vector_3d& abs_max, const float& radius, int r, int g, int b, int a, float duration) = 0;
	virtual void				draw_pill(vector_3d& abs_min, vector_3d& abs_max, float radius, int r, int g, int b, int a, float duration) = 0;
};