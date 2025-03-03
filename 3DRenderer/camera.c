#include "camera.h"

static camera_t camera;
static int current_camera_index = 0;

void init_camera(vec3_t position, vec3_t direction, float speed) {
    camera.position = position;
    camera.direction = direction;
    camera.forward_velocity = vec3_new(0, 0, 0);
    camera.yaw = 0.0;
    camera.pitch = 0.0;
	camera.speed = speed;
};

// Getters for camera properties
vec3_t get_camera_position(void) {
    return camera.position;
}

vec3_t get_camera_direction(void) {
    return camera.direction;
}

vec3_t get_camera_forward_velocity(void) {
    return camera.forward_velocity;
}

float get_camera_yaw(void) {
    return camera.yaw;
}

float get_camera_pitch(void) {
    return camera.pitch;
}

float get_camera_speed(void)
{
    return camera.speed;
}

void update_camera_speed(float speed)
{
	camera.speed = speed;
}

// Setters for camera properties
void update_camera_position(vec3_t position) {
    camera.position = position;
}

void update_camera_direction(vec3_t direction) {
    camera.direction = direction;
}

void update_camera_forward_velocity(vec3_t forward_velocity) {
    camera.forward_velocity = forward_velocity;
}

void rotate_camera(vec3_t rotation)
{
	camera.yaw += rotation.x;
	camera.pitch += rotation.y;
}

void rotate_camera_yaw(float angle) {
    camera.yaw += angle;
}

void rotate_camera_pitch(float angle) {
    camera.pitch += angle;
}

vec3_t get_camera_lookat_target(void) {
    // Initialize the target looking at the positive z-axis
    vec3_t target = { 0, 0, 1 };
    
    mat4_t camera_yaw_rotation = mat4_make_rotation_y(camera.yaw);
    mat4_t camera_pitch_rotation = mat4_make_rotation_x(camera.pitch);

    // Create camera rotation matrix based on yaw and pitch
    mat4_t camera_rotation = mat4_identity();
    camera_rotation = mat4_mul_mat4(camera_pitch_rotation, camera_rotation);
    camera_rotation = mat4_mul_mat4(camera_yaw_rotation, camera_rotation);

    // Update camera direction based on the rotation
    vec4_t camera_direction = mat4_mul_vec4(camera_rotation, vec4_from_vec3(target));
    camera.direction = vec3_from_vec4(camera_direction);

    // Offset the camera position in the direction where the camera is pointing at
    target = vec3_add(camera.position, camera.direction);

    return target;
}

vec3_t get_camera_right(void) {
    vec3_t up = { 0.0f, 1.0f, 0.0f };
    return vec3_cross(up, camera.direction);  // Right = Up x Direction
}

vec3_t get_camera_up(void) {
    vec3_t right = get_camera_right();
    return vec3_cross(camera.direction, right);  // Up = Direction x Right
}

void reset_camera_view(void) {
    update_camera_position(CAMERA_DEFAULT_POSITION);  
    update_camera_direction(CAMERA_DEFAULT_DIRECTION); 
    rotate_camera_yaw(-get_camera_yaw());   // Reset yaw to 0
    rotate_camera_pitch(-get_camera_pitch());  // Reset pitch to 0
    current_camera_index = 0; 

    printf("Camera reset to default position and direction.\n");
}


void cycle_camera_position(int direction) {
    vec3_t camera_positions[NUM_CAMERA_POSITIONS] = CAMERA_POSITIONS;
    vec3_t target = vec3_new(0, 0, 25);

    // Cycle to the next camera position
    current_camera_index = (current_camera_index + direction + NUM_CAMERA_POSITIONS) % NUM_CAMERA_POSITIONS;

    // Update the camera position
    update_camera_position(camera_positions[current_camera_index]);

    // Compute direction to target
    vec3_t current_position = get_camera_position();
    vec3_t direction_to_target = vec3_normalize_t(vec3_sub(target, current_position));

    // Convert direction to yaw and pitch angles
    float yaw = atan2(direction_to_target.x, direction_to_target.z);  // Yaw (rotation around Y-axis)
    float pitch = asin(direction_to_target.y);                        // Pitch (rotation up/down)

    // Update camera rotation
    rotate_camera_yaw(yaw - get_camera_yaw());   // Adjust yaw
    rotate_camera_pitch(pitch - get_camera_pitch());  // Adjust pitch

    // Update the camera direction
    update_camera_direction(direction_to_target);

    printf("Camera position changed to %d\n", current_camera_index);
    printf("Camera position: (%.2f, %.2f, %.2f)\n", camera.position.x, camera.position.y, camera.position.z);
    printf("Camera direction: (%.2f, %.2f, %.2f)\n", camera.direction.x, camera.direction.y, camera.direction.z);
    printf("Yaw: %.2f, Pitch: %.2f\n", get_camera_yaw(), get_camera_pitch());
}




void move_camera_forward(float delta_time) {
    vec3_t velocity = vec3_mul(get_camera_direction(), camera.speed * delta_time);
    update_camera_forward_velocity(velocity);
    update_camera_position(vec3_add(get_camera_position(), velocity));
}

void move_camera_backward(float delta_time) {
    vec3_t velocity = vec3_mul(get_camera_direction(), camera.speed * delta_time);
    update_camera_forward_velocity(velocity);
    update_camera_position(vec3_sub(get_camera_position(), velocity));
}

void move_camera_left(float delta_time) {
    vec3_t velocity = vec3_mul(get_camera_right(), camera.speed * delta_time);
    update_camera_forward_velocity(velocity);
    update_camera_position(vec3_sub(get_camera_position(), velocity));
}

void move_camera_right(float delta_time) {
    vec3_t velocity = vec3_mul(get_camera_right(), camera.speed * delta_time);
    update_camera_forward_velocity(velocity);
    update_camera_position(vec3_add(get_camera_position(), velocity));
}

void move_camera_up(float delta_time) {  
    vec3_t velocity = vec3_mul(get_camera_up(), camera.speed * delta_time);
    update_camera_forward_velocity(velocity); 
    update_camera_position(vec3_add(get_camera_position(), velocity));  
}

void move_camera_down(float delta_time) {
    vec3_t velocity = vec3_mul(get_camera_up(), camera.speed * delta_time);
    update_camera_forward_velocity(velocity);  
    update_camera_position(vec3_sub(get_camera_position(), velocity));  
}
