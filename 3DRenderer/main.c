#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "upng.h"
#include "array.h"
#include "display.h"
#include "triangle.h"
#include "clipping.h"
#include "vector.h"
#include "matrix.h"
#include "light.h"
#include "camera.h"
#include "texture.h"
#include "mesh.h"
#include "input.h"
#include "resources.h"


///////////////////////////////////////////////////////////////////////////////
// Utilities properties
///////////////////////////////////////////////////////////////////////////////
#define ROTATION_SPEED 2.0f

///////////////////////////////////////////////////////////////////////////////
// Global variables for execution status and game loop
///////////////////////////////////////////////////////////////////////////////
bool is_running = false;
int previous_frame_time = 0;
float delta_time = 0; 
static int target_fps = 60;

///////////////////////////////////////////////////////////////////////////////
// Array to store triangles that should be rendered each frame
///////////////////////////////////////////////////////////////////////////////
#define MAX_TRIANGLES 100000
triangle_t triangles_to_render[MAX_TRIANGLES];
int triangles_to_render_count = 0;

///////////////////////////////////////////////////////////////////////////////
// Global variables to control the rendering of the scene
///////////////////////////////////////////////////////////////////////////////
#define MAX_MESHES 10  // Define a reasonable limit for meshes
static vec3_t original_positions[MAX_MESHES];
static vec3_t original_rotations[MAX_MESHES];


///////////////////////////////////////////////////////////////////////////////
// Declaration of our global transformation matrices
///////////////////////////////////////////////////////////////////////////////
mat4_t world_matrix;
mat4_t proj_matrix;
mat4_t view_matrix;

///////////////////////////////////////////////////////////////////////////////
// Utilities
///////////////////////////////////////////////////////////////////////////////
void cap_fps(void) {
    static Uint32 last_frame_time = 0;
    Uint32 current_time = SDL_GetTicks();
    Uint32 frame_time = current_time - last_frame_time;

    // Durée idéale d'une frame (en millisecondes)
    Uint32 target_frame_time = 1000 / target_fps;

    if (frame_time < target_frame_time) {
        SDL_Delay(target_frame_time - frame_time); // Attendre le temps nécessaire
    }

    last_frame_time = SDL_GetTicks(); // Mise à jour du temps de frame
}


void show_fps_console()
{
    static int frame_count = 0;
    static float fps_timer = 0;
    frame_count++;
    fps_timer += delta_time;

    if (fps_timer >= 1.0f) {
        int fps = frame_count;
        frame_count = 0;
        fps_timer = 0;

        printf("FPS: %d\n", fps);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Debug Animation
///////////////////////////////////////////////////////////////////////////////
void animation_airport(float speed) {
    if (!show_all_meshes || !play_animation) return;  // Vérifie que l'animation est activée

    static float elapsed_time = 0;  
    float wait_time = 2.0f;  

    int takeoff_meshes[] = { 1, 3 };                         // Meshes to take off
    float takeoff_speeds[] = { speed * 0.5f, speed * 0.3f }; // Planes Speed in Z
    float ascent_speeds[] = { speed * 0.2f, speed * 0.15f }; // Planes Speeds in Y
    float max_altitudes[] = { 5.0f, 7.0f };                  //Max Altitude for each plane
    float start_tilt_threshold = -1.29f;                     // Height at which they begin to tilt
    float flatten_threshold = 4.0f;                          // Height at which they become horizontal again
    float max_tilt = -M_PI / 6;                              // Maximum tilt(-30°)

    elapsed_time += delta_time; 

    for (int i = 0; i < 2; i++) {
        mesh_t* mesh = get_mesh(takeoff_meshes[i]);
        if (mesh) {
            mesh->translation.z += takeoff_speeds[i] * delta_time; 

            // Wait for `wait_time` before starting to climb in Y
            if (elapsed_time >= wait_time && mesh->translation.y < max_altitudes[i]) {
                mesh->translation.y += ascent_speeds[i] * delta_time;   
            }

            // Progressive tilt phase
            if (mesh->translation.y >= start_tilt_threshold && mesh->translation.y < flatten_threshold) {
                float progression = (mesh->translation.y - start_tilt_threshold) / (flatten_threshold - start_tilt_threshold);
                float tilt_value = max_tilt * progression;
                mesh->rotation.x = tilt_value;
            }

			// Progressive Horizontal return phase
            else if (mesh->translation.y >= flatten_threshold && mesh->translation.y < max_altitudes[i]) {
                float remaining_progress = (max_altitudes[i] - mesh->translation.y) / (max_altitudes[i] - flatten_threshold);
                mesh->rotation.x = max_tilt * remaining_progress; 
            }

            // Stop Climb when max altitude reached 
            if (mesh->translation.y >= max_altitudes[i]) {
                mesh->translation.y = max_altitudes[i]; 
                mesh->rotation.x = 0; 
            }
        }
    }

    // Move Car
    mesh_t* mesh2 = get_mesh(2);
    if (mesh2) {
        if (mesh2->translation.z < 40.0f)
        {
            mesh2->translation.z += speed * delta_time * 0.25f;
        }
    }
}









///////////////////////////////////////////////////////////////////////////////
// Function to load all the meshes and textures
///////////////////////////////////////////////////////////////////////////////
void initialize_meshes(void) {
    load_mesh(RUNWAY_MESH_PATH, RUNWAY_TEXTURE_PATH, vec3_new(1, 1, 1), vec3_new(0, -1.5, +23), vec3_new(0, 0, 0));
    load_mesh(EFA_MESH_PATH, EFA_TEXTURE_PATH, vec3_new(1, 1, 1), vec3_new(-2, -1.3, +9), vec3_new(0, -M_PI / 2, 0));
    load_mesh(GT_40_MESH_PATH, GT_40_TEXTURE_PATH, vec3_new(0.5, 0.5, 0.5), vec3_new(0, -1.3, +12), vec3_new(0, -M_PI, 0));
    load_mesh(F117_MESH_PATH, F117_TEXTURE_PATH, vec3_new(1, 1, 1), vec3_new(+2, -1.3, +9), vec3_new(0, -M_PI / 2, 0));
    load_mesh(DOOM_MESH_PATH, DOOM_TEXTURE_PATH, vec3_new(2, 2, 2), vec3_new(0, -1, 4), vec3_new(0, M_PI / 3, 0));
	load_mesh(CRASH_BANDICOOT_MESH_PATH, CRASH_BANDICOOT_TEXTURE_PATH, vec3_new(1, 1, 1), vec3_new(-2, -1.6, 4), vec3_new(0,  M_PI / 3, 0));
    load_mesh(SPYRO_MESH_PATH, SPYRO_TEXTURE_PATH, vec3_new(1, 1, 1), vec3_new(2, -1.4 , 4), vec3_new(0, 2 * M_PI / 3, 0));
}



///////////////////////////////////////////////////////////////////////////////
// Setup function to initialize variables and game objects
///////////////////////////////////////////////////////////////////////////////
void setup(void) {
    // Initialize Camera
	init_camera(CAMERA_DEFAULT_POSITION,CAMERA_DEFAULT_DIRECTION, CAMERA_DEFAULT_SPEED);

    // Initialize input system
    init_input();

    // Initialize render mode and triangle culling method
    set_render_method(RENDER_TEXTURED);
    set_cull_method(CULL_BACKFACE);

    // Initialize the scene light direction
    init_light(vec3_new(0, 0, 1));

    // Initialize the perspective projection matrix
    float aspect_y = (float)get_window_height() / (float)get_window_width();
    float aspect_x = (float)get_window_width() / (float)get_window_height();
    float fov_y = 3.141592 / 3.0; // the same as 180/3, or 60deg
    float fov_x = atan(tan(fov_y / 2) * aspect_x) * 2;
    float znear = 1.0;
    float zfar = 50.0;
    proj_matrix = mat4_make_perspective(fov_y, aspect_y, znear, zfar);

    // Initialize frustum planes with a point and a normal
    init_frustum_planes(fov_x, fov_y, znear, zfar);

    // Loads mesh entities
	initialize_meshes();

    // Initialize Mesh to Render
    show_all_meshes = false;
    get_mesh(1)->translation = MESH_DEFAULT_POS;
	get_mesh(1)->rotation = vec3_new(0, 0, 0);

	rotate_meshes = true;
}

///////////////////////////////////////////////////////////////////////////////
// Poll system events and handle keyboard input
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Process the graphics pipeline stages for all the mesh triangles
///////////////////////////////////////////////////////////////////////////////
// +-------------+
// | Model space |  <-- original mesh vertices
// +-------------+
// |   +-------------+
// `-> | World space |  <-- multiply by world matrix
//     +-------------+
//     |   +--------------+
//     `-> | Camera space |  <-- multiply by view matrix
//         +--------------+
//         |    +------------+
//         `--> |  Clipping  |  <-- clip against the six frustum planes
//              +------------+
//              |    +------------+
//              `--> | Projection |  <-- multiply by projection matrix
//                   +------------+
//                   |    +-------------+
//                   `--> | Image space |  <-- apply perspective divide
//                        +-------------+
//                        |    +--------------+
//                        `--> | Screen space |  <-- ready to render
//                             +--------------+
///////////////////////////////////////////////////////////////////////////////
void process_graphics_pipeline_stages(mesh_t* mesh) {
    // Create scale, rotation, and translation matrices that will be used to multiply the mesh vertices
    mat4_t scale_matrix = mat4_make_scale(mesh->scale.x, mesh->scale.y, mesh->scale.z);
    mat4_t translation_matrix = mat4_make_translation(mesh->translation.x, mesh->translation.y, mesh->translation.z);
    mat4_t rotation_matrix_x = mat4_make_rotation_x(mesh->rotation.x);
    mat4_t rotation_matrix_y = mat4_make_rotation_y(mesh->rotation.y);
    mat4_t rotation_matrix_z = mat4_make_rotation_z(mesh->rotation.z);

    // Update camera look at target to create view matrix
    vec3_t target = get_camera_lookat_target();
    vec3_t up_direction = vec3_new(0, 1, 0);
    view_matrix = mat4_look_at(get_camera_position(), target, up_direction);

    // Loop all triangle faces of our mesh
    int num_faces = array_length(mesh->faces);
    for (int face_index = 0; face_index < num_faces; face_index++) {
        face_t mesh_face = mesh->faces[face_index];

        vec3_t face_vertices[3];
        face_vertices[0] = mesh->vertices[mesh_face.a - 1];
        face_vertices[1] = mesh->vertices[mesh_face.b - 1];
        face_vertices[2] = mesh->vertices[mesh_face.c - 1];

        vec4_t transformed_vertices[3];

        // Loop all three vertices of this current face and apply transformations
        for (int v = 0; v < 3; v++) {
            vec4_t transformed_vertex = vec4_from_vec3(face_vertices[v]);

            // Create a World Matrix combining scale, rotation, and translation matrices
            world_matrix = mat4_identity();

            // Order matters: First scale, then rotate, then translate. [T]*[R]*[S]*v
            world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);
            world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
            world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);
            world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);
            world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);

            // Multiply the world matrix by the original vector
            transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);

            // Multiply the view matrix by the vector to transform the scene to camera space
            transformed_vertex = mat4_mul_vec4(view_matrix, transformed_vertex);

            // Save transformed vertex in the array of transformed vertices
            transformed_vertices[v] = transformed_vertex;
        }

        // Calculate the triangle face normal
        vec3_t face_normal = get_triangle_normal(transformed_vertices);

        // Backface culling test to see if the current face should be projected
        if (should_cull_backface()) {
            // Find the vector between vertex A in the triangle and the camera origin
            vec3_t camera_ray = vec3_sub(vec3_new(0, 0, 0), vec3_from_vec4(transformed_vertices[0]));

            // Calculate how aligned the camera ray is with the face normal (using dot product)
            float dot_normal_camera = vec3_dot(face_normal, camera_ray);

            // Backface culling, bypassing triangles that are looking away from the camera
            if (dot_normal_camera < 0) {
                continue;
            }
        }
        
        // Create a polygon from the original transformed triangle to be clipped
        polygon_t polygon = polygon_from_triangle(
            vec3_from_vec4(transformed_vertices[0]),
            vec3_from_vec4(transformed_vertices[1]),
            vec3_from_vec4(transformed_vertices[2]),
            mesh_face.a_uv,
            mesh_face.b_uv,
            mesh_face.c_uv
        );
        
        // Clip the polygon and returns a new polygon with potential new vertices
        clip_polygon(&polygon);

        // Break the clipped polygon apart back into a list of triangles
        triangle_t triangles_after_clipping[MAX_NUM_POLY_TRIANGLES];
        int num_triangles_after_clipping = 0;

        triangles_from_polygon(&polygon, triangles_after_clipping, &num_triangles_after_clipping);

        // Loops all the assembled triangles after clipping
        for (int triangle_index = 0; triangle_index < num_triangles_after_clipping; triangle_index++) {
            triangle_t triangle_after_clipping = triangles_after_clipping[triangle_index];

            vec4_t projected_points[3];

            // Loop all three vertices to perform projection and conversion to screen space
            for (int v = 0; v < 3; v++) {
                // Project the current vertex using a perspective projection matrix
                projected_points[v] = mat4_mul_vec4(proj_matrix, triangle_after_clipping.points[v]);

                // Perform perspective divide
                if (projected_points[v].w != 0) {
                    projected_points[v].x /= projected_points[v].w;
                    projected_points[v].y /= projected_points[v].w;
                    projected_points[v].z /= projected_points[v].w;
                }

                // Flip vertically since the y values of the 3D mesh grow bottom->up and in screen space y values grow top->down
                projected_points[v].y *= -1;

                // Scale into the view
                projected_points[v].x *= (get_window_width() / 2.0);
                projected_points[v].y *= (get_window_height() / 2.0);

                // Translate the projected points to the middle of the screen
                projected_points[v].x += (get_window_width() / 2.0);
                projected_points[v].y += (get_window_height() / 2.0);
            }

            // Calculate the shade intensity based on how aliged is the normal with the flipped light direction ray
            float light_intensity_factor = -vec3_dot(face_normal, get_light_direction());

            // Calculate the triangle color based on the light angle
            uint32_t triangle_color = apply_light_intensity(mesh_face.color, light_intensity_factor);

            // Create the final projected triangle that will be rendered in screen space
            triangle_t triangle_to_render = {
                .points = {
                    { projected_points[0].x, projected_points[0].y, projected_points[0].z, projected_points[0].w },
                    { projected_points[1].x, projected_points[1].y, projected_points[1].z, projected_points[1].w },
                    { projected_points[2].x, projected_points[2].y, projected_points[2].z, projected_points[2].w }
                },
                .texcoords = {
                    { triangle_after_clipping.texcoords[0].u, triangle_after_clipping.texcoords[0].v },
                    { triangle_after_clipping.texcoords[1].u, triangle_after_clipping.texcoords[1].v },
                    { triangle_after_clipping.texcoords[2].u, triangle_after_clipping.texcoords[2].v }
                },
                .color = triangle_color,
                .texture = mesh->texture
            };

            // Save the projected triangle in the array of triangles to render
            if (triangles_to_render_count < MAX_TRIANGLES) {
                triangles_to_render[triangles_to_render_count++] = triangle_to_render;
            }
        }
    }
}

void render_meshes(void)
{
    // If show_all_meshes is true, loop through all meshes and render them
    if (show_all_meshes) {
        for (int mesh_index = 0; mesh_index < get_num_meshes(); mesh_index++) {
            mesh_t* mesh = get_mesh(mesh_index);
            process_graphics_pipeline_stages(mesh);
        }
    }
    else {
        // Otherwise, only render the current mesh
        mesh_t* current_mesh = get_current_mesh();
        process_graphics_pipeline_stages(current_mesh);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function frame by frame with a fixed time step
///////////////////////////////////////////////////////////////////////////////
void update(void) {
    // Wait some time until the reach the target frame time in milliseconds
    cap_fps();

    // Get a delta time factor converted to seconds to be used to update our game objects
    delta_time = (SDL_GetTicks() - previous_frame_time) / 1000.0;
    previous_frame_time = SDL_GetTicks();

    // Initialize the counter of triangles to render for the current frame
    triangles_to_render_count = 0;

    if (show_fps)
    {
        show_fps_console();
    }

    if (rotate_meshes) {
        rotate_mesh_y(ROTATION_SPEED * delta_time);
    }

    if (play_animation) {
        animation_airport(5.0f);
    }

    render_meshes();
}



///////////////////////////////////////////////////////////////////////////////
// Render function to draw objects on the display
///////////////////////////////////////////////////////////////////////////////
void render(void) {
    // Clear all the arrays to get ready for the next frame
    clear_color_buffer(0xFF000000);
    clear_z_buffer();
    
    draw_grid();

    // Loop all triangles from the triangles_to_render array
    for (int i = 0; i < triangles_to_render_count; i++) {
        triangle_t triangle = triangles_to_render[i];

        // Draw filled triangle
        if (should_render_filled_triangle()){
            draw_filled_triangle(
                triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w, // vertex A
                triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w, // vertex B
                triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w, // vertex C
                triangle.color
            );
        }

        // Draw textured triangle
        if (should_render_textured_triangle()) {
            draw_textured_triangle(
                triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w, triangle.texcoords[0].u, triangle.texcoords[0].v, // vertex A
                triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w, triangle.texcoords[1].u, triangle.texcoords[1].v, // vertex B
                triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w, triangle.texcoords[2].u, triangle.texcoords[2].v, // vertex C
                triangle.texture
            );
        }

        // Draw triangle wireframe
        if (should_render_wire()) {
            draw_triangle(
                triangle.points[0].x, triangle.points[0].y, // vertex A
                triangle.points[1].x, triangle.points[1].y, // vertex B
                triangle.points[2].x, triangle.points[2].y, // vertex C
                0xFFFFFFFF
            );
        }

        // Draw triangle vertex points
        if (should_render_wire_vertex()) {
            draw_rect(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6, 0xFF0000FF); // vertex A
            draw_rect(triangle.points[1].x - 3, triangle.points[1].y - 3, 6, 6, 0xFF0000FF); // vertex B
            draw_rect(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6, 0xFF0000FF); // vertex C
        }
    }

    // Finally draw the color buffer to the SDL window
    render_color_buffer();
}

///////////////////////////////////////////////////////////////////////////////
// Free the memory that was dynamically allocated by the program
///////////////////////////////////////////////////////////////////////////////
void free_resources(void) {
    free_meshes();
    destroy_window();
}

///////////////////////////////////////////////////////////////////////////////
// Main function
///////////////////////////////////////////////////////////////////////////////
int main(void) {
    is_running = init_window();

    setup();

    while (is_running) {
        process_input(delta_time, &is_running);
        update();
        render();
    }

    free_resources();

    return 0;
}
