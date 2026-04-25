#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <utility>

#include "defs.h"
#include "ming_print.h"
#include "renderer.h"
#include "vec.h"

internal const float64 PI = 3.14159;
internal const float64 RAD_PER_DEGREE = PI / 180;

void set_pixel(Canvas canvas, usize x, usize y, rgba color) {
  if (x >= canvas.width || y >= canvas.height) return;
  canvas.pixels[x + (canvas.height - y - 1) * canvas.width] = color;
}

void fill_bg(Canvas canvas, rgba color) {
  for (usize y = 0; y < canvas.height; y++) {
    for (usize x = 0; x < canvas.width; x++) {
      set_pixel(canvas, x, y, color);
    }
  }
}

void draw_line(Canvas canvas, int x0, int y0, int x1, int y1, rgba color) {
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
      set_pixel(canvas, y, x, color);
    } else {
      set_pixel(canvas, x, y, color);
    }
    error += 2 * abs(y1 - y0);
    y += (y1 > y0 ? 1 : -1) * (error > x1 - x0);
    error -= 2 * (x1 - x0) * (error > x1 - x0);
  }
}

void draw_triangle_scanline(Canvas canvas, int x0, int y0, int x1, int y1,
                            int x2, int y2, rgba color) {
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
      set_pixel(canvas, xa, y, color);
      set_pixel(canvas, xb, y, color);

      if (xa > xb)
        std::swap(xa, xb);
      for (int x = xa; x < xb; x++) {
        set_pixel(canvas, x, y, color);
      }
    }
  }
  if (y2 != y1) {
    int top_height = y2 - y1;
    for (int y = y1; y <= y2; y++) {
      int xa = x0 + ((x2 - x0) * (y - y0)) / height;
      int xb = x1 + ((x2 - x1) * (y - y1)) / top_height;
      set_pixel(canvas, xa, y, color);
      set_pixel(canvas, xb, y, color);

      if (xa > xb)
        std::swap(xa, xb);
      for (int x = xa; x < xb; x++) {
        set_pixel(canvas, x, y, color);
      }
    }
  }
}

inline float64 signed_triangle_area(v2i a, v2i b, v2i c) {
  return 0.5 * ((b.y() - a.y()) * (b.x() + a.x()) +
                (c.y() - b.y()) * (c.x() + b.x()) +
                (a.y() - c.y()) * (a.x() + c.x()));
}

void draw_triangle(Canvas canvas, v3i a, v3i b, v3i c, rgba color, uint8* zbuffer) {
  int xmin = std::min(std::min(a.x(), b.x()), c.x());
  int xmax = std::max(std::max(a.x(), b.x()), c.x());
  int ymin = std::min(std::min(a.y(), b.y()), c.y());
  int ymax = std::max(std::max(a.y(), b.y()), c.y());

  // Ensure that we only check values on the screen
  xmin = std::clamp(xmin, 0, (int)canvas.width - 1);
  xmax = std::clamp(xmax, 0, (int)canvas.width - 1);
  ymin = std::clamp(ymin, 0, (int)canvas.height - 1);
  ymax = std::clamp(ymax, 0, (int)canvas.height - 1);

  int triangle_area = signed_triangle_area({a.x(), a.y()}, {b.x(), b.y()}, {c.x(), c.y()});

  // Back face culling
  if (triangle_area < 1) return;

  // #pragma omp parallel for
  for (int x = xmin; x <= xmax; x++) {
    for (int y = ymin; y <= ymax; y++) {

      // Check if point in triangle
      float64 alpha = signed_triangle_area({x, y}, {b.x(), b.y()}, {c.x(), c.y()}) / triangle_area;
      float64 beta  = signed_triangle_area({x, y}, {c.x(), c.y()}, {a.x(), a.y()}) / triangle_area;
      float64 gamma = signed_triangle_area({x, y}, {a.x(), a.y()}, {b.x(), b.y()}) / triangle_area;
      if (alpha < 0 || beta < 0 || gamma < 0) continue;

      // Check if z position occuluded
      uint8 z = (alpha * a.z() + beta * b.z() + gamma * c.z());
      // debug("z: %d, zbuf: %d", z, canvas.depths[x + (canvas.height - y - 1) * canvas.width]);
      if (z < zbuffer[x + (canvas.height - y - 1) * canvas.width]) continue;
      zbuffer[x + (canvas.height - y - 1) * canvas.width] = z;

      // Draw point in triangle
      uint8 red = (alpha * 255);
      uint8 green = (beta * 255);
      uint8 blue = (gamma * 255);
      set_pixel(canvas, x, y, {red, green, blue, 255});
    }
  }
}

v3i to_screen(Canvas canvas, v3f v) {
  float64 fov_degrees = 60;
  float64 fov = fov_degrees * RAD_PER_DEGREE;

  float64 screen_height = std::tan(fov / 2) * 2;
  int px_per_unit = canvas.height / screen_height;

  // v2f proj_vertex = {v.x() / v.z(), v.y() / v.z()};
  v2f proj_vertex = {v.x(), v.y()};

  int x = (canvas.width / 2) + (int)(proj_vertex.x() * px_per_unit);
  int y = (canvas.height / 2) + (int)(proj_vertex.y() * px_per_unit);
  int z = (255.0 / 2) * ((v.z() + 1));
  return v3i{x, y, z};
}

v3f apply_transform(v3f v, Transform t) {
  // Scale
  v *= t.scale;

  // Rotate
  float64 sintheta = sin(t.rot.y());
  float64 costheta = cos(t.rot.y());
  v.x() = v.x() * costheta - v.z() * sintheta;
  v.y() = v.y();
  v.z() = v.x() * sintheta + v.z() * costheta;

  // Translate
  v += t.pos;

  return v;
}

void render_model(Canvas canvas, Model m, Transform t) {
  uint8 zbuffer[canvas.width * canvas.height] = {0};
  for (usize f = 0; f < m.n_faces; f++) {
    v3i face = m.faces[f];
    v3i a = to_screen(canvas, apply_transform(m.vertices[face[0]], t));
    v3i b = to_screen(canvas, apply_transform(m.vertices[face[1]], t));
    v3i c = to_screen(canvas, apply_transform(m.vertices[face[2]], t));
    draw_triangle(canvas, a, b, c, {}, zbuffer);
  }
}
