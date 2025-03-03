#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

// Structure representing a 4x4 matrix.
typedef struct {
    float m[4][4];
} mat4_t;

// Returns the identity matrix.
mat4_t mat4_identity(void);

// Creates a scaling matrix with given scale factors.
mat4_t mat4_make_scale(float sx, float sy, float sz);

// Creates a translation matrix with given translation values.
mat4_t mat4_make_translation(float tx, float ty, float tz);

// Creates a rotation matrix around the X-axis.
mat4_t mat4_make_rotation_x(float angle);

// Creates a rotation matrix around the Y-axis.
mat4_t mat4_make_rotation_y(float angle);

// Creates a rotation matrix around the Z-axis.
mat4_t mat4_make_rotation_z(float angle);

// Creates a perspective projection matrix.
mat4_t mat4_make_perspective(float fov, float aspect, float znear, float zfar);

// Multiplies a 4x4 matrix with a 4D vector.
vec4_t mat4_mul_vec4(mat4_t m, vec4_t v);

// Multiplies two 4x4 matrices.
mat4_t mat4_mul_mat4(mat4_t a, mat4_t b);

// Creates a view matrix for a camera using the LookAt method.
mat4_t mat4_look_at(vec3_t eye, vec3_t target, vec3_t up);

#endif // MATRIX_H
