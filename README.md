| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C6 | ESP32-H2 | ESP32-P4 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- | -------- | -------- | -------- |

# OTA

This project will update over-the-air by default. You can change that.

# Build WITH OTA support (default)
cd /home/qcarver/Dev/ota/pentacubes_c3
idf.py build

# Build WITHOUT OTA support
idf.py -DENABLE_OTA=OFF build

# Running OTA server
requires python3
go to the `serve_me` directory
if needs be `chmod a+x update_and_start.sh`
`./update_and_start.sh`
To let the server 'know' there is a new version: change PROJECT_VER in the root cmake.

# Pentacubes C3 - 3D Wireframe Visualization on ESP32-C3

A real-time 3D wireframe renderer for all 29 pentacubes, running on an ESP32-C3 microcontroller with a 128x40 OLED display.

```
    ╱─────╲
   ╱ ■ ■ ■ ╲
  ╱ ■     ■ ╲
 ╱ ■ ■ ■ ■ ■ ╲
 ╲           ╱
  ╲─────────╱
```

## Overview

This project combines Blender 3D modeling, Python preprocessing, and embedded C graphics to create an interactive pentacube viewer. Each of the 29 distinct pentacube shapes (5-cube combinations) rotates continuously while being culled and rendered as clean wireframe drawings on a tiny OLED display.

**Inspiration:** This project was inspired by [Sicherman Dice and Pentacubes](https://sicherman.net/c5l2/)

## Workflow

### 1. Blender Modeling (`meta/All29PentacubesMinPts.blend`)

- 29 pentacube shapes are modeled in Blender
- Each shape is a separate object with:
  - Individual cube unit vertices
  - Faces connecting groups of vertices
  - Named appropriately (A, B, E, E', F, etc.)

### 2. Python Export (`meta/run_in_blender.py`)

Run this script from **inside Blender's text editor**:

```
Text Editor → run_in_blender.py → Alt+P (or Run Script)
```

The script performs three passes:

**PASS 1: Export from Blender**
- Extracts vertices, edges, and faces from all 29 pentacube objects
- Preserves Blender's face normal data
- Creates metadata about each shape

**PASS 2: Validate & Fix Winding**
- Uses **Newell's method** to compute accurate face normals
- Ensures all faces have consistent [counter-clockwise (CCW) winding](https://en.wikipedia.org/wiki/Polygon#Orientation) when viewed from outside
- Adjusts vertex coordinates relative to object origins
- Shifts all coordinates to positive space for efficient rendering
- Reports which faces needed winding corrections

**PASS 3: Generate C Code**
- Produces fully optimized `main/pentacubes.c`
- Generates vertex arrays, edge lists, and face definitions
- Pre-computes edge counts and face vertex counts
- Creates the `pentacube_data` struct array ready for runtime

**Output:** `main/pentacubes.c` (auto-generated, don't edit)

### 3. Rendering (`main/lvgl_demo_ui.c`)

The rendering pipeline has three key stages:

#### Face Culling
- Computes screen-space normal for each face using cross product of first 3 vertices
- Tests `cross > 0` to determine visibility (front-facing = positive cross product)
- **Eliminates edge-on faces** (cross = 0) to prevent "yarn" artifacts
- Only front-facing faces contribute edges to the wireframe

#### Edge Deduplication
- Collects edges from all visible faces into a **set data structure**
- Removes duplicate edges automatically (important since edges are shared between faces)
- Result: Clean wireframe with no double-drawn edges

#### Rendering & Morphing
- **Rotation:** Shape rotates on X/Y/Z axes smoothly
- **Translation:** Drifts left-right (X direction) with automatic reversal
- **Morphing:** Smoothly transitions between pentacubes with opacity control
- **Animation:** Frame-by-frame with configurable duration

```c
// Key rendering parameters
#define FRAME_COUNT 32          // Frames per rotation
#define ANIMATION_DURATION 2000 // ms per pentacube
#define PERSPECTIVE_DISTANCE 800 // Camera Z distance
```

## Data Structure

### Vertex Data
```c
static const float vertices_X[][3] = {
    {1.0f, 0.0f, 1.0f},  // [x, y, z]
    {2.0f, 1.0f, 2.0f},
    // ...
};
```

### Face Definition
```c
static const int face_vertices_X[] = {
    // Face 0: 4 vertices
    0, 1, 3, 2,
    // Face 1: 4 vertices
    4, 5, 7, 6,
    // Face 2: 6 vertices (non-rectangular)
    8, 9, 10, 11, 12, 13,
    // ...
};

static const int face_vertex_counts_X[] = {
    4, 4, 6, 4, 4,  // Vertex count for each face
    // ...
};
```

### Pentacube Metadata
```c
const pentacube_data_t pentacube_data[PENTACUBE_COUNT] = {
    {
        .vertices = (const float *)vertices_A,
        .vertex_count = 20,
        .edges = edges_A,
        .edge_count = 30,
        .face_vertices = face_vertices_A,
        .face_vertex_counts = face_vertex_counts_A,
        .face_count = 12,
        .name = "A"
    },
    // ... 28 more pentacubes
};
```

## Building & Flashing

### Prerequisites
```bash
# Install ESP-IDF
export IDF_PATH=/path/to/esp-idf
source $IDF_PATH/export.sh

# Install dependencies
pip install idf-env
```

### Build
```bash
cd /home/qcarver/Dev/pentacubes_c3
idf.py build
```

### Flash
```bash
idf.py -p /dev/ttyACM0 flash
```

### Monitor
```bash
idf.py -p /dev/ttyACM0 monitor
```

## Troubleshooting

### Device Won't Program

**Symptom:** `ERRCODE: 0x104` or timeout during flash  
**Solution:** This is a known ESP32-C3 issue on first programming:

1. **Hold BOOT button** on the device
2. Pulse the **RESET button** while holding BOOT
3. Release BOOT when flashing begins
4. Subsequent flashes should work normally

This is a hardware quirk documented in various ESP32-C3 communities - the bootloader sometimes needs manual intervention on the very first write.

### Display Not Showing

- Check SSD1315 I2C connection (GPIO 8 & 9 on ESP32-C3)
- Verify LVGL is initialized (check serial output)
- Confirm pull-up resistors on I2C lines (typically 4.7kΩ)

### Glitchy Rendering / Reboots

- Ensure `DEBUG_CULLING` is set to 0 in `lvgl_demo_ui.c` (reduces overhead)
- Check heap size isn't exhausted (monitor output shows heap info)
- Verify pentacubes.c was regenerated from latest Blender script

### Wireframe Has Missing Edges

- Regenerate `pentacubes.c` from Blender script
- Check winding correction report in Python output
- Verify face_vertex_counts array matches actual face sizes

### OTA Update Spins Forever

**Symptom:** Device repeatedly detects an update, downloads OTA image, reboots, then repeats.

**Cause:** `APP_VERSION` was changed before running `serve_me/update_and_start.sh`, but the firmware binary was not rebuilt first. The server advertises the new version in `version.json`, while the served `.bin` still contains the old version string.

**Fix:** Always update in this order:

1. Change `APP_VERSION` in `CMakeLists.txt`
2. Run `idf.py build` (this embeds the new version into the binary)
3. Then run `serve_me/update_and_start.sh` (this writes `version.json` and serves the new `.bin`)

## Key Statistics

| Metric | Value |
|--------|-------|
| Total Pentacubes | 29 |
| Total Vertices | ~490 |
| Total Edges | ~750 |
| Total Faces | ~290 |
| Display Resolution | 128 × 40 pixels |
| Rotation Speed | 32 frames / 2 seconds |
| Morph Duration | 2 seconds |

## Files & Directories

```
pentacubes_c3/
├── meta/
│   ├── All29PentacubesMinPts.blend    # Blender model
│   ├── run_in_blender.py              # Export script (run inside Blender)
│   └── README.md                      # This file
├── main/
│   ├── pentacubes.c                   # AUTO-GENERATED - don't edit
│   ├── pentacubes.h                   # Data structure definitions
│   ├── lvgl_demo_ui.c                 # Rendering & animation
│   └── main.c                         # ESP-IDF entry point
├── CMakeLists.txt
└── idf_component.yml
```

## Technical Highlights

### Efficient Face Culling
- O(1) per-face visibility test using cross product
- Handles non-planar (6+ vertex) faces correctly
- Eliminates numerical precision issues by checking for `cross = 0`

### Memory Optimization
- Face data stored as flat index arrays (no pointers)
- Pre-computed face vertex counts eliminate runtime loops
- Edges deduplicated at export time, not runtime

### Smooth Animation
- Linear interpolation for rotation
- Opacity-based morphing between shapes
- Configurable animation timings

### Display Efficiency
- LVGL canvas rendering with alpha blending
- Minimal memory footprint (fits in ESP32-C3 RAM)
- Runs at 32 FPS with headroom for additional features

## Future Enhancements

- [ ] Interactive rotation control via button input
- [ ] Color coding for different pentacube families
- [ ] Configurable rotation speed/axes
- [ ] Wireframe vs. solid shading modes
- [ ] Larger display support (e.g., 240×240)

## References

- **Pentacubes Inspiration:** [Sicherman Dice & Pentacubes](https://sicherman.net/c5l2/)
- **ESP32-C3 Docs:** [Espressif ESP32-C3 Reference](https://www.espressif.com/en/products/socs/esp32-c3)
- **LVGL Graphics:** [LVGL Documentation](https://docs.lvgl.io/)
- **Polygon Winding:** [Wikipedia - Polygon Orientation](https://en.wikipedia.org/wiki/Polygon#Orientation)

## License

This project is provided as-is for educational and creative purposes.

---

**Last Updated:** November 2025  
**Author:** Q Carver  
**Platform:** ESP32-C3 with SSD1315 OLED Display
