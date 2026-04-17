#ifndef MING_RENDERER_H
#define MING_RENDERER_H

#include <cassert>

#include "defs.h"
#include "vec.h"

struct Canvas {
  rgba *pixels;
  usize width, height;
};

struct Model {
  v3f *vertices;
  v3i *faces;
  usize n_vertices;
  usize n_faces;
};

struct Transform {
  v3f pos;
  v3f rot; // 'rot.y' == rotation about the 'Y' axis == 'yaw'
  float scale;
};

Model load_model(const char *obj_file, v3f *vertex_buf, usize max_vertices,
                 v3i *face_buf, usize max_faces);

void set_pixel(Canvas c, usize x, usize y, rgba color);
void fill_bg(Canvas c, rgba color);
void draw_line(Canvas c, int x0, int y0, int x1, int y1, rgba color);
void draw_triangle(Canvas c, v2i p0, v2i p1, v2i p2, rgba color);

void render_model(Canvas c, Model m, Transform t);

#endif
