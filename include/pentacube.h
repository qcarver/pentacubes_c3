#ifndef PENTACUBE_H
#define PENTACUBE_H

#include "lvgl.h"

class Pentacube {
  public:
    // Constructor - initializes and renders first frame
    Pentacube(lv_obj_t *canvas, lv_obj_t *label_name);

    // Public methods
    void pentacube_init();
    void pentacube_frame(lv_obj_t *canvas, lv_obj_t *label_name);

  private:
    // Edge structure - must be defined BEFORE use in method signatures
    struct edge_t { int v1, v2; };
    
    // Private methods
    void project_point(float x, float y, float z, lv_coord_t *screen_x, lv_coord_t *screen_y);
    void rotate_point(float *x, float *y, float *z, float angle_x, float angle_y, float angle_z);
    bool edge_add_to_set(edge_t *edge_set, int *edge_count, int v1, int v2);
    void anim_update();

    // Private member variables (state)
    float shape_angle_x;
    float shape_angle_y;
    float shape_angle_z;
    float rotation_speed_x;
    float rotation_speed_y;
    float rotation_speed_z;
    float start_x;
    float start_y;
    float target_x;
    float target_y;
    float current_x;
    float current_y;
    float z_position;

    int current_pentacube;
    int next_pentacube;
    int shape_instance_count;
    float travel_progress;
    int last_logged_pentacube;

    // Static arrays for rendering
    float rotated_verts[32 * 3];    // MAX_VERTICES * 3
    float rotated_normals[24 * 3];  // MAX_FACES * 3
    lv_coord_t screen_coords[64];   // MAX_SCREEN_COORDS - changed from int

    // Edge array
    edge_t visible_edges[256];      // MAX_EDGE_SET
};

#endif // PENTACUBE_H