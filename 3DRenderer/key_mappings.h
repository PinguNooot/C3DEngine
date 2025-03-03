// KeyMappings.h

#ifndef KEYMAPPINGS_H
#define KEYMAPPINGS_H

#include <SDL.h>

///////////////////////////////////////////////////////////////////////////////
// Key Mappings
///////////////////////////////////////////////////////////////////////////////

// Rendering
extern SDL_Keycode key_render_wire_vertex;
extern SDL_Keycode key_render_wire;
extern SDL_Keycode key_render_fill_triangle;
extern SDL_Keycode key_render_fill_triangle_wire;
extern SDL_Keycode key_render_textured;
extern SDL_Keycode key_render_textured_wire;

// Culling
extern SDL_Keycode key_cull_backface;
extern SDL_Keycode key_cull_none;

// Camera Rotation
extern SDL_Keycode key_rotate_camera_pitch_up;
extern SDL_Keycode key_rotate_camera_pitch_down;
extern SDL_Keycode key_rotate_camera_yaw_left;
extern SDL_Keycode key_rotate_camera_yaw_right;

// Camera Movement
extern SDL_Keycode key_move_camera_forward;
extern SDL_Keycode key_move_camera_backward;
extern SDL_Keycode key_move_camera_left;
extern SDL_Keycode key_move_camera_right;
extern SDL_Keycode key_move_camera_up;
extern SDL_Keycode key_move_camera_down;
extern SDL_Keycode key_move_camera_accelerate;
extern SDL_Keycode key_move_camera_deccelerate;

// Other
extern SDL_Keycode key_toggle_fullscreen;
extern SDL_Keycode key_toggle_mouse_capture;
extern SDL_Keycode key_toggle_fps;

// Mesh Controls
extern SDL_Keycode key_swap_mesh;
extern SDL_Keycode key_swap_to_mesh_one;
extern SDL_Keycode key_swap_to_mesh_two;
extern SDL_Keycode key_swap_to_mesh_third;
extern SDL_Keycode key_show_all_meshes;
extern SDL_Keycode key_rotate_mesh;
#endif
