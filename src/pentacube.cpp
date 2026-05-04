#include "pentacube.h"
#include "pentacubes.h"

#ifdef OTA
#include "ota_helper.h"
#endif

#include <cmath>
#include <cstdlib>
#include <cstdio>

#include "esp_log.h"

#define BASE_SIZE 8
#define CENTER_X 50
#define CENTER_Y 20
#define FRAME_COUNT 120
#define PERSPECTIVE_DISTANCE 800
#define MAX_EDGE_SET 256
#define MAX_VERTICES 32
#define MAX_FACES 24
#define MAX_SCREEN_COORDS (MAX_VERTICES * 2)

// Helper functions (outside class scope)
static int rand_seed = 12345;

static int rand_next() {
    rand_seed = (rand_seed * 1103515245 + 12345) & 0x7fffffff;
    return rand_seed;
}

static int rand_pentacube() {
    return rand_next() % PENTACUBE_COUNT;
}

static float rand_float(float min, float max) {
    return min + (max - min) * (rand_next() % 1000) / 1000.0f;
}

// Constructor
Pentacube::Pentacube(lv_obj_t *canvas, lv_obj_t *label_name)
    : shape_angle_x(0.0f), shape_angle_y(0.0f), shape_angle_z(0.0f),
      rotation_speed_x(0.0f), rotation_speed_y(0.0f), rotation_speed_z(0.0f),
      start_x(0.0f), start_y(0.0f), target_x(0.0f), target_y(0.0f),
      current_x(0.0f), current_y(0.0f), z_position(0.0f),
      current_pentacube(0), next_pentacube(1), shape_instance_count(0),
      travel_progress(0.0f), last_logged_pentacube(-1)
{
    pentacube_init();
    pentacube_frame(canvas, label_name);
}

void Pentacube::pentacube_init() {
    current_pentacube = next_pentacube;
    next_pentacube = rand_pentacube();
    shape_instance_count++;
    
    shape_angle_x = rand_float(0.0f, 2.0f * M_PI);
    shape_angle_y = rand_float(0.0f, 2.0f * M_PI);
    shape_angle_z = rand_float(0.0f, 2.0f * M_PI);
    
    rotation_speed_x = rand_float(0.01f, 0.03f);
    rotation_speed_y = rand_float(0.01f, 0.03f);
    rotation_speed_z = rand_float(0.01f, 0.03f);
    
    if (std::abs(rotation_speed_x) < 0.01f && 
        std::abs(rotation_speed_y) < 0.01f && 
        std::abs(rotation_speed_z) < 0.01f) {
        rotation_speed_y = 0.03f;
    }
    
    if (shape_instance_count % 2 == 1) {
        start_x = rand_float(-25.0f, 0.0f);
        target_x = 50.0f;
    } else {
        start_x = rand_float(0.0f, 25.0f);
        target_x = -50.0f;
    }
    
    start_y = rand_float(-15.0f, 15.0f);
    target_y = start_y;
    
    current_x = start_x;
    current_y = start_y;
    travel_progress = 0.0f;
    
    last_logged_pentacube = -1;
}

void Pentacube::project_point(float x, float y, float z, lv_coord_t *screen_x, lv_coord_t *screen_y) {
    float perspective_z = z + z_position + PERSPECTIVE_DISTANCE;
    if (perspective_z <= 0.1f) perspective_z = 0.1f;
    float scale = PERSPECTIVE_DISTANCE / perspective_z;
    *screen_x = static_cast<lv_coord_t>(CENTER_X + static_cast<int>(current_x + x * BASE_SIZE * scale));
    *screen_y = static_cast<lv_coord_t>(CENTER_Y + static_cast<int>(current_y + y * BASE_SIZE * scale));
}

void Pentacube::rotate_point(float *x, float *y, float *z, float angle_x, float angle_y, float angle_z) {
    float cos_x = std::cos(angle_x), sin_x = std::sin(angle_x);
    float cos_y = std::cos(angle_y), sin_y = std::sin(angle_y);
    float cos_z = std::cos(angle_z), sin_z = std::sin(angle_z);

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

bool Pentacube::edge_add_to_set(edge_t *edge_set, int *edge_count, int v1, int v2) {
    if (*edge_count >= MAX_EDGE_SET) return false;
    if (v1 > v2) std::swap(v1, v2);
    for (int i = 0; i < *edge_count; i++)
        if (edge_set[i].v1 == v1 && edge_set[i].v2 == v2) return true;
    edge_set[*edge_count].v1 = v1;
    edge_set[*edge_count].v2 = v2;
    (*edge_count)++;
    return true;
}

void Pentacube::anim_update() {
    shape_angle_x += rotation_speed_x;
    shape_angle_y += rotation_speed_y;
    shape_angle_z += rotation_speed_z;
    
    while (shape_angle_x > 2.0f * M_PI) shape_angle_x -= 2.0f * M_PI;
    while (shape_angle_y > 2.0f * M_PI) shape_angle_y -= 2.0f * M_PI;
    while (shape_angle_z > 2.0f * M_PI) shape_angle_z -= 2.0f * M_PI;
    while (shape_angle_x < 0.0f) shape_angle_x += 2.0f * M_PI;
    while (shape_angle_y < 0.0f) shape_angle_y += 2.0f * M_PI;
    while (shape_angle_z < 0.0f) shape_angle_z += 2.0f * M_PI;
    
    travel_progress += 1.0f / FRAME_COUNT;
    current_x = start_x + (target_x - start_x) * travel_progress;
    current_y = start_y + (target_y - start_y) * travel_progress;
    
    if (travel_progress >= 1.0f) {
        pentacube_init();
    }
}

void Pentacube::pentacube_frame(lv_obj_t *canvas, lv_obj_t *label_name) {
    lv_canvas_fill_bg(canvas, lv_color_black(), LV_OPA_COVER);
    anim_update();

    const pentacube_data_t *pentacube = &pentacube_data[current_pentacube];
    lv_label_set_text(label_name, pentacube->name);
    
    const float (*verts)[3] = reinterpret_cast<const float (*)[3]>(pentacube->vertices);
    const float (*normals)[3] = reinterpret_cast<const float (*)[3]>(pentacube->face_normals);

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

    for (int i = 0; i < pentacube->face_count; i++) {
        float nx = normals[i][0];
        float ny = normals[i][1];
        float nz = normals[i][2];
        rotate_point(&nx, &ny, &nz, shape_angle_x, shape_angle_y, shape_angle_z);
        rotated_normals[i * 3 + 0] = nx;
        rotated_normals[i * 3 + 1] = ny;
        rotated_normals[i * 3 + 2] = nz;
    }

    int visible_edge_count = 0;
    int face_vert_idx = 0;
    int visible_faces = 0;
    

    for (int face_idx = 0; face_idx < pentacube->face_count; face_idx++) {
        float nz = rotated_normals[face_idx * 3 + 2];

        if (nz > 0.0f) {
            visible_faces++;
            
            int face_vert_count = pentacube->face_vertex_counts[face_idx];
            for (int i = 0; i < face_vert_count; i++) {
                int va = pentacube->face_vertices[face_vert_idx + i];
                int vb = pentacube->face_vertices[face_vert_idx + (i + 1) % face_vert_count];
                edge_add_to_set(visible_edges, &visible_edge_count, va, vb);
            }
        }
        face_vert_idx += pentacube->face_vertex_counts[face_idx];
    }

    for (int i = 0; i < visible_edge_count; i++) {
        int v1_idx = visible_edges[i].v1;
        int v2_idx = visible_edges[i].v2;
        lv_draw_line_dsc_t line_dsc;
        lv_draw_line_dsc_init(&line_dsc);
        line_dsc.color = lv_color_white();
        line_dsc.width = 1;
        lv_point_t points[2] = {
            {static_cast<lv_coord_t>(screen_coords[v1_idx * 2]), static_cast<lv_coord_t>(screen_coords[v1_idx * 2 + 1])},
            {static_cast<lv_coord_t>(screen_coords[v2_idx * 2]), static_cast<lv_coord_t>(screen_coords[v2_idx * 2 + 1])}
        };
        lv_canvas_draw_line(canvas, points, 2, &line_dsc);
    }
    
    static bool once = true;
    if (once) {
#ifdef OTA
        // Initialize OTA helper - sets next boot to factory
        ota_helper_init();
        
        // Print version info
        char sha[65];
        ota_helper_get_sha256(sha);
        std::printf("pentacube_c3 version %s (%.8s...)\n", 
                    ota_helper_get_version(), sha);
#endif
        once = false;
    }
    
    if (travel_progress > 0.5f && travel_progress < 0.6f && current_pentacube != last_logged_pentacube) {
        ESP_LOGI("pentacube", "PENTACUBE #%d: %-4s | Spawn: %s | Visible: %d/%d",
                 shape_instance_count, pentacube->name,
                 (shape_instance_count % 2 == 1) ? "LEFT" : "RIGHT",
                 visible_faces, pentacube->face_count);
        last_logged_pentacube = current_pentacube;
    }
}