#include <cassert>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

#include "defs.h"
#include "ming.h"
#include "renderer.h"
#include "vec.h"

internal const uint16 FPS = 60;
internal const float DT = 1.0 / FPS;
internal const usize DISPLAY_WIDTH = 1920;
internal const usize DISPLAY_HEIGHT = 1080;
internal rgba display_pixels[DISPLAY_WIDTH * DISPLAY_HEIGHT];

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
  Canvas c = Canvas{display_pixels, DISPLAY_WIDTH, DISPLAY_HEIGHT};
  fill_bg(c, BLACK);

  const char *path = "resources/obj/diablo.obj";
  Model model = load_model(path, verts, MAX_VERTS, faces, MAX_FACES);
  persistant Transform t = {{0, 0, 1}, {}, 0.70};
  render_model(c, model, t);
}
