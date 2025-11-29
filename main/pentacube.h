#ifndef PENTACUBE_H_RENDER
#define PENTACUBE_H_RENDER

#include "lvgl.h"

void pentacube_render_init(void);
void pentacube_render_frame(lv_obj_t *canvas, lv_obj_t *label_name);

#endif // PENTACUBE_H_RENDER