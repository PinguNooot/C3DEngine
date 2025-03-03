#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"
#include "upng.h"

#define MESH_DEFAULT_POS vec3_new(0, -0.5, 5)

extern int current_mesh_index;

typedef struct {
    vec3_t* vertices;         // mesh dynamic array of vertices
    face_t* faces;            // mesh dynamic array of faces
    upng_t* texture;          // mesh PNG texture
    vec3_t scale;             // mesh scale in x, y, and z
    vec3_t rotation;          // mesh rotation in x, y, and z
    vec3_t translation;       // mesh translation in x, y, and z
} mesh_t;

typedef struct {
    vec3_t scale;
    vec3_t rotation;
    vec3_t translation;
} mesh_transform_t;            // store the original transformation of the mesh

//void initialize_meshes(void);
void load_mesh_obj_data(mesh_t* mesh, char* obj_filename);
void load_mesh_png_data(mesh_t* mesh, char* png_filename);

void load_mesh(char* obj_filename, char* png_filename, vec3_t scale, vec3_t translation, vec3_t rotation);
void swap_mesh(void);  
void swap_to_mesh(int mesh_index);
void swap_mesh_index(int direction);

mesh_t* get_mesh(int mesh_index);
mesh_t* get_current_mesh(void);
int get_num_meshes(void);

void rotate_mesh_x(float angle);
void rotate_mesh_y(float angle);
void rotate_mesh_z(float angle);

void reset_mesh_position(int mesh_index);
void reset_all_meshes(void);
void free_meshes(void);

#endif
