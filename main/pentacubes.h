#ifndef PENTACUBES_H
#define PENTACUBES_H

#define PENTACUBE_COUNT 29

typedef struct {
    const float *vertices;      // Pointer to vertex array (x,y,z triplets)
    int vertex_count;            // Number of vertices
    const int *edges;            // Pointer to edge array (vertex index pairs)
    int edge_count;              // Number of edges (total ints / 2)
    const int *face_vertices;    // Pointer to face vertex indices
    const int *face_vertex_counts; // Pointer to array of vertex counts per face
    int face_count;              // Number of faces
    const char *name;            // Name of the pentacube
} pentacube_data_t;

extern const pentacube_data_t pentacube_data[PENTACUBE_COUNT];

#endif // PENTACUBES_H