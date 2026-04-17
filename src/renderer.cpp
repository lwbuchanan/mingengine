#include <cassert>
#include <cstdlib>
#include <utility>

#include "defs.h"
#include "renderer.h"
#include "vec.h"

Canvas init_canvas(rgba *pixels, usize width, usize height) {
  return Canvas{pixels, width, height};
}

void set_pixel(Canvas c, usize x, usize y, rgba color) {
  // assert(x < c.width && y < c.height);
  if (x >= c.width || y >= c.height)
    return;
  c.pixels[x + y * c.width] = color;
}

void fill_bg(Canvas c, rgba color) {
  for (usize y = 0; y < c.height; y++) {
    for (usize x = 0; x < c.width; x++) {
      set_pixel(c, x, y, color);
    }
  }
}

void draw_line(Canvas c, int x0, int y0, int x1, int y1, rgba color) {
  bool steep = abs(x1 - x0) < abs(y1 - y0);
  if (steep) {
    std::swap(x0, y0);
    std::swap(x1, y1);
  }

  if (x0 > x1) {
    std::swap(x0, x1);
    std::swap(y0, y1);
  };

  float y = y0;
  for (int x = x0; x < x1; x++) {
    if (steep) {
      set_pixel(c, y, x, color);
    } else {
      set_pixel(c, x, y, color);
    }
    y += (y1 - y0) / (float)(x1 - x0);
  }
}

void draw_traingle_wires(Canvas c, int x0, int y0, int x1, int y1, int x2,
                         int y2, rgba color) {
  draw_line(c, x0, y0, x1, y1, color);
  draw_line(c, x1, y1, x2, y2, color);
  draw_line(c, x2, y2, x0, y0, color);
}

void draw_quad_wires(Canvas c, int x0, int y0, int x1, int y1, int x2, int y2,
                     int x3, int y3, rgba color) {
  draw_line(c, x0, y0, x1, y1, color);
  draw_line(c, x1, y1, x2, y2, color);
  draw_line(c, x2, y2, x3, y3, color);
  draw_line(c, x3, y3, x0, y0, color);
}

v2i to_screen(Canvas c, v3f v) {
  float screen_height = 5.0f;
  int px_per_unit = c.height / screen_height;
  v2f proj_vertex = v2f{v.x() / v.z(), v.y() / v.z()};

  int x = (c.width / 2) + (int)(proj_vertex.x() * px_per_unit);
  int y = c.height - ((c.height / 2) + (int)(proj_vertex.y() * px_per_unit));
  return v2i{x, y};
}

void render_model(Canvas c, Model m, Transform t) {
  for (usize f = 0; f < m.n_faces; f++) {
    v3i face = m.faces[f];
    v2i point_a = to_screen(c, m.vertices[face[0]]);
    v2i point_b = to_screen(c, m.vertices[face[1]]);
    v2i point_c = to_screen(c, m.vertices[face[2]]);

    draw_traingle_wires(c, point_a.x(), point_a.y(), point_b.x(), point_b.y(),
                        point_c.x(), point_c.y(), 0xFFFFFFFF);
  }
}
