#ifndef INPUT_H
#define INPUT_H

#include <SDL.h>
#include <stdbool.h>

extern bool rotate_meshes;
extern bool play_animation;
extern bool show_all_meshes;
extern bool show_fps;

///////////////////////////////////////////////////////////////////////////////
// Input Handling Functions
///////////////////////////////////////////////////////////////////////////////
/**
 * @brief Initialize input-related settings if needed.
 */
void init_input(void);

/**
 * @brief Poll and process input events such as keyboard and mouse.
 * @param delta_time Time since the last frame update in seconds.
 * @param is_running Pointer to the running state of the application.
 */
void process_input(float delta_time, bool* is_running);

/**
 * @brief Handle specific keyboard input for movement or camera rotation.
 * @param delta_time Time since the last frame update in seconds.
 */
void handle_keyboard_input(float delta_time);

/**
 * @brief Handle mouse input for camera rotation (yaw and pitch).
 * @param delta_time Time since the last frame update in seconds.
 */
void handle_mouse_input(float delta_time);

/**
 * @brief Toggle mouse capture mode.
 */
void toggle_mouse_capture(void);

/**
 * @brief Handle key press event.
 * @param key The key that was pressed.
 * @param pressed True if the key was pressed, false if released.
 */
extern bool show_all_meshes;

/**
 * @brief Handle key press event.
 * @param key The key that was pressed.
 * @param pressed True if the key was pressed, false if released.
 */
void toggle_fps_display(void);


#endif // INPUT_H
