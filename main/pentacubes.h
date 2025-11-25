#ifndef PENTACUBES_H
#define PENTACUBES_H

#define PENTACUBE_COUNT 29

typedef struct {
    const char *name;
    const float *vertices;
    const float *face_normals;
    const int *face_vertices;
    const int *face_vertex_counts;
    int vertex_count;
    int face_count;
} pentacube_data_t;

extern const pentacube_data_t pentacube_data[PENTACUBE_COUNT];

#endif