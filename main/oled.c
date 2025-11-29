/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "lvgl.h"
#include "pentacubes.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define BASE_SIZE 8
#define CENTER_X 50
#define CENTER_Y 20
#define FRAME_COUNT 120
#define ANIMATION_DURATION 6000
#define PERSPECTIVE_DISTANCE 800
#define MAX_EDGE_SET 256

static lv_obj_t *canvas;
static lv_color_t *canvas_buf;
static lv_timer_t *animation_timer;
static int current_frame = 0;
static lv_obj_t *label_name;

static float shape_angle_x = 0.0f;
static float shape_angle_y = 0.0f;
static float shape_angle_z = 0.0f;
static float rotation_speed_x = 0.0f;  // Angular velocity per frame
static float rotation_speed_y = 0.0f;
static float rotation_speed_z = 0.0f;
static float start_x = 0.0f;
static float start_y = 0.0f;
static float target_x = 0.0f;
static float target_y = 0.0f;
static float current_x = 0.0f;
static float current_y = 0.0f;
static const float z_position = 0.0f;

static int current_pentacube = 0;
static int next_pentacube = 1;
static int shape_instance_count = 0;
static float travel_progress = 0.0f;
static int last_logged_pentacube = -1;

typedef struct { int v1, v2; } edge_t;

static int simple_rand(void)
{
    static int seed = 12345;
    seed = (seed * 1103515245 + 12345) & 0x7fffffff;
    return seed;
}

static int random_pentacube(void) { return simple_rand() % PENTACUBE_COUNT; }

static float randf(float min, float max) {
    return min + (max - min) * (simple_rand() % 1000) / 1000.0f;
}

static void init_new_shape(void) {
    current_pentacube = next_pentacube;
    next_pentacube = random_pentacube();
    shape_instance_count++;
    
    // Random initial rotation
    shape_angle_x = randf(0.0f, 2.0f * M_PI);
    shape_angle_y = randf(0.0f, 2.0f * M_PI);
    shape_angle_z = randf(0.0f, 2.0f * M_PI);
    
    // Random rotation speeds - INCREASED for visibility
    rotation_speed_x = randf(0.01f, 0.03f);
    rotation_speed_y = randf(0.01f, 0.03f);
    rotation_speed_z = randf(0.01f, 0.03f);
    
    // Ensure at least one axis has meaningful rotation
    if (fabsf(rotation_speed_x) < 0.01f && 
        fabsf(rotation_speed_y) < 0.01f && 
        fabsf(rotation_speed_z) < 0.01f) {
        rotation_speed_y = 0.03f; // Force visible rotation
    }
    
    // Spawn position: odd = left field, even = right field
    if (shape_instance_count % 2 == 1) {
        // Left field: X in [0, 50]
        start_x = randf(-25.0f, 0.0f);  // Offset from CENTER_X=50
        target_x = 50.0f;  // Move toward right edge (100 - 50 = 50 offset)
    } else {
        // Right field: X in [50, 100]
        start_x = randf(0.0f, 25.0f);
        target_x = -50.0f; // Move toward left edge (0 - 50 = -50 offset)
    }
    
    // Random Y position
    start_y = randf(-15.0f, 15.0f);
    target_y = start_y; // Straight horizontal travel
    
    current_x = start_x;
    current_y = start_y;
    travel_progress = 0.0f;
    
    last_logged_pentacube = -1;
}

static void project_point(float x, float y, float z, int *screen_x, int *screen_y)
{
    float perspective_z = z + z_position + PERSPECTIVE_DISTANCE;
    if (perspective_z <= 0.1f) perspective_z = 0.1f;
    float scale = PERSPECTIVE_DISTANCE / perspective_z;
    *screen_x = CENTER_X + (int)(current_x + x * BASE_SIZE * scale);
    *screen_y = CENTER_Y + (int)(current_y + y * BASE_SIZE * scale);
}

static void rotate_point(float *x, float *y, float *z, float angle_x, float angle_y, float angle_z)
{
    float cos_x = cosf(angle_x), sin_x = sinf(angle_x);
    float cos_y = cosf(angle_y), sin_y = sinf(angle_y);
    float cos_z = cosf(angle_z), sin_z = sinf(angle_z);

    float temp_y = *y * cos_x - *z * sin_x;
    float temp_z = *y * sin_x + *z * cos_x;
    *y = temp_y; *z = temp_z;

    float temp_x = *x * cos_y + *z * sin_y;
    *z = -*x * sin_y + *z * cos_y;
    *x = temp_x;

    temp_x = *x * cos_z - *y * sin_z;
    temp_y = *x * sin_z + *y * cos_z;
    *x = temp_x; *y = temp_y;
}

static bool add_edge_to_set(edge_t *edge_set, int *edge_count, int v1, int v2)
{
    if (*edge_count >= MAX_EDGE_SET) return false;
    if (v1 > v2) { int temp = v1; v1 = v2; v2 = temp; }
    for (int i = 0; i < *edge_count; i++)
        if (edge_set[i].v1 == v1 && edge_set[i].v2 == v2) return true;
    edge_set[*edge_count].v1 = v1;
    edge_set[*edge_count].v2 = v2;
    (*edge_count)++;
    return true;
}

static void update_animation_state(void)
{
    // Update rotation angles FIRST (continuous tumbling)
    shape_angle_x += rotation_speed_x;
    shape_angle_y += rotation_speed_y;
    shape_angle_z += rotation_speed_z;
    
    // Wrap angles to [0, 2π] to prevent overflow
    while (shape_angle_x > 2.0f * M_PI) shape_angle_x -= 2.0f * M_PI;
    while (shape_angle_y > 2.0f * M_PI) shape_angle_y -= 2.0f * M_PI;
    while (shape_angle_z > 2.0f * M_PI) shape_angle_z -= 2.0f * M_PI;
    while (shape_angle_x < 0.0f) shape_angle_x += 2.0f * M_PI;
    while (shape_angle_y < 0.0f) shape_angle_y += 2.0f * M_PI;
    while (shape_angle_z < 0.0f) shape_angle_z += 2.0f * M_PI;
    
    // Update position
    travel_progress += 1.0f / FRAME_COUNT;
    current_x = start_x + (target_x - start_x) * travel_progress;
    current_y = start_y + (target_y - start_y) * travel_progress;
    
    // Reset when reached edge
    if (travel_progress >= 1.0f) {
        init_new_shape();
    }
}

static void draw_pentacube_frame(void)
{
    lv_canvas_fill_bg(canvas, lv_color_white(), LV_OPA_COVER);
    update_animation_state();

    const pentacube_data_t *pentacube = &pentacube_data[current_pentacube];
    lv_label_set_text(label_name, pentacube->name);
    
    // Fade in at start, fade out at end
    float fade = 1.0f;
    if (travel_progress < 0.1f) {
        fade = travel_progress / 0.1f;
    } else if (travel_progress > 0.9f) {
        fade = (1.0f - travel_progress) / 0.1f;
    }
    lv_opa_t opacity = (lv_opa_t)(LV_OPA_COVER * fade);

    const float (*verts)[3] = (const float (*)[3])pentacube->vertices;
    const float (*normals)[3] = (const float (*)[3])pentacube->face_normals;

    float *rotated_verts = lv_mem_alloc(pentacube->vertex_count * 3 * sizeof(float));
    float *rotated_normals = lv_mem_alloc(pentacube->face_count * 3 * sizeof(float));
    int *screen_coords = lv_mem_alloc(pentacube->vertex_count * 2 * sizeof(int));

    // Rotate vertices (using persisted angles)
    for (int i = 0; i < pentacube->vertex_count; i++) {
        float vx = verts[i][0] - 1.5f;
        float vy = verts[i][1] - 1.5f;
        float vz = verts[i][2] - 1.5f;
        rotate_point(&vx, &vy, &vz, shape_angle_x, shape_angle_y, shape_angle_z);
        rotated_verts[i * 3 + 0] = vx;
        rotated_verts[i * 3 + 1] = vy;
        rotated_verts[i * 3 + 2] = vz;
        project_point(vx, vy, vz, &screen_coords[i * 2], &screen_coords[i * 2 + 1]);
    }

    // Rotate face normals
    for (int i = 0; i < pentacube->face_count; i++) {
        float nx = normals[i][0];
        float ny = normals[i][1];
        float nz = normals[i][2];
        rotate_point(&nx, &ny, &nz, shape_angle_x, shape_angle_y, shape_angle_z);
        rotated_normals[i * 3 + 0] = nx;
        rotated_normals[i * 3 + 1] = ny;
        rotated_normals[i * 3 + 2] = nz;
    }

    edge_t *visible_edges = lv_mem_alloc(MAX_EDGE_SET * sizeof(edge_t));
    int visible_edge_count = 0;
    int face_vert_idx = 0;
    int visible_faces = 0;
    
    // Find most front-facing face
    int most_frontal_face = -1;
    float max_frontal_dot = -1.0f;

    // First pass: find visible faces and most frontal
    for (int face_idx = 0; face_idx < pentacube->face_count; face_idx++) {
        float nz = rotated_normals[face_idx * 3 + 2];

        if (nz > 0.0f) {
            visible_faces++;
            if (nz > max_frontal_dot) {
                max_frontal_dot = nz;
                most_frontal_face = face_idx;
            }
            
            int face_vert_count = pentacube->face_vertex_counts[face_idx];
            for (int i = 0; i < face_vert_count; i++) {
                int va = pentacube->face_vertices[face_vert_idx + i];
                int vb = pentacube->face_vertices[face_vert_idx + (i + 1) % face_vert_count];
                add_edge_to_set(visible_edges, &visible_edge_count, va, vb);
            }
        }
        face_vert_idx += pentacube->face_vertex_counts[face_idx];
    }

    // Fill the most frontal face (checkerboard dither for monochrome)
    if (most_frontal_face >= 0) {
        int fill_face_start = 0;
        for (int i = 0; i < most_frontal_face; i++) {
            fill_face_start += pentacube->face_vertex_counts[i];
        }
        
        int fill_vert_count = pentacube->face_vertex_counts[most_frontal_face];
        lv_point_t *poly_points = lv_mem_alloc(fill_vert_count * sizeof(lv_point_t));
        
        for (int i = 0; i < fill_vert_count; i++) {
            int v_idx = pentacube->face_vertices[fill_face_start + i];
            poly_points[i].x = screen_coords[v_idx * 2];
            poly_points[i].y = screen_coords[v_idx * 2 + 1];
        }
        
        lv_draw_line_dsc_t fill_line_dsc;
        lv_draw_line_dsc_init(&fill_line_dsc);
        fill_line_dsc.color = lv_color_black();
        fill_line_dsc.width = 1;
        fill_line_dsc.opa = opacity;
        
        // Scanline fill with checkerboard
        int min_y = poly_points[0].y, max_y = poly_points[0].y;
        for (int i = 1; i < fill_vert_count; i++) {
            if (poly_points[i].y < min_y) min_y = poly_points[i].y;
            if (poly_points[i].y > max_y) max_y = poly_points[i].y;
        }
        
        for (int y = min_y; y <= max_y; y++) {
            int x_intersect[2] = {10000, -10000};
            int intersect_count = 0;
            
            for (int i = 0; i < fill_vert_count && intersect_count < 2; i++) {
                int i_next = (i + 1) % fill_vert_count;
                int y1 = poly_points[i].y;
                int y2 = poly_points[i_next].y;
                
                if ((y1 <= y && y < y2) || (y2 <= y && y < y1)) {
                    int x1 = poly_points[i].x;
                    int x2 = poly_points[i_next].x;
                    int x = x1 + (y - y1) * (x2 - x1) / (y2 - y1);
                    
                    if (intersect_count == 0) {
                        x_intersect[0] = x;
                        intersect_count++;
                    } else if (x != x_intersect[0]) {
                        x_intersect[1] = x;
                        intersect_count++;
                    }
                }
            }
            
            if (intersect_count == 2) {
                int x_min = (x_intersect[0] < x_intersect[1]) ? x_intersect[0] : x_intersect[1];
                int x_max = (x_intersect[0] > x_intersect[1]) ? x_intersect[0] : x_intersect[1];
                
                // Checkerboard dither
                for (int x = x_min; x <= x_max; x++) {
                    if ((x + y) % 2 == 0) {
                        lv_point_t pt[2] = {{x, y}, {x, y}};
                        lv_canvas_draw_line(canvas, pt, 2, &fill_line_dsc);
                    }
                }
            }
        }
        
        lv_mem_free(poly_points);
    }

    // Draw edges
    for (int i = 0; i < visible_edge_count; i++) {
        int v1_idx = visible_edges[i].v1;
        int v2_idx = visible_edges[i].v2;
        lv_draw_line_dsc_t line_dsc;
        lv_draw_line_dsc_init(&line_dsc);
        line_dsc.color = lv_color_black();
        line_dsc.width = 1;
        line_dsc.opa = opacity;
        lv_point_t points[2] = {
            {screen_coords[v1_idx * 2], screen_coords[v1_idx * 2 + 1]},
            {screen_coords[v2_idx * 2], screen_coords[v2_idx * 2 + 1]}
        };
        lv_canvas_draw_line(canvas, points, 2, &line_dsc);
    }

    lv_mem_free(rotated_verts);
    lv_mem_free(rotated_normals);
    lv_mem_free(screen_coords);
    lv_mem_free(visible_edges);

    if (travel_progress > 0.5f && travel_progress < 0.6f && current_pentacube != last_logged_pentacube) {
        printf("PENTACUBE: %-8s | Instance: %d | Spawn: %s | Visible: %d/%d\n",
               pentacube->name, shape_instance_count,
               (shape_instance_count % 2 == 1) ? "LEFT" : "RIGHT",
               visible_faces, pentacube->face_count);
        last_logged_pentacube = current_pentacube;
    }
}

static void animation_timer_cb(lv_timer_t *timer) {
    (void)timer;
    draw_pentacube_frame();
    current_frame = (current_frame + 1) % FRAME_COUNT;
}

void lvgl_ui(lv_disp_t *disp)
{
    lv_obj_t *scr = lv_disp_get_scr_act(disp);
    canvas = lv_canvas_create(scr);
    canvas_buf = lv_mem_alloc(disp->driver->hor_res * disp->driver->ver_res * sizeof(lv_color_t));
    lv_canvas_set_buffer(canvas, canvas_buf, disp->driver->hor_res, disp->driver->ver_res, LV_IMG_CF_TRUE_COLOR);
    lv_obj_align(canvas, LV_ALIGN_CENTER, 0, 0);

    label_name = lv_label_create(scr);
    lv_label_set_text(label_name, "");
    lv_obj_set_style_text_color(label_name, lv_color_black(), 0);
    lv_obj_align(label_name, LV_ALIGN_TOP_LEFT, 1, 0);

    animation_timer = lv_timer_create(animation_timer_cb, ANIMATION_DURATION / FRAME_COUNT, NULL);
    
    init_new_shape();
    draw_pentacube_frame();
}