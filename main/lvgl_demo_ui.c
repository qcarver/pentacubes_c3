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

#define BASE_SIZE 8          // Back to 8 for visibility
#define CENTER_X 50          // Center of 100px width
#define CENTER_Y 20          // Center of 40px height
#define FRAME_COUNT 32
#define ANIMATION_DURATION 2000
#define PERSPECTIVE_DISTANCE 800  // Reduced for smaller display

static lv_obj_t *canvas;
static lv_color_t *canvas_buf;
static lv_timer_t *animation_timer;
static int current_frame = 0;
static lv_obj_t *label_name;  // Label for shape name

// Animation state
static float z_position = 0.0f;
static float x_offset = 0.0f;
static float x_direction = 1.0f;
static int direction_change_counter = 0;

// Rotation axes (0=X, 1=Y, 2=Z)
static int rotation_axes[3] = {1, 0, 0};
static int axis_change_counter = 0;

// Current and next pentacubes
static int current_pentacube = 0;
static int next_pentacube = 1;
static float morphing_progress = 0.0f;
static bool is_morphing = false;
static int last_logged_pentacube = -1;

static int simple_rand(void)
{
    static int seed = 12345;
    seed = (seed * 1103515245 + 12345) & 0x7fffffff;
    return seed;
}

static int random_pentacube(void)
{
    return simple_rand() % PENTACUBE_COUNT;
}

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
    float cos_x = cosf(angle_x);
    float sin_x = sinf(angle_x);
    float cos_y = cosf(angle_y);
    float sin_y = sinf(angle_y);
    float cos_z = cosf(angle_z);
    float sin_z = sinf(angle_z);
    
    // Rotate around X
    float temp_y = *y * cos_x - *z * sin_x;
    float temp_z = *y * sin_x + *z * cos_x;
    *y = temp_y;
    *z = temp_z;
    
    // Rotate around Y
    float temp_x = *x * cos_y + *z * sin_y;
    *z = -*x * sin_y + *z * cos_y;
    *x = temp_x;
    
    // Rotate around Z
    temp_x = *x * cos_z - *y * sin_z;
    temp_y = *x * sin_z + *y * cos_z;
    *x = temp_x;
    *y = temp_y;
}

static void update_animation_state(void)
{
    // Update X offset (side-to-side movement) - reduced range for small display
    direction_change_counter++;
    if (direction_change_counter > 8) {
        if (simple_rand() % 3 == 0) {
            x_direction = -x_direction;
        }
        direction_change_counter = 0;
    }
    
    x_offset += x_direction * 0.6f;  // Slower movement
    
    if (x_offset > 15.0f) {  // Reduced range
        x_offset = 15.0f;
        x_direction = -1.0f;
    } else if (x_offset < -15.0f) {
        x_offset = -15.0f;
        x_direction = 1.0f;
    }
    
    // Calculate Z position based on distance from center
    float norm_x = fabsf(x_offset) / 15.0f;
    float distance_from_center = norm_x * norm_x;
    z_position = distance_from_center * 200.0f;  // Reduced Z movement
    
    // Update morphing progress
    if (z_position > 180.0f && !is_morphing) {
        is_morphing = true;
        morphing_progress = 0.0f;
        current_pentacube = next_pentacube;
        next_pentacube = random_pentacube();
        last_logged_pentacube = -1;
    }
    
    if (is_morphing) {
        morphing_progress += 0.05f;
        if (morphing_progress >= 1.0f) {
            is_morphing = false;
            morphing_progress = 1.0f;
        }
    }
    
    // Randomly change rotation axes
    axis_change_counter++;
    if (axis_change_counter > 16) {
        rotation_axes[0] = simple_rand() % 2;
        rotation_axes[1] = simple_rand() % 2;
        rotation_axes[2] = simple_rand() % 2;
        axis_change_counter = 0;
    }
}

static void draw_pentacube_frame(void)
{
    // Clear canvas to WHITE
    lv_canvas_fill_bg(canvas, lv_color_white(), LV_OPA_COVER);
    
    update_animation_state();
    
    // Calculate rotation angles
    float angle = (float)current_frame * 2.0f * M_PI / FRAME_COUNT;
    float angle_x = angle * rotation_axes[0];
    float angle_y = angle * rotation_axes[1];
    float angle_z = angle * rotation_axes[2];
    
    // Get current pentacube data
    const pentacube_data_t *pentacube = &pentacube_data[current_pentacube];
    
    // Update label with shape name
    lv_label_set_text(label_name, pentacube->name);
    
    // Calculate opacity for morphing
    lv_opa_t opacity = (lv_opa_t)(LV_OPA_COVER * morphing_progress);
    
    // Cast vertices pointer correctly as array of 3-float arrays
    const float (*verts)[3] = (const float (*)[3])pentacube->vertices;
    
    int lines_drawn = 0;
    
    // Draw all edges directly
    for (int edge_idx = 0; edge_idx < pentacube->edge_count; edge_idx++) {
        int v1_idx = pentacube->edges[edge_idx * 2];
        int v2_idx = pentacube->edges[edge_idx * 2 + 1];
        
        // Bounds check to prevent crashes
        if (v1_idx >= pentacube->vertex_count || v2_idx >= pentacube->vertex_count) {
            printf("ERROR: Invalid vertex index! edge=%d v1=%d v2=%d max=%d shape=%s\n",
                   edge_idx, v1_idx, v2_idx, pentacube->vertex_count, pentacube->name);
            continue;
        }
        
        // Get vertex coordinates - access as 2D array
        float v1_x = verts[v1_idx][0];
        float v1_y = verts[v1_idx][1];
        float v1_z = verts[v1_idx][2];
        
        float v2_x = verts[v2_idx][0];
        float v2_y = verts[v2_idx][1];
        float v2_z = verts[v2_idx][2];
        
        // Center the pentacube
        v1_x -= 1.5f;
        v1_y -= 1.5f;
        v1_z -= 1.5f;
        v2_x -= 1.5f;
        v2_y -= 1.5f;
        v2_z -= 1.5f;
        
        // Rotate vertices
        rotate_point(&v1_x, &v1_y, &v1_z, angle_x, angle_y, angle_z);
        rotate_point(&v2_x, &v2_y, &v2_z, angle_x, angle_y, angle_z);
        
        // Project to screen
        int x1, y1, x2, y2;
        project_point(v1_x, v1_y, v1_z, &x1, &y1);
        project_point(v2_x, v2_y, v2_z, &x2, &y2);
        
        // Draw the edge
        lv_draw_line_dsc_t line_dsc;
        lv_draw_line_dsc_init(&line_dsc);
        line_dsc.color = lv_color_black();
        line_dsc.width = 1;
        line_dsc.opa = opacity;
        
        lv_point_t points[2] = {{x1, y1}, {x2, y2}};
        lv_canvas_draw_line(canvas, points, 2, &line_dsc);
        lines_drawn++;
    }
    
    // Log when a new pentacube is displayed (only once per shape change)
    if (morphing_progress > 0.9f && current_pentacube != last_logged_pentacube) {
        printf("PENTACUBE: %-8s | Edges: %2d | Drawn: %2d\n", 
               pentacube->name, 
               pentacube->edge_count, 
               lines_drawn);
        last_logged_pentacube = current_pentacube;
    }
}

static void animation_timer_cb(lv_timer_t *timer)
{
    (void)timer;
    draw_pentacube_frame();
    current_frame = (current_frame + 1) % FRAME_COUNT;
}

// Calculate center of mass for pentacube
static float get_pentacube_center_x(int pentacube_idx)
{
    const pentacube_data_t *p = &pentacube_data[pentacube_idx];
    const float (*verts)[3] = (const float (*)[3])p->vertices;
    float sum_x = 0.0f;
    int vertex_count = p->vertex_count;
    
    for (int i = 0; i < vertex_count; i++) {
        sum_x += verts[i][0];
    }
    
    return sum_x / vertex_count;
}

void example_lvgl_demo_ui(lv_disp_t *disp)
{
    lv_obj_t *scr = lv_disp_get_scr_act(disp);
    
    canvas = lv_canvas_create(scr);
    
    canvas_buf = lv_mem_alloc(disp->driver->hor_res * disp->driver->ver_res * sizeof(lv_color_t));
    lv_canvas_set_buffer(canvas, canvas_buf, disp->driver->hor_res, disp->driver->ver_res, LV_IMG_CF_TRUE_COLOR);
    
    lv_obj_align(canvas, LV_ALIGN_CENTER, 0, 0);
    
    // Create label for shape name (top-left corner)
    label_name = lv_label_create(scr);
    lv_label_set_text(label_name, "");
    lv_obj_set_style_text_color(label_name, lv_color_black(), 0);
    lv_obj_set_style_text_font(label_name, &lv_font_montserrat_14, 0);
    lv_obj_align(label_name, LV_ALIGN_TOP_LEFT, 1, 0);
    
    animation_timer = lv_timer_create(animation_timer_cb, ANIMATION_DURATION / FRAME_COUNT, NULL);
    
    // Randomize initial pentacube
    current_pentacube = random_pentacube();
    next_pentacube = random_pentacube();
    
    // Center the initial pentacube
    x_offset = -get_pentacube_center_x(current_pentacube) * BASE_SIZE;
    
    printf("\n=== PENTACUBE WIREFRAME (100x40) ===\n");
    
    draw_pentacube_frame();
}