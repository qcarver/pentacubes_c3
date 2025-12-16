#include "pentacubes.h"

static const float pentacube_A_verts[][3] = {
    {1, 0, 1},
    {1, 0, 0},
    {2, 0, 1},
    {2, 1, 1},
    {2, 0, 0},
    {2, 1, 0},
    {3, 1, 1},
    {3, 2, 1},
    {3, 1, 0},
    {3, 2, 0},
    {1, 1, 0},
    {1, 2, 0},
    {0, 1, 1},
    {0, 1, 0},
    {1, 1, 1},
    {0, 3, 1},
    {0, 3, 0},
    {1, 2, 1},
    {1, 3, 1},
    {1, 3, 0},
};

static const float pentacube_A_normals[][3] = {
    {-1.000000f, 0.000000f, 0.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {0.000000f, -1.000000f, -0.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {0.000000f, -1.000000f, -0.000000f},
    {-1.000000f, -0.000000f, 0.000000f},
    {0.000000f, 1.000000f, 0.000000f},
    {0.000000f, -1.000000f, -0.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {0.000000f, 1.000000f, 0.000000f},
    {0.000000f, 0.000000f, 1.000000f},
    {0.000000f, 0.000000f, -1.000000f},
};

static const int pentacube_A_face_verts[] = {0, 14, 10, 1, 4, 5, 3, 2, 1, 4, 2, 0, 8, 9, 7, 6, 5, 8, 6, 3, 13, 12, 15, 16, 9, 11, 17, 7, 13, 10, 14, 12, 11, 19, 18, 17, 19, 16, 15, 18, 2, 3, 6, 7, 17, 18, 15, 12, 14, 0, 4, 1, 10, 13, 16, 19, 11, 9, 8, 5};

static const int pentacube_A_face_counts[] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 10, 10};

static const float pentacube_B_verts[][3] = {
    {1, 0, 1},
    {1, 1, 1},
    {1, 0, 0},
    {2, 0, 1},
    {2, 0, 0},
    {1, 2, 0},
    {2, 2, 0},
    {2, 2, 1},
    {2, 1, 1},
    {3, 1, 2},
    {3, 2, 2},
    {3, 1, 1},
    {3, 2, 1},
    {0, 1, 2},
    {0, 2, 2},
    {0, 1, 1},
    {0, 2, 1},
    {1, 2, 1},
};

static const float pentacube_B_normals[][3] = {
    {-0.000000f, 0.000000f, 1.000000f},
    {0.000000f, -1.000000f, -0.000000f},
    {-0.000000f, 0.000000f, -1.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {0.000000f, -1.000000f, 0.000000f},
    {0.000000f, 1.000000f, 0.000000f},
    {-0.000000f, 0.000000f, -1.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {-0.000000f, 0.000000f, -1.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {0.000000f, -0.000000f, 1.000000f},
};

static const int pentacube_B_face_verts[] = {3, 8, 1, 0, 2, 4, 3, 0, 2, 5, 6, 4, 4, 6, 7, 8, 3, 1, 8, 11, 9, 13, 15, 6, 5, 17, 16, 14, 10, 12, 7, 8, 7, 12, 11, 11, 12, 10, 9, 13, 14, 16, 15, 15, 16, 17, 1, 0, 1, 17, 5, 2, 10, 14, 13, 9};

static const int pentacube_B_face_counts[] = {4, 4, 4, 5, 6, 8, 4, 4, 4, 4, 5, 4};

static const float pentacube_E_verts[][3] = {
    {1, 0, 1},
    {1, 0, 2},
    {2, 0, 1},
    {2, 0, 2},
    {2, 1, 1},
    {2, 1, 2},
    {0, 1, 0},
    {0, 2, 0},
    {1, 1, 0},
    {1, 2, 0},
    {1, 1, 1},
    {1, 1, 2},
    {3, 1, 1},
    {3, 1, 2},
    {3, 2, 1},
    {3, 2, 2},
    {0, 1, 2},
    {0, 2, 2},
    {1, 2, 1},
};

static const float pentacube_E_normals[][3] = {
    {-1.000000f, -0.000000f, 0.000000f},
    {1.000000f, -0.000000f, 0.000000f},
    {0.000000f, -1.000000f, 0.000000f},
    {1.000000f, -0.000000f, 0.000000f},
    {0.000000f, 0.000000f, -1.000000f},
    {1.000000f, -0.000000f, 0.000000f},
    {0.000000f, -1.000000f, 0.000000f},
    {-1.000000f, -0.000000f, 0.000000f},
    {0.000000f, 0.000000f, 1.000000f},
    {0.000000f, 0.000000f, -1.000000f},
    {0.000000f, 1.000000f, 0.000000f},
    {0.000000f, -1.000000f, -0.000000f},
};

static const int pentacube_E_face_verts[] = {0, 1, 11, 10, 4, 5, 3, 2, 2, 3, 1, 0, 9, 18, 10, 8, 7, 9, 8, 6, 14, 15, 13, 12, 12, 13, 5, 4, 6, 16, 17, 7, 11, 1, 3, 5, 13, 15, 17, 16, 4, 2, 0, 10, 18, 14, 12, 7, 17, 15, 14, 18, 9, 6, 8, 11, 16};

static const int pentacube_E_face_counts[] = {4, 4, 4, 4, 4, 4, 4, 4, 8, 7, 6, 4};

static const float pentacube_E_prime_verts[][3] = {
    {2, 0, 1},
    {2, 0, 2},
    {1, 0, 1},
    {1, 0, 2},
    {1, 1, 1},
    {1, 1, 2},
    {3, 1, 0},
    {3, 2, 0},
    {2, 1, 0},
    {2, 2, 0},
    {2, 1, 1},
    {2, 1, 2},
    {0, 1, 1},
    {0, 1, 2},
    {0, 2, 1},
    {0, 2, 2},
    {3, 1, 2},
    {3, 2, 2},
    {2, 2, 1},
};

static const float pentacube_E_prime_normals[][3] = {
    {1.000000f, 0.000000f, 0.000000f},
    {-1.000000f, 0.000000f, -0.000000f},
    {0.000000f, -1.000000f, -0.000000f},
    {-1.000000f, 0.000000f, -0.000000f},
    {0.000000f, 0.000000f, -1.000000f},
    {-1.000000f, 0.000000f, -0.000000f},
    {0.000000f, -1.000000f, -0.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {0.000000f, 0.000000f, 1.000000f},
    {0.000000f, 0.000000f, -1.000000f},
    {0.000000f, 1.000000f, 0.000000f},
    {0.000000f, -1.000000f, 0.000000f},
};

static const int pentacube_E_prime_face_verts[] = {0, 10, 11, 1, 4, 2, 3, 5, 2, 0, 1, 3, 9, 8, 10, 18, 7, 6, 8, 9, 14, 12, 13, 15, 12, 4, 5, 13, 6, 7, 17, 16, 11, 16, 17, 15, 13, 5, 3, 1, 4, 12, 14, 18, 10, 0, 2, 7, 9, 18, 14, 15, 17, 6, 16, 11, 8};

static const int pentacube_E_prime_face_counts[] = {4, 4, 4, 4, 4, 4, 4, 4, 8, 7, 6, 4};

static const float pentacube_F_verts[][3] = {
    {3, 1, 0},
    {3, 1, 1},
    {3, 2, 0},
    {3, 2, 1},
    {0, 3, 0},
    {0, 3, 1},
    {1, 3, 0},
    {1, 3, 1},
    {1, 2, 0},
    {1, 0, 0},
    {1, 0, 1},
    {1, 1, 1},
    {2, 0, 0},
    {2, 0, 1},
    {2, 1, 0},
    {2, 1, 1},
    {0, 1, 0},
    {0, 1, 1},
    {1, 1, 0},
    {1, 2, 1},
};

static const float pentacube_F_normals[][3] = {
    {1.000000f, -0.000000f, 0.000000f},
    {0.000000f, -1.000000f, 0.000000f},
    {0.000000f, 1.000000f, 0.000000f},
    {1.000000f, -0.000000f, 0.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {0.000000f, 0.000000f, 1.000000f},
    {-1.000000f, -0.000000f, 0.000000f},
    {1.000000f, -0.000000f, 0.000000f},
    {0.000000f, -1.000000f, 0.000000f},
    {0.000000f, -1.000000f, 0.000000f},
    {-0.000000f, 1.000000f, 0.000000f},
    {0.000000f, 0.000000f, -1.000000f},
};

static const int pentacube_F_face_verts[] = {2, 3, 1, 0, 0, 1, 15, 14, 4, 5, 7, 6, 6, 7, 19, 8, 5, 4, 16, 17, 3, 19, 7, 5, 17, 11, 10, 13, 15, 1, 9, 10, 11, 18, 14, 15, 13, 12, 12, 13, 10, 9, 18, 11, 17, 16, 3, 2, 8, 19, 8, 2, 0, 14, 12, 9, 18, 16, 4, 6};

static const int pentacube_F_face_counts[] = {4, 4, 4, 4, 4, 10, 4, 4, 4, 4, 4, 10};

static const float pentacube_G_verts[][3] = {
    {1, 1, 1},
    {1, 1, 2},
    {0, 0, 1},
    {0, 0, 2},
    {0, 1, 1},
    {0, 1, 2},
    {3, 1, 0},
    {3, 2, 0},
    {2, 1, 0},
    {2, 2, 0},
    {1, 2, 1},
    {1, 2, 2},
    {2, 0, 1},
    {2, 0, 2},
    {3, 1, 2},
    {3, 2, 2},
    {2, 1, 1},
    {2, 1, 2},
    {2, 2, 1},
};

static const float pentacube_G_normals[][3] = {
    {0.000000f, 1.000000f, 0.000000f},
    {-1.000000f, 0.000000f, -0.000000f},
    {-1.000000f, 0.000000f, -0.000000f},
    {0.000000f, 0.000000f, -1.000000f},
    {-1.000000f, 0.000000f, -0.000000f},
    {0.000000f, 0.000000f, 1.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {0.000000f, -1.000000f, 0.000000f},
    {0.000000f, 0.000000f, -1.000000f},
    {0.000000f, -1.000000f, -0.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {0.000000f, 1.000000f, 0.000000f},
};

static const int pentacube_G_face_verts[] = {0, 4, 5, 1, 4, 2, 3, 5, 9, 8, 16, 18, 7, 6, 8, 9, 10, 0, 1, 11, 5, 3, 13, 17, 14, 15, 11, 1, 12, 16, 17, 13, 8, 6, 14, 17, 16, 0, 10, 18, 16, 12, 2, 4, 2, 12, 13, 3, 6, 7, 15, 14, 7, 9, 18, 10, 11, 15};

static const int pentacube_G_face_counts[] = {4, 4, 4, 4, 4, 8, 4, 5, 7, 4, 4, 6};

static const float pentacube_G_prime_verts[][3] = {
    {2, 1, 1},
    {2, 1, 2},
    {3, 0, 1},
    {3, 0, 2},
    {3, 1, 1},
    {3, 1, 2},
    {0, 1, 0},
    {0, 2, 0},
    {1, 1, 0},
    {1, 2, 0},
    {2, 2, 1},
    {2, 2, 2},
    {1, 0, 1},
    {1, 0, 2},
    {0, 1, 2},
    {0, 2, 2},
    {1, 1, 1},
    {1, 1, 2},
    {1, 2, 1},
};

static const float pentacube_G_prime_normals[][3] = {
    {0.000000f, 1.000000f, 0.000000f},
    {1.000000f, -0.000000f, 0.000000f},
    {1.000000f, -0.000000f, 0.000000f},
    {0.000000f, 0.000000f, -1.000000f},
    {1.000000f, -0.000000f, 0.000000f},
    {0.000000f, 0.000000f, 1.000000f},
    {-1.000000f, -0.000000f, 0.000000f},
    {0.000000f, -1.000000f, 0.000000f},
    {0.000000f, 0.000000f, -1.000000f},
    {0.000000f, -1.000000f, 0.000000f},
    {-1.000000f, -0.000000f, 0.000000f},
    {0.000000f, 1.000000f, 0.000000f},
};

static const int pentacube_G_prime_face_verts[] = {0, 1, 5, 4, 4, 5, 3, 2, 9, 18, 16, 8, 7, 9, 8, 6, 10, 11, 1, 0, 5, 1, 11, 15, 14, 17, 13, 3, 12, 13, 17, 16, 8, 16, 17, 14, 6, 0, 4, 2, 12, 16, 18, 10, 2, 3, 13, 12, 6, 14, 15, 7, 7, 15, 11, 10, 18, 9};

static const int pentacube_G_prime_face_counts[] = {4, 4, 4, 4, 4, 8, 4, 5, 7, 4, 4, 6};

static const float pentacube_H_verts[][3] = {
    {0, 1, 0},
    {0, 2, 0},
    {1, 1, 1},
    {0, 1, 1},
    {0, 2, 1},
    {2, 1, 0},
    {2, 2, 0},
    {2, 1, 1},
    {2, 2, 1},
    {3, 2, 2},
    {3, 2, 1},
    {1, 1, 2},
    {1, 2, 2},
    {1, 2, 1},
    {3, 0, 2},
    {2, 0, 2},
    {2, 1, 2},
    {3, 0, 1},
    {2, 0, 1},
};

static const float pentacube_H_normals[][3] = {
    {0.000000f, -0.000000f, 1.000000f},
    {0.000000f, 0.000000f, -1.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {0.000000f, -0.000000f, -1.000000f},
    {1.000000f, -0.000000f, 0.000000f},
    {0.000000f, 1.000000f, 0.000000f},
    {0.000000f, -1.000000f, 0.000000f},
    {-1.000000f, 0.000000f, -0.000000f},
    {1.000000f, 0.000000f, -0.000000f},
    {0.000000f, -1.000000f, 0.000000f},
    {0.000000f, 0.000000f, 1.000000f},
    {-1.000000f, 0.000000f, -0.000000f},
};

static const int pentacube_H_face_verts[] = {13, 4, 3, 2, 10, 17, 18, 7, 8, 4, 1, 0, 3, 5, 0, 1, 6, 6, 8, 7, 5, 1, 4, 13, 12, 9, 10, 8, 6, 2, 3, 0, 5, 7, 16, 11, 13, 2, 11, 12, 9, 14, 17, 10, 17, 14, 15, 18, 16, 15, 14, 9, 12, 11, 7, 18, 15, 16};

static const int pentacube_H_face_counts[] = {4, 5, 4, 4, 4, 8, 7, 4, 4, 4, 6, 4};

static const float pentacube_H_prime_verts[][3] = {
    {3, 1, 0},
    {3, 2, 0},
    {2, 1, 1},
    {3, 1, 1},
    {3, 2, 1},
    {1, 1, 0},
    {1, 2, 0},
    {1, 1, 1},
    {1, 2, 1},
    {0, 2, 2},
    {0, 2, 1},
    {2, 1, 2},
    {2, 2, 2},
    {2, 2, 1},
    {0, 0, 2},
    {1, 0, 2},
    {1, 1, 2},
    {0, 0, 1},
    {1, 0, 1},
};

static const float pentacube_H_prime_normals[][3] = {
    {0.000000f, 0.000000f, 1.000000f},
    {0.000000f, 0.000000f, -1.000000f},
    {1.000000f, 0.000000f, -0.000000f},
    {-0.000000f, 0.000000f, -1.000000f},
    {-1.000000f, 0.000000f, -0.000000f},
    {0.000000f, 1.000000f, 0.000000f},
    {0.000000f, -1.000000f, 0.000000f},
    {1.000000f, -0.000000f, 0.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {0.000000f, -1.000000f, -0.000000f},
    {0.000000f, 0.000000f, 1.000000f},
    {1.000000f, -0.000000f, 0.000000f},
};

static const int pentacube_H_prime_face_verts[] = {13, 2, 3, 4, 10, 8, 7, 18, 17, 4, 3, 0, 1, 5, 6, 1, 0, 6, 5, 7, 8, 1, 6, 8, 10, 9, 12, 13, 4, 2, 11, 16, 7, 5, 0, 3, 13, 12, 11, 2, 9, 10, 17, 14, 17, 18, 15, 14, 16, 11, 12, 9, 14, 15, 7, 16, 15, 18};

static const int pentacube_H_prime_face_counts[] = {4, 5, 4, 4, 4, 8, 7, 4, 4, 4, 6, 4};

static const float pentacube_I_verts[][3] = {
    {5, 0, 1},
    {5, 1, 1},
    {5, 0, 0},
    {5, 1, 0},
    {0, 0, 1},
    {0, 1, 1},
    {0, 0, 0},
    {0, 1, 0},
};

static const float pentacube_I_normals[][3] = {
    {1.000000f, 0.000000f, 0.000000f},
    {0.000000f, -1.000000f, 0.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {0.000000f, 0.000000f, -1.000000f},
    {0.000000f, -0.000000f, 1.000000f},
    {0.000000f, 1.000000f, 0.000000f},
};

static const int pentacube_I_face_verts[] = {2, 3, 1, 0, 2, 0, 4, 6, 4, 5, 7, 6, 3, 2, 6, 7, 1, 5, 4, 0, 3, 7, 5, 1};

static const int pentacube_I_face_counts[] = {4, 4, 4, 4, 4, 4};

static const float pentacube_J_verts[][3] = {
    {1, 0, 1},
    {1, 1, 1},
    {0, 0, 1},
    {0, 1, 1},
    {1, 0, 0},
    {0, 0, 0},
    {3, 1, 2},
    {1, 1, 1},
    {3, 2, 2},
    {3, 1, 1},
    {3, 2, 1},
    {0, 1, 2},
    {0, 2, 2},
    {1, 1, 1},
    {0, 1, 1},
    {1, 2, 1},
    {0, 1, 1},
    {1, 2, 0},
    {0, 2, 0},
};

static const float pentacube_J_normals[][3] = {
    {-0.000000f, -1.000000f, 0.000000f},
    {1.000000f, 0.000000f, -0.000000f},
    {0.000000f, -0.000000f, -1.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {0.000000f, 0.000000f, -1.000000f},
    {0.000000f, 0.000000f, 1.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {0.000000f, 1.000000f, 0.000000f},
    {0.000000f, -0.000000f, 1.000000f},
    {0.000000f, -1.000000f, 0.000000f},
};

static const int pentacube_J_face_verts[] = {2, 5, 4, 0, 8, 6, 9, 10, 4, 5, 18, 17, 5, 2, 16, 11, 12, 18, 15, 10, 9, 13, 2, 0, 13, 16, 0, 4, 17, 15, 13, 10, 15, 17, 18, 12, 8, 8, 12, 11, 6, 16, 13, 9, 6, 11};

static const int pentacube_J_face_counts[] = {4, 4, 4, 6, 4, 4, 5, 6, 4, 5};

static const float pentacube_J_prime_verts[][3] = {
    {2, 2, 1},
    {2, 1, 1},
    {3, 2, 1},
    {3, 1, 1},
    {2, 2, 2},
    {3, 2, 2},
    {0, 1, 0},
    {2, 1, 1},
    {0, 0, 0},
    {0, 1, 1},
    {0, 0, 1},
    {3, 1, 0},
    {3, 0, 0},
    {2, 1, 1},
    {3, 1, 1},
    {2, 0, 1},
    {3, 1, 1},
    {2, 0, 2},
    {3, 0, 2},
};

static const float pentacube_J_prime_normals[][3] = {
    {0.000000f, 1.000000f, 0.000000f},
    {-1.000000f, -0.000000f, 0.000000f},
    {0.000000f, 0.000000f, 1.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {-0.000000f, 0.000000f, 1.000000f},
    {0.000000f, 0.000000f, -1.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {0.000000f, -1.000000f, 0.000000f},
    {-0.000000f, 0.000000f, -1.000000f},
    {0.000000f, 1.000000f, 0.000000f},
};

static const int pentacube_J_prime_face_verts[] = {2, 0, 4, 5, 8, 10, 9, 6, 4, 17, 18, 5, 5, 18, 12, 11, 16, 2, 15, 13, 9, 10, 2, 16, 13, 0, 0, 13, 15, 17, 4, 10, 8, 12, 18, 17, 15, 8, 6, 11, 12, 16, 11, 6, 9, 13};

static const int pentacube_J_prime_face_counts[] = {4, 4, 4, 6, 4, 4, 5, 6, 4, 5};

static const float pentacube_K_verts[][3] = {
    {2, 1, 1},
    {2, 2, 1},
    {3, 1, 2},
    {3, 2, 2},
    {3, 1, 1},
    {3, 2, 1},
    {2, 1, 2},
    {2, 2, 2},
    {0, 1, 2},
    {0, 2, 2},
    {1, 2, 2},
    {1, 1, 1},
    {1, 2, 1},
    {0, 2, 1},
    {1, 1, 0},
    {1, 2, 0},
    {0, 1, 0},
    {0, 2, 0},
    {1, 0, 2},
    {1, 1, 2},
    {0, 0, 2},
    {1, 0, 1},
    {0, 0, 1},
    {0, 1, 1},
};

static const float pentacube_K_normals[][3] = {
    {0.000000f, -1.000000f, 0.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {0.000000f, 0.000000f, -1.000000f},
    {-0.000000f, 0.000000f, -1.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {-0.000000f, -1.000000f, 0.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {0.000000f, 1.000000f, 0.000000f},
    {-0.000000f, 0.000000f, -1.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {-0.000000f, -1.000000f, 0.000000f},
    {0.000000f, 0.000000f, 1.000000f},
};

static const int pentacube_K_face_verts[] = {0, 4, 2, 6, 19, 11, 4, 5, 3, 2, 1, 5, 4, 0, 11, 12, 16, 17, 15, 14, 14, 15, 12, 11, 23, 16, 14, 11, 8, 9, 13, 17, 16, 23, 22, 20, 5, 1, 12, 15, 17, 13, 9, 10, 7, 3, 22, 23, 11, 21, 21, 11, 19, 18, 20, 22, 21, 18, 2, 3, 7, 10, 9, 8, 20, 18, 19, 6};

static const int pentacube_K_face_counts[] = {6, 4, 6, 4, 4, 4, 8, 10, 4, 4, 4, 10};

static const float pentacube_L_verts[][3] = {
    {4, 0, 2},
    {4, 1, 2},
    {0, 0, 2},
    {0, 1, 2},
    {0, 0, 1},
    {0, 1, 1},
    {3, 0, 1},
    {3, 1, 1},
    {4, 0, 0},
    {4, 1, 0},
    {3, 0, 0},
    {3, 1, 0},
};

static const float pentacube_L_normals[][3] = {
    {-0.000000f, 0.000000f, 1.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {1.000000f, -0.000000f, 0.000000f},
    {0.000000f, -1.000000f, 0.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {-0.000000f, 0.000000f, -1.000000f},
    {0.000000f, 0.000000f, -1.000000f},
    {0.000000f, 1.000000f, 0.000000f},
};

static const int pentacube_L_face_verts[] = {0, 1, 3, 2, 2, 3, 5, 4, 9, 1, 0, 8, 8, 0, 2, 4, 6, 10, 6, 7, 11, 10, 10, 11, 9, 8, 7, 6, 4, 5, 1, 9, 11, 7, 5, 3};

static const int pentacube_L_face_counts[] = {4, 4, 4, 6, 4, 4, 4, 6};

static const float pentacube_M_verts[][3] = {
    {2, 2, 2},
    {2, 2, 1},
    {3, 1, 2},
    {3, 2, 2},
    {3, 1, 1},
    {3, 2, 1},
    {2, 1, 2},
    {0, 1, 2},
    {0, 2, 2},
    {0, 1, 1},
    {0, 2, 1},
    {1, 2, 2},
    {1, 2, 1},
    {2, 1, 0},
    {2, 2, 0},
    {1, 1, 0},
    {1, 2, 0},
    {2, 0, 2},
    {1, 0, 2},
    {1, 1, 2},
    {2, 0, 1},
    {2, 1, 1},
    {1, 0, 1},
    {1, 1, 1},
};

static const float pentacube_M_normals[][3] = {
    {-0.000000f, 0.000000f, -1.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {0.000000f, -1.000000f, -0.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {-0.000000f, 0.000000f, -1.000000f},
    {0.000000f, -1.000000f, -0.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {-0.000000f, 0.000000f, -1.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {-0.000000f, -1.000000f, 0.000000f},
    {0.000000f, 1.000000f, 0.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {-0.000000f, 0.000000f, -1.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {-0.000000f, -1.000000f, 0.000000f},
    {0.000000f, 0.000000f, 1.000000f},
};

static const int pentacube_M_face_verts[] = {21, 1, 5, 4, 4, 5, 3, 2, 21, 4, 2, 6, 7, 8, 10, 9, 9, 10, 12, 23, 9, 23, 19, 7, 23, 12, 16, 15, 15, 16, 14, 13, 13, 14, 1, 21, 23, 15, 13, 21, 5, 1, 14, 16, 12, 10, 8, 11, 0, 3, 18, 19, 23, 22, 22, 23, 21, 20, 20, 21, 6, 17, 18, 22, 20, 17, 2, 3, 0, 11, 8, 7, 19, 18, 17, 6};

static const int pentacube_M_face_counts[] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 10, 4, 4, 4, 4, 10};

static const float pentacube_N_verts[][3] = {
    {0, 0, 1},
    {0, 1, 1},
    {1, 1, 1},
    {0, 0, 2},
    {0, 1, 2},
    {2, 0, 1},
    {2, 1, 1},
    {2, 0, 2},
    {2, 1, 2},
    {4, 0, 0},
    {4, 1, 0},
    {4, 0, 1},
    {4, 1, 1},
    {1, 0, 0},
    {1, 1, 0},
    {1, 0, 1},
};

static const float pentacube_N_normals[][3] = {
    {-0.000000f, 0.000000f, -1.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {0.000000f, 0.000000f, 1.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {0.000000f, -1.000000f, 0.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {0.000000f, 0.000000f, 1.000000f},
    {0.000000f, 0.000000f, -1.000000f},
    {0.000000f, 1.000000f, 0.000000f},
};

static const int pentacube_N_face_verts[] = {0, 1, 2, 15, 3, 4, 1, 0, 5, 6, 8, 7, 6, 5, 11, 12, 9, 10, 12, 11, 3, 0, 15, 13, 9, 11, 5, 7, 15, 2, 14, 13, 4, 3, 7, 8, 14, 10, 9, 13, 2, 1, 4, 8, 6, 12, 10, 14};

static const int pentacube_N_face_counts[] = {4, 4, 4, 4, 4, 8, 4, 4, 4, 8};

static const float pentacube_P_verts[][3] = {
    {0, 0, 0},
    {0, 1, 0},
    {3, 0, 2},
    {3, 1, 2},
    {3, 0, 1},
    {3, 1, 1},
    {2, 0, 1},
    {0, 0, 2},
    {0, 1, 2},
    {2, 1, 1},
    {2, 0, 0},
    {2, 1, 0},
};

static const float pentacube_P_normals[][3] = {
    {-0.000000f, 0.000000f, -1.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {-0.000000f, 0.000000f, 1.000000f},
    {0.000000f, -1.000000f, 0.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {0.000000f, 1.000000f, 0.000000f},
    {-0.000000f, 0.000000f, -1.000000f},
};

static const int pentacube_P_face_verts[] = {6, 9, 5, 4, 4, 5, 3, 2, 2, 3, 8, 7, 7, 0, 10, 6, 4, 2, 8, 1, 0, 7, 10, 11, 9, 6, 1, 8, 3, 5, 9, 11, 0, 1, 11, 10};

static const int pentacube_P_face_counts[] = {4, 4, 4, 6, 4, 4, 6, 4};

static const float pentacube_Q_verts[][3] = {
    {0, 0, 2},
    {0, 1, 2},
    {2, 1, 1},
    {2, 2, 1},
    {1, 2, 1},
    {2, 2, 0},
    {1, 1, 0},
    {1, 2, 0},
    {2, 0, 2},
    {2, 1, 2},
    {0, 0, 0},
    {0, 1, 0},
    {1, 1, 1},
    {2, 0, 0},
};

static const float pentacube_Q_normals[][3] = {
    {-0.000000f, 0.000000f, 1.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {0.000000f, 1.000000f, 0.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {0.000000f, 0.000000f, -1.000000f},
    {0.000000f, 0.000000f, 1.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {-0.000000f, -1.000000f, 0.000000f},
    {0.000000f, 1.000000f, 0.000000f},
};

static const int pentacube_Q_face_verts[] = {2, 3, 4, 12, 12, 4, 7, 6, 7, 4, 3, 5, 5, 3, 2, 9, 8, 13, 6, 7, 5, 13, 10, 11, 1, 0, 8, 9, 0, 1, 11, 10, 0, 10, 13, 8, 1, 9, 2, 12, 6, 11};

static const int pentacube_Q_face_counts[] = {4, 4, 4, 6, 6, 4, 4, 4, 6};

static const float pentacube_R_verts[][3] = {
    {3, 1, 1},
    {3, 2, 1},
    {3, 1, 2},
    {3, 2, 2},
    {2, 1, 2},
    {1, 2, 2},
    {2, 2, 1},
    {2, 1, 0},
    {2, 2, 0},
    {2, 1, 1},
    {0, 1, 0},
    {0, 2, 0},
    {0, 1, 1},
    {0, 2, 1},
    {1, 2, 1},
    {2, 0, 1},
    {1, 0, 1},
    {1, 1, 1},
    {2, 0, 2},
    {1, 0, 2},
};

static const float pentacube_R_normals[][3] = {
    {0.000000f, -0.000000f, -1.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {0.000000f, -1.000000f, -0.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {-1.000000f, -0.000000f, 0.000000f},
    {0.000000f, 0.000000f, 1.000000f},
    {0.000000f, -1.000000f, 0.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {0.000000f, -0.000000f, -1.000000f},
    {0.000000f, -0.000000f, -1.000000f},
    {0.000000f, 1.000000f, 0.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {0.000000f, -1.000000f, -0.000000f},
    {0.000000f, 0.000000f, 1.000000f},
};

static const int pentacube_R_face_verts[] = {0, 9, 6, 1, 2, 0, 1, 3, 9, 0, 2, 4, 9, 7, 8, 6, 10, 12, 13, 11, 12, 17, 14, 13, 17, 12, 10, 7, 9, 17, 16, 19, 5, 14, 15, 16, 17, 9, 7, 10, 11, 8, 6, 8, 11, 13, 14, 5, 3, 1, 18, 15, 9, 4, 16, 15, 18, 19, 3, 5, 19, 18, 4, 2};

static const int pentacube_R_face_counts[] = {4, 4, 4, 4, 4, 4, 5, 5, 4, 4, 8, 4, 4, 6};

static const float pentacube_R_prime_verts[][3] = {
    {0, 1, 1},
    {0, 2, 1},
    {0, 1, 2},
    {0, 2, 2},
    {1, 1, 2},
    {2, 2, 2},
    {1, 2, 1},
    {1, 1, 0},
    {1, 2, 0},
    {1, 1, 1},
    {3, 1, 0},
    {3, 2, 0},
    {3, 1, 1},
    {3, 2, 1},
    {2, 2, 1},
    {1, 0, 1},
    {2, 0, 1},
    {2, 1, 1},
    {1, 0, 2},
    {2, 0, 2},
};

static const float pentacube_R_prime_normals[][3] = {
    {-0.000000f, 0.000000f, -1.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {0.000000f, -1.000000f, 0.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {-0.000000f, 0.000000f, 1.000000f},
    {0.000000f, -1.000000f, 0.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {-0.000000f, 0.000000f, -1.000000f},
    {-0.000000f, 0.000000f, -1.000000f},
    {0.000000f, 1.000000f, 0.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {0.000000f, -1.000000f, 0.000000f},
    {0.000000f, 0.000000f, 1.000000f},
};

static const int pentacube_R_prime_face_verts[] = {0, 1, 6, 9, 2, 3, 1, 0, 9, 4, 2, 0, 9, 6, 8, 7, 10, 11, 13, 12, 12, 13, 14, 17, 17, 9, 7, 10, 12, 17, 14, 5, 19, 16, 15, 9, 17, 16, 7, 8, 11, 10, 6, 1, 3, 5, 14, 13, 11, 8, 18, 4, 9, 15, 16, 19, 18, 15, 3, 2, 4, 18, 19, 5};

static const int pentacube_R_prime_face_counts[] = {4, 4, 4, 4, 4, 4, 5, 5, 4, 4, 8, 4, 4, 6};

static const float pentacube_S_verts[][3] = {
    {0, 2, 2},
    {0, 2, 1},
    {2, 1, 1},
    {3, 1, 2},
    {2, 2, 1},
    {2, 1, 0},
    {2, 2, 0},
    {3, 1, 0},
    {0, 0, 2},
    {1, 0, 2},
    {1, 0, 1},
    {3, 2, 2},
    {3, 2, 0},
    {1, 1, 2},
    {0, 0, 1},
    {1, 1, 1},
};

static const float pentacube_S_normals[][3] = {
    {1.000000f, 0.000000f, 0.000000f},
    {0.000000f, -1.000000f, 0.000000f},
    {0.000000f, -0.000000f, -1.000000f},
    {-1.000000f, -0.000000f, 0.000000f},
    {0.000000f, 0.000000f, 1.000000f},
    {0.000000f, 0.000000f, -1.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {-1.000000f, 0.000000f, -0.000000f},
    {0.000000f, -1.000000f, 0.000000f},
    {0.000000f, 1.000000f, 0.000000f},
};

static const int pentacube_S_face_verts[] = {9, 10, 15, 13, 7, 3, 13, 15, 2, 5, 7, 5, 6, 12, 5, 2, 4, 6, 0, 8, 9, 13, 3, 11, 1, 4, 2, 15, 10, 14, 3, 7, 12, 11, 1, 14, 8, 0, 9, 8, 14, 10, 1, 0, 11, 12, 6, 4};

static const int pentacube_S_face_counts[] = {4, 6, 4, 4, 6, 6, 4, 4, 4, 6};

static const float pentacube_S_prime_verts[][3] = {
    {3, 2, 2},
    {3, 2, 1},
    {1, 1, 1},
    {0, 1, 2},
    {1, 2, 1},
    {1, 1, 0},
    {1, 2, 0},
    {0, 1, 0},
    {3, 0, 2},
    {2, 0, 2},
    {2, 0, 1},
    {0, 2, 2},
    {0, 2, 0},
    {2, 1, 2},
    {3, 0, 1},
    {2, 1, 1},
};

static const float pentacube_S_prime_normals[][3] = {
    {-1.000000f, 0.000000f, 0.000000f},
    {0.000000f, -1.000000f, 0.000000f},
    {-0.000000f, 0.000000f, -1.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {0.000000f, 0.000000f, 1.000000f},
    {0.000000f, 0.000000f, -1.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {1.000000f, -0.000000f, 0.000000f},
    {-0.000000f, -1.000000f, 0.000000f},
    {0.000000f, 1.000000f, 0.000000f},
};

static const int pentacube_S_prime_face_verts[] = {9, 13, 15, 10, 7, 5, 2, 15, 13, 3, 7, 12, 6, 5, 5, 6, 4, 2, 0, 11, 3, 13, 9, 8, 1, 14, 10, 15, 2, 4, 3, 11, 12, 7, 1, 0, 8, 14, 9, 10, 14, 8, 1, 4, 6, 12, 11, 0};

static const int pentacube_S_prime_face_counts[] = {4, 6, 4, 4, 6, 6, 4, 4, 4, 6};

static const float pentacube_T_verts[][3] = {
    {1, 0, 1},
    {1, 0, 0},
    {2, 0, 1},
    {2, 0, 0},
    {1, 1, 1},
    {1, 1, 0},
    {2, 1, 1},
    {2, 1, 0},
    {0, 2, 1},
    {0, 2, 0},
    {1, 2, 1},
    {1, 2, 0},
    {0, 3, 1},
    {0, 3, 0},
    {1, 3, 1},
    {1, 3, 0},
    {2, 3, 1},
    {2, 3, 0},
    {2, 2, 1},
    {2, 2, 0},
    {3, 3, 1},
    {3, 3, 0},
    {3, 2, 1},
    {3, 2, 0},
};

static const float pentacube_T_normals[][3] = {
    {-0.000000f, -1.000000f, 0.000000f},
    {0.000000f, 1.000000f, 0.000000f},
    {0.000000f, 0.000000f, 1.000000f},
    {-0.000000f, -1.000000f, 0.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {-0.000000f, -1.000000f, 0.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {0.000000f, 0.000000f, -1.000000f},
};

static const int pentacube_T_face_verts[] = {0, 1, 3, 2, 15, 13, 12, 14, 16, 20, 21, 17, 2, 6, 18, 22, 20, 16, 14, 12, 8, 10, 4, 0, 8, 9, 11, 10, 12, 13, 9, 8, 18, 19, 23, 22, 22, 23, 21, 20, 5, 1, 0, 4, 10, 11, 2, 3, 7, 19, 18, 6, 7, 3, 1, 5, 11, 9, 13, 15, 17, 21, 23, 19};

static const int pentacube_T_face_counts[] = {4, 8, 12, 4, 4, 4, 4, 6, 6, 12};

static const float pentacube_U_verts[][3] = {
    {0, 0, 1},
    {0, 1, 1},
    {0, 0, 0},
    {0, 1, 0},
    {2, 0, 3},
    {2, 1, 3},
    {0, 0, 3},
    {0, 1, 3},
    {0, 0, 2},
    {0, 1, 2},
    {1, 0, 2},
    {1, 1, 2},
    {2, 0, 0},
    {2, 1, 0},
    {1, 0, 1},
    {1, 1, 1},
};

static const float pentacube_U_normals[][3] = {
    {-1.000000f, 0.000000f, 0.000000f},
    {-0.000000f, 0.000000f, 1.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {-0.000000f, 0.000000f, -1.000000f},
    {1.000000f, -0.000000f, 0.000000f},
    {0.000000f, -1.000000f, 0.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {-0.000000f, 0.000000f, -1.000000f},
    {-0.000000f, 0.000000f, 1.000000f},
    {0.000000f, 1.000000f, 0.000000f},
};

static const int pentacube_U_face_verts[] = {0, 1, 3, 2, 14, 15, 1, 0, 6, 7, 9, 8, 8, 9, 11, 10, 13, 5, 4, 12, 2, 12, 4, 6, 8, 10, 14, 0, 10, 11, 15, 14, 2, 3, 13, 12, 4, 5, 7, 6, 3, 1, 15, 11, 9, 7, 5, 13};

static const int pentacube_U_face_counts[] = {4, 4, 4, 4, 4, 8, 4, 4, 4, 8};

static const float pentacube_V_verts[][3] = {
    {1, 0, 0},
    {1, 1, 0},
    {0, 0, 0},
    {0, 1, 0},
    {3, 0, 3},
    {3, 1, 3},
    {3, 0, 2},
    {3, 1, 2},
    {1, 0, 2},
    {1, 1, 2},
    {0, 0, 3},
    {0, 1, 3},
    {1, 0, 2},
    {1, 1, 2},
    {1, 0, 2},
    {1, 1, 2},
};

static const float pentacube_V_normals[][3] = {
    {-0.000000f, 0.000000f, -1.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {0.000000f, -1.000000f, 0.000000f},
    {1.000000f, -0.000000f, 0.000000f},
    {0.000000f, 0.000000f, -1.000000f},
    {0.000000f, -0.000000f, 1.000000f},
    {-1.000000f, 0.000000f, -0.000000f},
    {0.000000f, 1.000000f, 0.000000f},
};

static const int pentacube_V_face_verts[] = {2, 3, 1, 0, 6, 7, 5, 4, 0, 14, 6, 4, 10, 2, 1, 15, 14, 0, 7, 6, 14, 15, 5, 11, 10, 4, 3, 2, 10, 11, 1, 3, 11, 5, 7, 15};

static const int pentacube_V_face_counts[] = {4, 4, 6, 4, 4, 4, 4, 6};

static const float pentacube_W_verts[][3] = {
    {3, 0, 0},
    {3, 1, 0},
    {2, 0, 0},
    {2, 1, 0},
    {1, 0, 1},
    {1, 1, 1},
    {2, 0, 1},
    {2, 1, 1},
    {1, 1, 2},
    {2, 0, 3},
    {2, 1, 3},
    {0, 0, 3},
    {0, 1, 3},
    {0, 0, 2},
    {0, 1, 2},
    {1, 0, 2},
    {3, 0, 2},
    {3, 1, 2},
    {2, 0, 2},
    {2, 1, 2},
};

static const float pentacube_W_normals[][3] = {
    {-1.000000f, 0.000000f, 0.000000f},
    {-0.000000f, 0.000000f, -1.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {-0.000000f, 0.000000f, -1.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {-0.000000f, 0.000000f, -1.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {-0.000000f, 0.000000f, 1.000000f},
    {0.000000f, -1.000000f, 0.000000f},
    {-0.000000f, 0.000000f, 1.000000f},
    {0.000000f, 1.000000f, 0.000000f},
};

static const int pentacube_W_face_verts[] = {6, 7, 3, 2, 2, 3, 1, 0, 15, 8, 5, 4, 4, 5, 7, 6, 18, 19, 10, 9, 11, 12, 14, 13, 13, 14, 8, 15, 0, 1, 17, 16, 16, 17, 19, 18, 6, 2, 0, 16, 18, 9, 11, 13, 15, 4, 9, 10, 12, 11, 3, 7, 5, 8, 14, 12, 10, 19, 17, 1};

static const int pentacube_W_face_counts[] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 10, 4, 10};

static const float pentacube_X_verts[][3] = {
    {2, 0, 0},
    {2, 1, 0},
    {1, 0, 0},
    {1, 1, 0},
    {2, 0, 2},
    {2, 1, 2},
    {3, 0, 2},
    {3, 1, 2},
    {3, 0, 1},
    {3, 1, 1},
    {1, 0, 3},
    {1, 1, 3},
    {1, 1, 2},
    {2, 0, 3},
    {2, 1, 3},
    {0, 0, 2},
    {0, 1, 2},
    {0, 0, 1},
    {0, 1, 1},
    {1, 0, 2},
    {1, 0, 1},
    {1, 1, 1},
    {2, 0, 1},
    {2, 1, 1},
};

static const float pentacube_X_normals[][3] = {
    {-1.000000f, 0.000000f, 0.000000f},
    {-0.000000f, 0.000000f, -1.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {-0.000000f, 0.000000f, -1.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {-0.000000f, 0.000000f, 1.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {-0.000000f, 0.000000f, 1.000000f},
    {0.000000f, -1.000000f, 0.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {-0.000000f, 0.000000f, -1.000000f},
    {-0.000000f, 0.000000f, 1.000000f},
    {0.000000f, 1.000000f, 0.000000f},
};

static const int pentacube_X_face_verts[] = {20, 21, 3, 2, 2, 3, 1, 0, 0, 1, 23, 22, 22, 23, 9, 8, 8, 9, 7, 6, 6, 7, 5, 4, 10, 11, 12, 19, 4, 5, 14, 13, 13, 14, 11, 10, 0, 22, 8, 6, 4, 13, 10, 19, 15, 17, 20, 2, 15, 16, 18, 17, 17, 18, 21, 20, 19, 12, 16, 15, 3, 21, 18, 16, 12, 11, 14, 5, 7, 9, 23, 1};

static const int pentacube_X_face_counts[] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 12, 4, 4, 4, 12};

static const float pentacube_Y_verts[][3] = {
    {4, 1, 1},
    {4, 1, 0},
    {4, 2, 1},
    {4, 2, 0},
    {1, 0, 1},
    {1, 0, 0},
    {2, 0, 1},
    {2, 0, 0},
    {0, 1, 1},
    {0, 1, 0},
    {1, 1, 1},
    {0, 2, 1},
    {0, 2, 0},
    {1, 1, 0},
    {2, 1, 0},
    {2, 1, 1},
};

static const float pentacube_Y_normals[][3] = {
    {1.000000f, 0.000000f, 0.000000f},
    {-0.000000f, -1.000000f, 0.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {-0.000000f, -1.000000f, 0.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {0.000000f, 0.000000f, -1.000000f},
    {0.000000f, -1.000000f, 0.000000f},
    {-0.000000f, 1.000000f, 0.000000f},
    {0.000000f, 0.000000f, 1.000000f},
};

static const int pentacube_Y_face_verts[] = {0, 1, 3, 2, 4, 5, 7, 6, 6, 7, 14, 15, 10, 13, 5, 4, 8, 9, 13, 10, 11, 12, 9, 8, 1, 14, 7, 5, 13, 9, 12, 3, 1, 0, 15, 14, 2, 3, 12, 11, 0, 2, 11, 8, 10, 4, 6, 15};

static const int pentacube_Y_face_counts[] = {4, 4, 4, 4, 4, 4, 8, 4, 4, 8};

static const float pentacube_Z_verts[][3] = {
    {2, 0, 3},
    {2, 1, 3},
    {2, 0, 2},
    {3, 0, 3},
    {3, 1, 3},
    {2, 1, 2},
    {3, 0, 1},
    {3, 1, 1},
    {0, 0, 2},
    {0, 1, 2},
    {0, 0, 0},
    {0, 1, 0},
    {1, 0, 1},
    {1, 1, 1},
    {1, 0, 0},
    {1, 1, 0},
};

static const float pentacube_Z_normals[][3] = {
    {-1.000000f, 0.000000f, 0.000000f},
    {-0.000000f, 0.000000f, 1.000000f},
    {-1.000000f, 0.000000f, 0.000000f},
    {0.000000f, -1.000000f, 0.000000f},
    {-0.000000f, 0.000000f, 1.000000f},
    {-0.000000f, 0.000000f, -1.000000f},
    {1.000000f, 0.000000f, 0.000000f},
    {1.000000f, -0.000000f, 0.000000f},
    {0.000000f, 0.000000f, -1.000000f},
    {0.000000f, 1.000000f, 0.000000f},
};

static const int pentacube_Z_face_verts[] = {0, 1, 5, 2, 3, 4, 1, 0, 8, 9, 11, 10, 6, 3, 0, 2, 8, 10, 14, 12, 2, 5, 9, 8, 10, 11, 15, 14, 14, 15, 13, 12, 7, 4, 3, 6, 7, 6, 12, 13, 5, 1, 4, 7, 13, 15, 11, 9};

static const int pentacube_Z_face_counts[] = {4, 4, 4, 8, 4, 4, 4, 4, 4, 8};

const pentacube_data_t pentacube_data[PENTACUBE_COUNT] = {
    {
        .name = "A",
        .vertices = (const float *)pentacube_A_verts,
        .face_normals = (const float *)pentacube_A_normals,
        .face_vertices = pentacube_A_face_verts,
        .face_vertex_counts = pentacube_A_face_counts,
        .vertex_count = 20,
        .face_count = 12
    },
    {
        .name = "B",
        .vertices = (const float *)pentacube_B_verts,
        .face_normals = (const float *)pentacube_B_normals,
        .face_vertices = pentacube_B_face_verts,
        .face_vertex_counts = pentacube_B_face_counts,
        .vertex_count = 18,
        .face_count = 12
    },
    {
        .name = "E",
        .vertices = (const float *)pentacube_E_verts,
        .face_normals = (const float *)pentacube_E_normals,
        .face_vertices = pentacube_E_face_verts,
        .face_vertex_counts = pentacube_E_face_counts,
        .vertex_count = 19,
        .face_count = 12
    },
    {
        .name = "E'",
        .vertices = (const float *)pentacube_E_prime_verts,
        .face_normals = (const float *)pentacube_E_prime_normals,
        .face_vertices = pentacube_E_prime_face_verts,
        .face_vertex_counts = pentacube_E_prime_face_counts,
        .vertex_count = 19,
        .face_count = 12
    },
    {
        .name = "F",
        .vertices = (const float *)pentacube_F_verts,
        .face_normals = (const float *)pentacube_F_normals,
        .face_vertices = pentacube_F_face_verts,
        .face_vertex_counts = pentacube_F_face_counts,
        .vertex_count = 20,
        .face_count = 12
    },
    {
        .name = "G",
        .vertices = (const float *)pentacube_G_verts,
        .face_normals = (const float *)pentacube_G_normals,
        .face_vertices = pentacube_G_face_verts,
        .face_vertex_counts = pentacube_G_face_counts,
        .vertex_count = 19,
        .face_count = 12
    },
    {
        .name = "G'",
        .vertices = (const float *)pentacube_G_prime_verts,
        .face_normals = (const float *)pentacube_G_prime_normals,
        .face_vertices = pentacube_G_prime_face_verts,
        .face_vertex_counts = pentacube_G_prime_face_counts,
        .vertex_count = 19,
        .face_count = 12
    },
    {
        .name = "H",
        .vertices = (const float *)pentacube_H_verts,
        .face_normals = (const float *)pentacube_H_normals,
        .face_vertices = pentacube_H_face_verts,
        .face_vertex_counts = pentacube_H_face_counts,
        .vertex_count = 19,
        .face_count = 12
    },
    {
        .name = "H'",
        .vertices = (const float *)pentacube_H_prime_verts,
        .face_normals = (const float *)pentacube_H_prime_normals,
        .face_vertices = pentacube_H_prime_face_verts,
        .face_vertex_counts = pentacube_H_prime_face_counts,
        .vertex_count = 19,
        .face_count = 12
    },
    {
        .name = "I",
        .vertices = (const float *)pentacube_I_verts,
        .face_normals = (const float *)pentacube_I_normals,
        .face_vertices = pentacube_I_face_verts,
        .face_vertex_counts = pentacube_I_face_counts,
        .vertex_count = 8,
        .face_count = 6
    },
    {
        .name = "J",
        .vertices = (const float *)pentacube_J_verts,
        .face_normals = (const float *)pentacube_J_normals,
        .face_vertices = pentacube_J_face_verts,
        .face_vertex_counts = pentacube_J_face_counts,
        .vertex_count = 19,
        .face_count = 10
    },
    {
        .name = "J'",
        .vertices = (const float *)pentacube_J_prime_verts,
        .face_normals = (const float *)pentacube_J_prime_normals,
        .face_vertices = pentacube_J_prime_face_verts,
        .face_vertex_counts = pentacube_J_prime_face_counts,
        .vertex_count = 19,
        .face_count = 10
    },
    {
        .name = "K",
        .vertices = (const float *)pentacube_K_verts,
        .face_normals = (const float *)pentacube_K_normals,
        .face_vertices = pentacube_K_face_verts,
        .face_vertex_counts = pentacube_K_face_counts,
        .vertex_count = 24,
        .face_count = 12
    },
    {
        .name = "L",
        .vertices = (const float *)pentacube_L_verts,
        .face_normals = (const float *)pentacube_L_normals,
        .face_vertices = pentacube_L_face_verts,
        .face_vertex_counts = pentacube_L_face_counts,
        .vertex_count = 12,
        .face_count = 8
    },
    {
        .name = "M",
        .vertices = (const float *)pentacube_M_verts,
        .face_normals = (const float *)pentacube_M_normals,
        .face_vertices = pentacube_M_face_verts,
        .face_vertex_counts = pentacube_M_face_counts,
        .vertex_count = 24,
        .face_count = 16
    },
    {
        .name = "N",
        .vertices = (const float *)pentacube_N_verts,
        .face_normals = (const float *)pentacube_N_normals,
        .face_vertices = pentacube_N_face_verts,
        .face_vertex_counts = pentacube_N_face_counts,
        .vertex_count = 16,
        .face_count = 10
    },
    {
        .name = "P",
        .vertices = (const float *)pentacube_P_verts,
        .face_normals = (const float *)pentacube_P_normals,
        .face_vertices = pentacube_P_face_verts,
        .face_vertex_counts = pentacube_P_face_counts,
        .vertex_count = 12,
        .face_count = 8
    },
    {
        .name = "Q",
        .vertices = (const float *)pentacube_Q_verts,
        .face_normals = (const float *)pentacube_Q_normals,
        .face_vertices = pentacube_Q_face_verts,
        .face_vertex_counts = pentacube_Q_face_counts,
        .vertex_count = 14,
        .face_count = 9
    },
    {
        .name = "R",
        .vertices = (const float *)pentacube_R_verts,
        .face_normals = (const float *)pentacube_R_normals,
        .face_vertices = pentacube_R_face_verts,
        .face_vertex_counts = pentacube_R_face_counts,
        .vertex_count = 20,
        .face_count = 14
    },
    {
        .name = "R'",
        .vertices = (const float *)pentacube_R_prime_verts,
        .face_normals = (const float *)pentacube_R_prime_normals,
        .face_vertices = pentacube_R_prime_face_verts,
        .face_vertex_counts = pentacube_R_prime_face_counts,
        .vertex_count = 20,
        .face_count = 14
    },
    {
        .name = "S",
        .vertices = (const float *)pentacube_S_verts,
        .face_normals = (const float *)pentacube_S_normals,
        .face_vertices = pentacube_S_face_verts,
        .face_vertex_counts = pentacube_S_face_counts,
        .vertex_count = 16,
        .face_count = 10
    },
    {
        .name = "S'",
        .vertices = (const float *)pentacube_S_prime_verts,
        .face_normals = (const float *)pentacube_S_prime_normals,
        .face_vertices = pentacube_S_prime_face_verts,
        .face_vertex_counts = pentacube_S_prime_face_counts,
        .vertex_count = 16,
        .face_count = 10
    },
    {
        .name = "T",
        .vertices = (const float *)pentacube_T_verts,
        .face_normals = (const float *)pentacube_T_normals,
        .face_vertices = pentacube_T_face_verts,
        .face_vertex_counts = pentacube_T_face_counts,
        .vertex_count = 24,
        .face_count = 10
    },
    {
        .name = "U",
        .vertices = (const float *)pentacube_U_verts,
        .face_normals = (const float *)pentacube_U_normals,
        .face_vertices = pentacube_U_face_verts,
        .face_vertex_counts = pentacube_U_face_counts,
        .vertex_count = 16,
        .face_count = 10
    },
    {
        .name = "V",
        .vertices = (const float *)pentacube_V_verts,
        .face_normals = (const float *)pentacube_V_normals,
        .face_vertices = pentacube_V_face_verts,
        .face_vertex_counts = pentacube_V_face_counts,
        .vertex_count = 16,
        .face_count = 8
    },
    {
        .name = "W",
        .vertices = (const float *)pentacube_W_verts,
        .face_normals = (const float *)pentacube_W_normals,
        .face_vertices = pentacube_W_face_verts,
        .face_vertex_counts = pentacube_W_face_counts,
        .vertex_count = 20,
        .face_count = 12
    },
    {
        .name = "X",
        .vertices = (const float *)pentacube_X_verts,
        .face_normals = (const float *)pentacube_X_normals,
        .face_vertices = pentacube_X_face_verts,
        .face_vertex_counts = pentacube_X_face_counts,
        .vertex_count = 24,
        .face_count = 14
    },
    {
        .name = "Y",
        .vertices = (const float *)pentacube_Y_verts,
        .face_normals = (const float *)pentacube_Y_normals,
        .face_vertices = pentacube_Y_face_verts,
        .face_vertex_counts = pentacube_Y_face_counts,
        .vertex_count = 16,
        .face_count = 10
    },
    {
        .name = "Z",
        .vertices = (const float *)pentacube_Z_verts,
        .face_normals = (const float *)pentacube_Z_normals,
        .face_vertices = pentacube_Z_face_verts,
        .face_vertex_counts = pentacube_Z_face_counts,
        .vertex_count = 16,
        .face_count = 10
    },
};
