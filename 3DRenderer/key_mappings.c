#include "key_mappings.h"

///////////////////////////////////////////////////////////////////////////////
// Default Key Mappings
///////////////////////////////////////////////////////////////////////////////
// Rendering Controls
SDL_Keycode key_render_wire_vertex			= SDLK_1;
SDL_Keycode key_render_wire					= SDLK_2;
SDL_Keycode key_render_fill_triangle		= SDLK_3;
SDL_Keycode key_render_fill_triangle_wire	= SDLK_4;
SDL_Keycode key_render_textured				= SDLK_5;
SDL_Keycode key_render_textured_wire		= SDLK_6;

// Culling Controls
SDL_Keycode key_cull_backface				= SDLK_c;
SDL_Keycode key_cull_none					= SDLK_x;

// Camera Rotation
SDL_Keycode key_rotate_camera_pitch_up		= SDLK_z;
SDL_Keycode key_rotate_camera_pitch_down	= SDLK_s;
SDL_Keycode key_rotate_camera_yaw_left		= SDLK_d;
SDL_Keycode key_rotate_camera_yaw_right		= SDLK_q;

// Camera Movement
SDL_Keycode key_move_camera_forward			= SDLK_UP;
SDL_Keycode key_move_camera_backward		= SDLK_DOWN;
SDL_Keycode key_move_camera_left			= SDLK_LEFT;
SDL_Keycode key_move_camera_right			= SDLK_RIGHT;
SDL_Keycode key_move_camera_up				= SDLK_e;
SDL_Keycode key_move_camera_down			= SDLK_a;
SDL_Keycode key_move_camera_accelerate		= SDLK_LSHIFT;
SDL_Keycode key_move_camera_deccelerate		= SDLK_LCTRL;

// Other
SDL_Keycode key_toggle_fullscreen			= SDLK_F11;
SDL_Keycode key_toggle_mouse_capture		= SDLK_m;
SDL_Keycode key_toggle_fps					= SDLK_f;

// Mesh Controls
SDL_Keycode key_swap_mesh					= SDLK_TAB;
SDL_Keycode key_swap_to_mesh_one			= SDLK_7;
SDL_Keycode key_swap_to_mesh_two			= SDLK_8;
SDL_Keycode key_swap_to_mesh_third			= SDLK_9;
SDL_Keycode key_show_all_meshes				= SDLK_0;
SDL_Keycode key_rotate_mesh					= SDLK_r;
