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
#define FRAME_COUNT 32
#define ANIMATION_DURATION 2000
#define PERSPECTIVE_DISTANCE 800
#define MAX_EDGE_SET 256

static lv_obj_t *canvas;
static lv_color_t *canvas_buf;
static lv_timer_t *animation_timer;
static int current_frame = 0;
static lv_obj_t *label_name;

static float z_position = 0.0f;
static float x_offset = 0.0f;
static float x_direction = 1.0f;
static int direction_change_counter = 0;

static int current_pentacube = 0;
static int next_pentacube = 1;
static float morphing_progress = 0.0f;
static bool is_morphing = false;
static int last_logged_pentacube = -1;

typedef struct { int v1, v2; } edge_t;

static int simple_rand(void)
{
    static int seed = 12345;
    seed = (seed * 1103515245 + 12345) & 0x7fffffff;
    return seed;
}

static int random_pentacube(void) { return simple_rand() % PENTACUBE_COUNT; }

static void project_point(float x, float y, float z, int *screen_x, int *screen_y)
{
    float perspective_z = z + z_position + PERSPECTIVE_DISTANCE;
    if (perspective_z <= 0.1f) perspective_z = 0.1f;
    float scale = PERSPECTIVE_DISTANCE / perspective_z;
    *screen_x = CENTER_X + (int)(x_offset + x * BASE_SIZE * scale);
    *screen_y = CENTER_Y + (int)(y * BASE_SIZE * scale);
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
    direction_change_counter++;
    if (direction_change_counter > 8) {
        if (simple_rand() % 3 == 0) x_direction = -x_direction;
        direction_change_counter = 0;
    }
    x_offset += x_direction * 0.6f;
    if (x_offset > 15.0f) { x_offset = 15.0f; x_direction = -1.0f; }
    else if (x_offset < -15.0f) { x_offset = -15.0f; x_direction = 1.0f; }

    float norm_x = fabsf(x_offset) / 15.0f;
    z_position = norm_x * norm_x * 200.0f;

    if (z_position > 180.0f && !is_morphing) {
        is_morphing = true;
        morphing_progress = 0.0f;
        current_pentacube = next_pentacube;
        next_pentacube = random_pentacube();
        last_logged_pentacube = -1;
    }
    if (is_morphing) {
        morphing_progress += 0.05f;
        if (morphing_progress >= 1.0f) { is_morphing = false; morphing_progress = 1.0f; }
    }
}

static void draw_pentacube_frame(void)
{
    lv_canvas_fill_bg(canvas, lv_color_white(), LV_OPA_COVER);
    update_animation_state();

    float angle = (float)current_frame * 2.0f * M_PI / FRAME_COUNT;
    float angle_x = angle * 0.7f + angle * 0.3f * sinf(angle * 0.5f);
    float angle_y = angle * 1.0f + angle * 0.2f * cosf(angle * 0.7f);
    float angle_z = angle * 0.5f + angle * 0.25f * sinf(angle * 0.3f);

    const pentacube_data_t *pentacube = &pentacube_data[current_pentacube];
    lv_label_set_text(label_name, pentacube->name);
    lv_opa_t opacity = (lv_opa_t)(LV_OPA_COVER * morphing_progress);

    const float (*verts)[3] = (const float (*)[3])pentacube->vertices;
    const float (*normals)[3] = (const float (*)[3])pentacube->face_normals;

    float *rotated_verts = lv_mem_alloc(pentacube->vertex_count * 3 * sizeof(float));
    float *rotated_normals = lv_mem_alloc(pentacube->face_count * 3 * sizeof(float));
    int *screen_coords = lv_mem_alloc(pentacube->vertex_count * 2 * sizeof(int));

    // Rotate vertices
    for (int i = 0; i < pentacube->vertex_count; i++) {
        float vx = verts[i][0] - 1.5f;
        float vy = verts[i][1] - 1.5f;
        float vz = verts[i][2] - 1.5f;
        rotate_point(&vx, &vy, &vz, angle_x, angle_y, angle_z);
        rotated_verts[i * 3 + 0] = vx;
        rotated_verts[i * 3 + 1] = vy;
        rotated_verts[i * 3 + 2] = vz;
        project_point(vx, vy, vz, &screen_coords[i * 2], &screen_coords[i * 2 + 1]);
    }

    // Rotate face normals (same rotation as vertices)
    for (int i = 0; i < pentacube->face_count; i++) {
        float nx = normals[i][0];
        float ny = normals[i][1];
        float nz = normals[i][2];
        rotate_point(&nx, &ny, &nz, angle_x, angle_y, angle_z);
        rotated_normals[i * 3 + 0] = nx;
        rotated_normals[i * 3 + 1] = ny;
        rotated_normals[i * 3 + 2] = nz;
    }

    edge_t *visible_edges = lv_mem_alloc(MAX_EDGE_SET * sizeof(edge_t));
    int visible_edge_count = 0;
    int face_vert_idx = 0;
    int visible_faces = 0;

    // Backface culling using dot product with view direction (0, 0, 1) in camera space
    for (int face_idx = 0; face_idx < pentacube->face_count; face_idx++) {
        float nz = rotated_normals[face_idx * 3 + 2];

        // View direction is (0, 0, 1) after rotation; dot(normal, view) = nz
        // Face visible if normal points toward camera (nz > 0)
        if (nz > 0.0f) {
            visible_faces++;
            int face_vert_count = pentacube->face_vertex_counts[face_idx];
            for (int i = 0; i < face_vert_count; i++) {
                int va = pentacube->face_vertices[face_vert_idx + i];
                int vb = pentacube->face_vertices[face_vert_idx + (i + 1) % face_vert_count];
                add_edge_to_set(visible_edges, &visible_edge_count, va, vb);
            }
        }
        face_vert_idx += pentacube->face_vertex_counts[face_idx];
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

    if (morphing_progress > 0.9f && current_pentacube != last_logged_pentacube) {
        printf("PENTACUBE: %-8s | Visible: %d/%d faces\n",
               pentacube->name, visible_faces, pentacube->face_count);
        last_logged_pentacube = current_pentacube;
    }
}

static void animation_timer_cb(lv_timer_t *timer) {
    (void)timer;
    draw_pentacube_frame();
    current_frame = (current_frame + 1) % FRAME_COUNT;
}

void example_lvgl_demo_ui(lv_disp_t *disp)
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
    current_pentacube = random_pentacube();
    next_pentacube = random_pentacube();
    draw_pentacube_frame();
}