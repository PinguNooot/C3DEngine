#include <stdio.h>
#include <string.h>
#include "array.h"
#include "mesh.h"
#include "input.h"

#define MAX_NUM_MESHES 100
static mesh_t meshes[MAX_NUM_MESHES];
static mesh_transform_t original_transforms[MAX_NUM_MESHES];
static int mesh_count = 0;
static int current_mesh_index = 1;

void load_mesh_obj_data(mesh_t* mesh, char* obj_filename) {
    FILE* file;
    fopen_s(&file,obj_filename, "r");
    char line[1024];
    tex2_t* texcoords = NULL;
    while (fgets(line, 1024, file)) {
        // Vertex information
        if (strncmp(line, "v ", 2) == 0) {
            vec3_t vertex;
            sscanf_s(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            array_push(meshes[mesh_count].vertices, vertex);
        }
        // Texture coordinate information
        if (strncmp(line, "vt ", 3) == 0) {
            tex2_t texcoord;
            sscanf_s(line, "vt %f %f", &texcoord.u, &texcoord.v);
            array_push(texcoords, texcoord);
        }
        // Face information
        if (strncmp(line, "f ", 2) == 0) {
            int vertex_indices[3];
            int texture_indices[3];
            int normal_indices[3];
            sscanf_s(
                line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                &vertex_indices[0], &texture_indices[0], &normal_indices[0],
                &vertex_indices[1], &texture_indices[1], &normal_indices[1],
                &vertex_indices[2], &texture_indices[2], &normal_indices[2]
            );
            face_t face = {
                .a = vertex_indices[0],
                .b = vertex_indices[1],
                .c = vertex_indices[2],
                .a_uv = texcoords[texture_indices[0] - 1],
                .b_uv = texcoords[texture_indices[1] - 1],
                .c_uv = texcoords[texture_indices[2] - 1],
                .color = 0xFFFFFFFF
            };
            array_push(meshes[mesh_count].faces, face);
        }
    }
    array_free(texcoords);
    fclose(file);
}

void load_mesh_png_data(mesh_t* mesh, char* png_filename) {
    upng_t* png_image = upng_new_from_file(png_filename);
    if (png_image != NULL) {
        upng_decode(png_image);
        if (upng_get_error(png_image) == UPNG_EOK) {
            mesh->texture = png_image;
        }
    }
}

void load_mesh(char* obj_filename, char* png_filename, vec3_t scale, vec3_t translation, vec3_t rotation) {
    load_mesh_obj_data(&meshes[mesh_count], obj_filename);
    load_mesh_png_data(&meshes[mesh_count], png_filename);

    meshes[mesh_count].scale = scale;
    meshes[mesh_count].translation = translation;
    meshes[mesh_count].rotation = rotation;

    // Store original positions and rotations
    original_transforms[mesh_count].scale = scale;
    original_transforms[mesh_count].rotation = rotation;
    original_transforms[mesh_count].translation = translation;

    mesh_count++;
}

void swap_mesh(void) {
    current_mesh_index++;
    if (current_mesh_index >= mesh_count) {
        current_mesh_index = 1;
    }

    meshes[current_mesh_index].translation = MESH_DEFAULT_POS;
    meshes[current_mesh_index].rotation = vec3_new(0, 0, 0);

    printf("Swapped to mesh %d\n", current_mesh_index);
}


void swap_to_mesh(int mesh_index) {
    if (mesh_index < 0 || mesh_index >= mesh_count) return;

    current_mesh_index = mesh_index;

    meshes[mesh_index].translation = MESH_DEFAULT_POS;
    meshes[mesh_index].rotation = vec3_new(0, 0, 0);
    meshes[mesh_index].scale = original_transforms[mesh_index].scale;
}

void swap_mesh_index(int direction) {
    if (mesh_count <= 1) return; 

    current_mesh_index += direction;
    if (current_mesh_index >= mesh_count) {
        current_mesh_index = 1;  
    }
    else if (current_mesh_index < 1) {
        current_mesh_index = mesh_count - 1;  
    }

    meshes[current_mesh_index].translation = MESH_DEFAULT_POS;
    meshes[current_mesh_index].rotation = vec3_new(0, 0, 0);
    meshes[current_mesh_index].scale = original_transforms[current_mesh_index].scale;

    printf("Swapped to mesh %d\n", current_mesh_index);
}

mesh_t* get_mesh(int mesh_index) {
    return &meshes[mesh_index];
}

mesh_t* get_current_mesh(void)
{
    return &meshes[current_mesh_index];
}

int get_num_meshes(void) {
    return mesh_count;
}

void rotate_mesh_x(float angle) {
	if (current_mesh_index < 1) return;
    meshes[current_mesh_index].rotation.x += angle;
}

void rotate_mesh_y(float angle) {
    if (current_mesh_index < 1) return;
    meshes[current_mesh_index].rotation.y += angle;
}

void rotate_mesh_z(float angle) {
    if (current_mesh_index < 1) return;
    meshes[current_mesh_index].rotation.z += angle;
}

void reset_mesh_position(int mesh_index) {
    if (mesh_index < 0) return;

    meshes[mesh_index].translation = original_transforms[mesh_index].translation;
    meshes[mesh_index].rotation = original_transforms[mesh_index].rotation;
    meshes[mesh_index].scale = original_transforms[mesh_index].scale;
}

void reset_all_meshes(void) {
    for (int i = 0; i < mesh_count; i++) {
        reset_mesh_position(i);
    }
    current_mesh_index = -1;
}



void free_meshes(void) {
    for (int i = 0; i < mesh_count; i++) {
        array_free(meshes[i].faces);
        array_free(meshes[i].vertices);
        upng_free(meshes[i].texture);
    }
}
