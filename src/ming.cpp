#include <cassert>
#include <unistd.h>

#include "defs.h"
#include "ming.h"
#include "renderer.h"
#include "vec.h"

internal const uint16 FPS = 60;
internal const usize DISPLAY_WIDTH = 1920;
internal const usize DISPLAY_HEIGHT = 1080;
internal rgba display_pixels[DISPLAY_WIDTH * DISPLAY_HEIGHT];

internal const usize MAX_VERTS = 2048;
internal const usize MAX_FACES = 2048;
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
  Canvas c = init_canvas(display_pixels, DISPLAY_WIDTH, DISPLAY_HEIGHT);
  fill_bg(c, 0x000000);
  const char* path = "resources/obj/triangle.obj";
  Model model = load_model(path, verts, MAX_VERTS,
                           faces, MAX_FACES);
  Transform t = {};
  // v3f triangle_verts[3] = {
  //     {0, 0.5, 0.25},
  //     {-0.5, -0.5, 0.25},
  //     {0.5, -0.5, 0.25},
  // };
  // v3i triangle_faces[1] = {{0, 1, 2}};
  //
  // Model model = {triangle_verts, triangle_faces, 3, 1};
  render_model(c, model, t);
}
