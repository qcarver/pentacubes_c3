/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "lvgl.h"
#include "pentacube.h"

#define ANIMATION_DURATION 6000
#define FRAME_COUNT 120

static lv_obj_t *canvas;
static lv_color_t *canvas_buf;
static lv_timer_t *animation_timer;
static lv_obj_t *label_name;
static int current_frame = 0;

static void animation_timer_cb(lv_timer_t *timer) {
    (void)timer;

    // Create static instance - only constructed on first call
    static Pentacube pentacube(canvas, label_name);

    pentacube.pentacube_frame(canvas, label_name);
    current_frame = (current_frame + 1) % FRAME_COUNT;
}

// Add C linkage for the function called from main.c
extern "C" {
    void oled(lv_disp_t *disp);
}

void oled(lv_disp_t *disp)
{
    lv_obj_t *scr = lv_disp_get_scr_act(disp);
    
    // Make screen background white (inverted)
    lv_obj_set_style_bg_color(scr, lv_color_white(), 0);
    
    canvas = lv_canvas_create(scr);
    
    // Use TRUE_COLOR format
    size_t buf_size = disp->driver->hor_res * disp->driver->ver_res * sizeof(lv_color_t);
    canvas_buf = static_cast<lv_color_t*>(lv_mem_alloc(buf_size));
    
    lv_canvas_set_buffer(canvas, canvas_buf, disp->driver->hor_res, disp->driver->ver_res, LV_IMG_CF_TRUE_COLOR);
    
    // Fill canvas with white background (inverted - this will be "off" pixels)
    lv_canvas_fill_bg(canvas, lv_color_white(), LV_OPA_COVER);
    
    lv_obj_align(canvas, LV_ALIGN_CENTER, 0, 0);

    label_name = lv_label_create(scr);
    lv_label_set_text(label_name, "Test");  // Start with visible text
    lv_obj_set_style_text_color(label_name, lv_color_black(), 0);  // Black text (inverted - this will be "on" pixels)
    lv_obj_align(label_name, LV_ALIGN_TOP_LEFT, 1, 0);

    // Add debug: draw a test line to verify canvas is working
    lv_draw_line_dsc_t test_line;
    lv_draw_line_dsc_init(&test_line);
    test_line.color = lv_color_black();  // Black line (inverted - this will be "on" pixels)
    test_line.width = 2;
    lv_point_t test_points[2] = {{10, 10}, {50, 50}};
    lv_canvas_draw_line(canvas, test_points, 2, &test_line);

    animation_timer = lv_timer_create(animation_timer_cb, ANIMATION_DURATION / FRAME_COUNT, NULL);
}