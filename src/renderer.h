#ifndef MING_RENDERER_H
#define MING_RENDERER_H

#include <cassert>

#include "defs.h"
#include "math.h"

struct Viewport {
  rgba *pixels;
  float64 *depths;
  usize width, height;
};

struct Model {
  v4f *vertices;
  v3i *faces;
  usize n_vertices;
  usize n_faces;
};

struct Transform {
  v3f pos;
  v3f rot; // 'rot.y' == rotation about the 'Y' axis == 'yaw'
  float64 scale;
};

Model load_model(const char *obj_file, v4f *vertex_buf, usize max_vertices,
                 v3i *face_buf, usize max_faces);

// void set_pixel(Viewport canvas, usize x, usize y, rgba color);
// void draw_line(Viewport vp, int x0, int y0, int x1, int y1, rgba color);
// void draw_triangle(Viewport vp, v3i a, v3i b, v3i c, rgba color);

void fill_bg(Viewport vp, rgba color);
void render_model(Viewport vp, Model m, Transform t);

#endif
