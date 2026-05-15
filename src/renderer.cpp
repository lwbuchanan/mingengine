#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <utility>

#include "defs.h"
#include "ming_print.h"
#include "renderer.h"
#include "math.h"

void set_pixel(Viewport canvas, usize x, usize y, rgba color) {
  if (x >= canvas.width || y >= canvas.height) return;
  canvas.pixels[x + (canvas.height - y - 1) * canvas.width] = color;
}

void fill_bg(Viewport canvas, rgba color) {
  for (usize y = 0; y < canvas.height; y++) {
    for (usize x = 0; x < canvas.width; x++) {
      set_pixel(canvas, x, y, color);
    }
  }
}

void draw_line(Viewport canvas, int x0, int y0, int x1, int y1, rgba color) {
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

void draw_triangle_scanline(Viewport canvas, int x0, int y0, int x1, int y1,
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

///////////////////////////////////////////////////////////////

// Clip space -> Screen space
Matrix<4,4> vp_transform(Viewport vp) {
  return Matrix<4,4>{{
    {vp.width/2.0, 0, 0, vp.width/2.0},
    {0, vp.height/2.0, 0, vp.height/2.0},
    {0, 0, 1, 0},
    {0, 0, 0, 1},
  }};
}

// Camera space -> Clip space
// f - focal length
Matrix<4,4> perspective(float64 f) {
  return Matrix<4,4>{{
    {1,0,0,0},
    {0,1,0,0},
    {0,0,1,0},
    {0,0,-1/f,1},
  }};
}

// Local space -> Camera space
Matrix<4,4> look_at(v3f eye, v3f target, v3f up) {
  v3f f = normalize(eye - target);
  v3f l = normalize(cross(up, f));
  v3f u = cross(f, l); // u already normalized
  Matrix<4, 4> rot = {{
    {l.x(), l.y(), l.z(), 0},
    {u.x(), u.y(), u.z(), 0},
    {f.x(), f.y(), f.z(), 0},
    {0, 0, 0, 1},
  }};
  Matrix<4, 4> trans = translation(-eye);
  return rot*trans;
};

inline float64 signed_triangle_area(v2i a, v2i b, v2i c) {
  return 0.5 * ((b.y() - a.y()) * (b.x() + a.x()) +
                (c.y() - b.y()) * (c.x() + b.x()) +
                (a.y() - c.y()) * (a.x() + c.x()));
}

// Renders a traingle defined in clip coordinates to the viewport
void rasterize_face(Viewport vp, v4f clip[3], rgba color) {
  // Apply perspective shift to clip coordinates
  v4f normalized_device[3] = {clip[0]/clip[0].w(), clip[1]/clip[1].w(), clip[2]/clip[2].w()};

  // Apply viewport transform
  Matrix<4, 4> viewport = vp_transform(vp);
  v4f screen_real[3] = {viewport * normalized_device[0],
                        viewport * normalized_device[1],
                        viewport * normalized_device[2]};
  v2i screen[3] = {{(int)screen_real[0].x(), (int)screen_real[0].y()},
                   {(int)screen_real[1].x(), (int)screen_real[1].y()},
                   {(int)screen_real[2].x(), (int)screen_real[2].y()}};

  int xmin = std::min({screen[0].x(), screen[1].x(),screen[2].x()});
  int xmax = std::max({screen[0].x(), screen[1].x(),screen[2].x()});
  int ymin = std::min({screen[0].y(), screen[1].y(),screen[2].y()});
  int ymax = std::max({screen[0].y(), screen[1].y(),screen[2].y()});
  xmin = std::clamp(xmin, 0, (int)vp.width - 1);
  xmax = std::clamp(xmax, 0, (int)vp.width - 1);
  ymin = std::clamp(ymin, 0, (int)vp.height - 1);
  ymax = std::clamp(ymax, 0, (int)vp.height - 1);

  int area = signed_triangle_area(screen[0], screen[1], screen[2]);

  // #pragma omp parallel for
  for (int x = xmin; x <= xmax; x++) {
    for (int y = ymin; y <= ymax; y++) {

      // Check if point in triangle
      float64 alpha = signed_triangle_area({x, y}, {screen[1].x(), screen[1].y()}, {screen[2].x(), screen[2].y()}) / area;
      float64 beta  = signed_triangle_area({x, y}, {screen[2].x(), screen[2].y()}, {screen[0].x(), screen[0].y()}) / area;
      float64 gamma = signed_triangle_area({x, y}, {screen[0].x(), screen[0].y()}, {screen[1].x(), screen[1].y()}) / area;
      if (alpha < 0 || beta < 0 || gamma < 0) continue;

      // Check if z position occuluded
      float64 z = (alpha * normalized_device[0].z() + beta * normalized_device[1].z() + gamma * normalized_device[2].z());
      if (z < vp.depths[x + (vp.height - y - 1) * vp.width]) continue;
      vp.depths[x + (vp.height - y - 1) * vp.width] = z;

      // Draw point in triangle
      set_pixel(vp, x, y, color);
    }
  }
}

// v3f rotate(v3f v, float64 x_rot, float64 y_rot, float64 z_rot) {
//   float64 sinalpha = sin(x_rot);
//   float64 sinbeta  = sin(y_rot);
//   float64 singamma = sin(z_rot);
//   float64 cosalpha = cos(x_rot);
//   float64 cosbeta  = cos(y_rot);
//   float64 cosgamma = cos(z_rot);
//
//   // honestly idk how this works i just took it from wikipedia
//   // TODO: relearn linear algebra
//   Matrix<3, 3, float64> rotation_matrix = {{
//     { cosbeta * cosgamma,
//       sinalpha * sinbeta * cosgamma - cosalpha * singamma,
//       cosalpha * sinbeta * cosgamma + sinalpha * singamma
//     },
//     { cosbeta * singamma,
//       sinalpha * sinbeta * singamma + cosalpha * cosgamma,
//       cosalpha * sinbeta * singamma - sinalpha * cosgamma
//     },
//     { -sinbeta,
//       sinalpha * cosbeta,
//       cosalpha * cosbeta
//     },
//   }};
//
//   return rotation_matrix * v;
// }

// TODO: affine transformations w/ homogenous coordinates
// v3f apply_transform(v3f v, Transform t) {
//   v *= t.scale;
//   v = rotate(v, t.rot.x(), t.rot.y(), t.rot.z());
//   v += t.pos;
//   return v;
// }

void render_model(Viewport vp, Model m, Transform t) {
  srand(m.n_faces);

  v3f eye = {-1, 0, 2};
  v3f target = {0, 0, 0};
  Matrix<4, 4> view = look_at(eye, target, {0, 1, 0});
  Matrix<4, 4> projection = perspective(length(eye - target));

  for (usize f = 0; f < m.n_faces; f++) {
    v3i face = m.faces[f];

    v4f clip[3] = {
      projection * view * m.vertices[face[0]],
      projection * view * m.vertices[face[1]],
      projection * view * m.vertices[face[2]],
    };

    rgba color = {(uint8)(rand()%255), (uint8)(rand()%255), (uint8)(rand()%255), 255};
    rasterize_face(vp, clip, color);
  }
}
