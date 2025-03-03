#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"
#include "matrix.h"
#include "math.h"

#define CAMERA_SPEED_BOOST       10.0f   // Boost speed
#define CAMERA_SPEED_SLOW        2.5f    // Slow down factor speed
#define CAMERA_DEFAULT_SPEED     5.0f    // Default camera speed
#define CAMERA_DEFAULT_POSITION  vec3_new(0, 0, 0)
#define CAMERA_ZOOM_POSITION     vec3_new(0, 0, 5)
#define CAMERA_DEFAULT_DIRECTION vec3_new(0, 0, 1)

#define NUM_CAMERA_POSITIONS 4

#define CAMERA_POSITION_1 vec3_new(-25, 0, 25)    // Left side
#define CAMERA_POSITION_2 vec3_new(0, 0, 50)   // Top side
#define CAMERA_POSITION_3 vec3_new(25, 0, 25)   // Right side
#define CAMERA_POSITION_4 vec3_new(0, 0, 0)  // Bottom side

#define CAMERA_POSITIONS { CAMERA_POSITION_1, CAMERA_POSITION_2, CAMERA_POSITION_3, CAMERA_POSITION_4 }


typedef struct {
    vec3_t position;
    vec3_t direction;
    vec3_t forward_velocity;
    float yaw;
    float pitch;
    float speed;
} camera_t;

void init_camera(vec3_t position, vec3_t direction, float speed);

vec3_t get_camera_position(void);
vec3_t get_camera_direction(void);
vec3_t get_camera_forward_velocity(void);
float get_camera_yaw(void);
float get_camera_pitch(void);
float get_camera_speed(void);

void update_camera_speed(float speed);
void update_camera_position(vec3_t position);
void update_camera_direction(vec3_t direction);
void update_camera_forward_velocity(vec3_t forward_velocity);

void rotate_camera(vec3_t rotation);
void rotate_camera_yaw(float angle);
void rotate_camera_pitch(float angle);

void move_camera_forward(float delta_time);
void move_camera_backward(float delta_time);
void move_camera_left(float delta_time);
void move_camera_right(float delta_time);
void move_camera_up(float delta_time);
void move_camera_down(float delta_time);

vec3_t get_camera_lookat_target(void);
vec3_t get_camera_right(void);
vec3_t get_camera_up(void);

void reset_camera_view(void);
void cycle_camera_position(int direction);


#endif
