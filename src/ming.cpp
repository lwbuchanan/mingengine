#include <cassert>
#include <unistd.h>

#include "defs.h"
#include "ming.h"
#include "renderer.h"
#include "vec.h"

internal const uint16 FPS = 60;
internal const float64 DT = 1.0 / FPS;
internal const usize DISPLAY_WIDTH = 1920;
internal const usize DISPLAY_HEIGHT = 1080;
internal rgba display_pixels[DISPLAY_WIDTH * DISPLAY_HEIGHT];
internal uint8 zbuffer[DISPLAY_WIDTH * DISPLAY_HEIGHT] = {0};

internal const usize MAX_VERTS = 6000;
internal const usize MAX_FACES = 6000;
internal v3f verts[MAX_VERTS];
internal v3i faces[MAX_FACES];

Ming init_ming(void) {
  return Ming{
      FPS,
      display_pixels,
      DISPLAY_WIDTH,
      DISPLAY_HEIGHT,
  };
}

void update_ming(void) {
  Canvas c = Canvas{display_pixels, zbuffer, DISPLAY_WIDTH, DISPLAY_HEIGHT};
  fill_bg(c, BLACK);

  const char *path = "resources/obj/suzanne.obj";
  // const char *path = "resources/obj/mycube.obj";
  Model model = load_model(path, verts, MAX_VERTS, faces, MAX_FACES);

  persistant Transform t = {{0, 0, 0}, {}, .5};
  t.rot.y() += (2*DT);
  // t.pos.z() -= (DT);

  render_model(c, model, t);

  // int ax = 170, ay = 40, az = 13;
  // int bx = 550, by = 390, bz = 128;
  // int cx = 230, cy = 590, cz = 255;
  // draw_triangle(c, {ax, ay, az}, {bx, by, bz}, {cx, cy, cz}, RED);
}
