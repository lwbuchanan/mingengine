#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <utility>

#include "defs.h"
#include "renderer.h"
#include "vec.h"
#include "ming_print.h"

void set_pixel(Canvas c, usize x, usize y, rgba color) {
  // assert(x < c.width && y < c.height);
  if (x >= c.width || y >= c.height)
    return;
  c.pixels[x + (c.height - y - 1) * c.width] = color;
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

  int y = y0;
  int error = 0;
  for (int x = x0; x < x1; x++) {
    if (steep) {
      set_pixel(c, y, x, color);
    } else {
      set_pixel(c, x, y, color);
    }
    error += 2 * abs(y1 - y0);
    y += (y1 > y0 ? 1 : -1) * (error > x1 - x0);
    error -= 2 * (x1 - x0) * (error > x1 - x0);
  }
}

void draw_triangle_scanline(Canvas c, int x0, int y0, int x1, int y1, int x2,
                            int y2, rgba color) {
  if (y0 > y1) {
    std::swap(y0, y1);
    std::swap(x0, x1);
  }
  if (y0 > y2) {
    std::swap(y0, y2);
    std::swap(x0, x2);
  }
  if (y1 > y2) {
    std::swap(y1, y2);
    std::swap(x1, x2);
  }

  int height = y2 - y0;
  if (y1 != y0) {
    int bottom_height = y1 - y0;
    for (int y = y0; y <= y1; y++) {
      int xa = x0 + ((x2 - x0) * (y - y0)) / height;
      int xb = x0 + ((x1 - x0) * (y - y0)) / bottom_height;
      set_pixel(c, xa, y, color);
      set_pixel(c, xb, y, color);

      if (xa > xb)
        std::swap(xa, xb);
      for (int x = xa; x < xb; x++) {
        set_pixel(c, x, y, color);
      }
    }
  }
  if (y2 != y1) {
    int top_height = y2 - y1;
    for (int y = y1; y <= y2; y++) {
      int xa = x0 + ((x2 - x0) * (y - y0)) / height;
      int xb = x1 + ((x2 - x1) * (y - y1)) / top_height;
      set_pixel(c, xa, y, color);
      set_pixel(c, xb, y, color);

      if (xa > xb)
        std::swap(xa, xb);
      for (int x = xa; x < xb; x++) {
        set_pixel(c, x, y, color);
      }
    }
  }
}

float signed_triangle_area(v2i p0, v2i p1, v2i p2) {
  return 0.5 * ((p1.y() - p0.y()) * (p1.x() + p0.x()) +
                (p2.y() - p1.y()) * (p2.x() + p1.x()) +
                (p0.y() - p2.y()) * (p0.x() + p2.x()));
}

void draw_triangle_area(Canvas c, v2i p0, v2i p1, v2i p2, rgba color) {
  int xmin = std::min(std::min(p0.x(), p1.x()), p2.x());
  int xmax = std::max(std::max(p0.x(), p1.x()), p2.x());
  int ymin = std::min(std::min(p0.y(), p1.y()), p2.y());
  int ymax = std::max(std::max(p0.y(), p1.y()), p2.y());
  int triangle_area = signed_triangle_area(p0, p1, p2);

  // #pragma omp parallel for
  for (int x = xmin; x <= xmax; x++) {
    for (int y = ymin; y <= ymax; y++) {
      float alpha = signed_triangle_area({x, y}, p1, p2) / triangle_area;
      float beta = signed_triangle_area({x, y}, p2, p0) / triangle_area;
      float gamma = signed_triangle_area({x, y}, p0, p1) / triangle_area;
      if (alpha >= 0 && beta >= 0 && gamma >= 0) {
        set_pixel(c, x, y, color);
      }
    }
  }
}

void draw_triangle(Canvas c, v2i p0, v2i p1, v2i p2, rgba color) {
  // draw_triangle_area(c, p0, p1, p2, color);
  draw_triangle_scanline(c, p0.x(), p0.y(), p1.x(), p1.y(), p2.x(), p2.y(),
                         color);
}

v2i to_screen(Canvas c, v3f v) {
  float screen_height = 3.0f;
  int px_per_unit = c.height / screen_height;
  v2f proj_vertex = v2f{v.x() / v.z(), v.y() / v.z()};

  int x = (c.width / 2) + (int)(proj_vertex.x() * px_per_unit);
  int y = (c.height / 2) + (int)(proj_vertex.y() * px_per_unit);
  return v2i{x, y};
}

v3f apply_transform(v3f v, Transform t) {
  // Scale
  v *= t.scale;

  // Rotate
  float sintheta = sin(t.rot.y());
  float costheta = cos(t.rot.y());
  v.x() = v.x() * costheta - v.z() * sintheta;
  v.y() = v.y();
  v.z() = v.x() * sintheta + v.z() * costheta;

  // Translate
  v += t.pos;

  return v;
}

void render_model(Canvas c, Model m, Transform t) {
  for (usize f = 0; f < m.n_faces; f++) {
    v3i face = m.faces[f];
    v2i p0 = to_screen(c, apply_transform(m.vertices[face[0]], t));
    v2i p1 = to_screen(c, apply_transform(m.vertices[face[1]], t));
    v2i p2 = to_screen(c, apply_transform(m.vertices[face[2]], t));
    srand(f % 100);
    rgba color = {(uint8)rand(), (uint8)rand(), (uint8)rand(), 255};
    draw_triangle(c, p0, p1, p2, color);
  }
}
