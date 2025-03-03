#include "input.h"
#include "key_mappings.h"
#include "camera.h"
#include "mesh.h"
#include "display.h"
#include <stdbool.h>

///////////////////////////////////////////////////////////////////////////////
// Global Variables for Input and Mouse Handling
///////////////////////////////////////////////////////////////////////////////
static bool key_states[SDL_NUM_SCANCODES] = { false };
static bool key_locked[SDL_NUM_SCANCODES] = { false };
static bool mouse_captured = false;
static bool is_middle_clicking = false;
static int last_x = 0, last_y = 0;
static float mouse_sensitivity = 0.01f;
bool rotate_meshes = false;
bool play_animation = false;
bool show_all_meshes = false;
bool show_fps = false;  

///////////////////////////////////////////////////////////////////////////////
// Initialize Input System
///////////////////////////////////////////////////////////////////////////////
void init_input(void) {
    memset(key_states, 0, sizeof(key_states));
    memset(key_locked, 0, sizeof(key_locked));

    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_ShowCursor(SDL_DISABLE);
    mouse_captured = false;

    SDL_GetMouseState(&last_x, &last_y);
}


///////////////////////////////////////////////////////////////////////////////
// Event-Driven Key Handling
///////////////////////////////////////////////////////////////////////////////
void handle_key_event(SDL_Keycode key, bool pressed) {
    SDL_Scancode scancode = SDL_GetScancodeFromKey(key);
    key_states[scancode] = pressed;
}

///////////////////////////////////////////////////////////////////////////////
// Event-Driven Mouse Handling
///////////////////////////////////////////////////////////////////////////////
void handle_mouse_event(SDL_Event event, float delta_time) {
    switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
        Uint8 button = event.button.button;

        if (button == SDL_BUTTON_MIDDLE) {
            is_middle_clicking = true;
            break;
        }

        if (!show_all_meshes) {
            swap_mesh_index((button == SDL_BUTTON_LEFT) ? 1 : (button == SDL_BUTTON_RIGHT) ? -1 : 0);
        }
        else {
            cycle_camera_position((button == SDL_BUTTON_LEFT) ? -1 : (button == SDL_BUTTON_RIGHT) ? 1 : 0);
        }
        break;
    }


    case SDL_MOUSEBUTTONUP: {
        Uint8 button = event.button.button;
        if (button == SDL_BUTTON_MIDDLE) {
            is_middle_clicking = false;
        }
        break;
    }

    case SDL_MOUSEWHEEL: {
        if (event.wheel.y > 0) {
            move_camera_forward(CAMERA_DEFAULT_SPEED * delta_time);
        }
        else if (event.wheel.y < 0) {
            move_camera_backward(CAMERA_DEFAULT_SPEED * delta_time);
        }
        break;
    }
    }
}


///////////////////////////////////////////////////////////////////////////////
// Check if a Specific Key is Pressed (Stored in Bitset)
///////////////////////////////////////////////////////////////////////////////
#define IS_KEY_PRESSED(key) (key_states[SDL_GetScancodeFromKey(key)])
#define IS_KEY_RELEASED(key) (!key_states[SDL_GetScancodeFromKey(key)] && key_locked[SDL_GetScancodeFromKey(key)])

///////////////////////////////////////////////////////////////////////////////
// Toggle Mouse Capture (Enable/Disable Relative Mouse Mode)
///////////////////////////////////////////////////////////////////////////////
void toggle_mouse_capture(void) {
    if (mouse_captured) {
        SDL_SetRelativeMouseMode(SDL_FALSE);
        SDL_ShowCursor(SDL_ENABLE);
    }
    else {
        SDL_SetRelativeMouseMode(SDL_TRUE);
    }
    mouse_captured = !mouse_captured;
}

///////////////////////////////////////////////////////////////////////////////
// Toggle FPS Display
///////////////////////////////////////////////////////////////////////////////
void toggle_fps_display(void) {
    show_fps = !show_fps;
}

///////////////////////////////////////////////////////////////////////////////
// Handle Mouse Input for Camera Rotation
///////////////////////////////////////////////////////////////////////////////
void handle_mouse_input(float delta_time) {

    int mouse_x, mouse_y;
    SDL_GetRelativeMouseState(&mouse_x, &mouse_y);

    if (is_middle_clicking) {
        move_camera_right(mouse_x * delta_time * CAMERA_DEFAULT_SPEED * 0.1f);
        move_camera_up   (mouse_y * delta_time * CAMERA_DEFAULT_SPEED * 0.1f);
    }
    else {
        if (!mouse_captured) return;
        if (mouse_x != 0) rotate_camera_yaw(mouse_x * mouse_sensitivity);
        if (mouse_y != 0) rotate_camera_pitch(mouse_y * mouse_sensitivity);
    }
}


///////////////////////////////////////////////////////////////////////////////
// Handle Keyboard Input for Camera, Rendering, and Mesh Swapping
///////////////////////////////////////////////////////////////////////////////
void handle_keyboard_input(float delta_time) {
    SDL_ShowCursor(SDL_DISABLE);

    // Camera Speed Controls
    if (IS_KEY_PRESSED(key_move_camera_accelerate)) update_camera_speed(CAMERA_SPEED_BOOST);
    else if (IS_KEY_PRESSED(key_move_camera_deccelerate)) update_camera_speed(CAMERA_SPEED_SLOW);
    else update_camera_speed(CAMERA_DEFAULT_SPEED);

    // Rendering Controls
    struct { SDL_Keycode key; void (*action)(int); int param; } render_actions[] = {
        { key_render_wire_vertex,        set_render_method, RENDER_WIRE_VERTEX },
        { key_render_wire,               set_render_method, RENDER_WIRE },
        { key_render_fill_triangle,      set_render_method, RENDER_FILL_TRIANGLE },
        { key_render_fill_triangle_wire, set_render_method, RENDER_FILL_TRIANGLE_WIRE },
        { key_render_textured,           set_render_method, RENDER_TEXTURED },
        { key_render_textured_wire,      set_render_method, RENDER_TEXTURED_WIRE }
    };

    // Iterate through actions
    for (size_t i = 0; i < sizeof(render_actions) / sizeof(render_actions[0]); i++) {
        if (IS_KEY_PRESSED(render_actions[i].key)) {
            render_actions[i].action(render_actions[i].param);
        }
    }


    // Culling Controls
    struct { SDL_Keycode key; void (*action)(int); int param;} cull_actions[] = {
        { key_cull_backface,            set_cull_method, CULL_BACKFACE },
        { key_cull_none,                set_cull_method, CULL_NONE }
    };

    for (size_t i = 0; i < sizeof(cull_actions) / sizeof(cull_actions[0]); i++) {
        if (IS_KEY_PRESSED(cull_actions[i].key)) cull_actions[i].action(cull_actions[i].param);
    }

    // Camera Rotation
    struct { SDL_Keycode key; void (*action)(float); float amount; } camera_rotations[] = {
        { key_rotate_camera_pitch_up,   rotate_camera_pitch, +3.0f },
        { key_rotate_camera_pitch_down, rotate_camera_pitch, -3.0f },
        { key_rotate_camera_yaw_left,   rotate_camera_yaw, +3.0f },
        { key_rotate_camera_yaw_right,  rotate_camera_yaw, -3.0f }
    };

    for (size_t i = 0; i < sizeof(camera_rotations) / sizeof(camera_rotations[0]); i++) {
        if (IS_KEY_PRESSED(camera_rotations[i].key)) camera_rotations[i].action(camera_rotations[i].amount * delta_time);
    }

    // Camera Movement
    struct { SDL_Keycode key; void (*action)(float); } camera_movements[] = {
        { key_move_camera_forward,      move_camera_forward },
        { key_move_camera_backward,     move_camera_backward },
        { key_move_camera_left,         move_camera_left },
        { key_move_camera_right,        move_camera_right },
        { key_move_camera_up,           move_camera_up },
        { key_move_camera_down,         move_camera_down }
    };

    for (size_t i = 0; i < sizeof(camera_movements) / sizeof(camera_movements[0]); i++) {
        if (IS_KEY_PRESSED(camera_movements[i].key)) camera_movements[i].action(delta_time);
    }

    // Toggle Mouse Capture
    if (IS_KEY_PRESSED(key_toggle_mouse_capture) && !key_locked[SDL_GetScancodeFromKey(key_toggle_mouse_capture)]) {
        toggle_mouse_capture();
        key_locked[SDL_GetScancodeFromKey(key_toggle_mouse_capture)] = true;
    }

    if (IS_KEY_RELEASED(key_toggle_mouse_capture)) {
        key_locked[SDL_GetScancodeFromKey(key_toggle_mouse_capture)] = false;
    }

    // Swap Mesh (One-shot trigger)
    struct { SDL_Keycode key; void (*action)(int); int mesh_id; } swap_mesh_actions[] = {
        { key_swap_mesh,                swap_mesh, -1 },
        { key_swap_to_mesh_one,         swap_to_mesh, 1 },
        { key_swap_to_mesh_two,         swap_to_mesh, 2 },
        { key_swap_to_mesh_third,       swap_to_mesh, 3 }
    };

    for (size_t i = 0; i < sizeof(swap_mesh_actions) / sizeof(swap_mesh_actions[0]); i++) {
        SDL_Scancode scancode = SDL_GetScancodeFromKey(swap_mesh_actions[i].key);
        if (IS_KEY_PRESSED(swap_mesh_actions[i].key) && !key_locked[scancode]) {
            show_all_meshes = false;
            if (swap_mesh_actions[i].mesh_id == -1) {
                swap_mesh();
                reset_camera_view(CAMERA_ZOOM_POSITION, CAMERA_DEFAULT_DIRECTION);
            }
            else {
                swap_to_mesh(swap_mesh_actions[i].mesh_id);
                reset_camera_view(CAMERA_ZOOM_POSITION, CAMERA_DEFAULT_DIRECTION);
                reset_mesh_position(swap_mesh_actions[i].mesh_id);
            }
            key_locked[scancode] = true;
        }
        if (IS_KEY_RELEASED(swap_mesh_actions[i].key)) {
            key_locked[scancode] = false;
        }
    }

    // Show all meshes
    if (IS_KEY_PRESSED(key_show_all_meshes))
    {
        show_all_meshes = true;
        update_camera_position(CAMERA_DEFAULT_POSITION);
        reset_all_meshes();
    }

	// Rotate Meshes
    if (IS_KEY_PRESSED(key_rotate_mesh) && !key_locked[SDL_GetScancodeFromKey(key_rotate_mesh)]) {
        rotate_meshes = !rotate_meshes;
        key_locked[SDL_GetScancodeFromKey(key_rotate_mesh)] = true;
    }
    if (IS_KEY_RELEASED(key_rotate_mesh)) {
        key_locked[SDL_GetScancodeFromKey(key_rotate_mesh)] = false;
    }

	// Toggle FPS Display
    if (IS_KEY_PRESSED(key_toggle_fps) && !key_locked[SDL_GetScancodeFromKey(key_toggle_fps)]) {
        toggle_fps_display(); // Toggle FPS display
        key_locked[SDL_GetScancodeFromKey(key_toggle_fps)] = true;
    }
    if (IS_KEY_RELEASED(key_toggle_fps)) {
        key_locked[SDL_GetScancodeFromKey(key_toggle_fps)] = false;
    }

    // Play Animation
    if (IS_KEY_PRESSED(SDLK_SPACE) && !key_locked[SDL_SCANCODE_SPACE]) {
        play_animation = !play_animation;  // Active/Désactive l'animation
        key_locked[SDL_SCANCODE_SPACE] = true;
        printf("PlayAnim: %s\n", play_animation ? "ON" : "OFF");
    }

    if (IS_KEY_RELEASED(SDLK_SPACE)) {
        key_locked[SDL_SCANCODE_SPACE] = false;
    }

}

///////////////////////////////////////////////////////////////////////////////
// Process Input Events (Now Event-Driven)
///////////////////////////////////////////////////////////////////////////////
void process_input(float delta_time, bool* is_running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            *is_running = false;
            break;

        case SDL_KEYDOWN:
            handle_key_event(event.key.keysym.sym, true);
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                *is_running = false;
            }
            break;

        case SDL_KEYUP:
            handle_key_event(event.key.keysym.sym, false);
            break;

        default:
            handle_mouse_event(event, delta_time); 
            break;
		}
	}

    handle_keyboard_input(delta_time);
    handle_mouse_input(delta_time);
}
