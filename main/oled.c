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
    pentacube_render_frame(canvas, label_name);
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
    
    pentacube_render_init();
    pentacube_render_frame(canvas, label_name);
}