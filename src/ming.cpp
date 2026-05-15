#include <cassert>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <unordered_map>

#include "defs.h"
#include "ming.h"
#include "ming_print.h"
#include "renderer.h"
#include "math.h"

internal const uint16 FPS = 60;
internal const float64 DT = 1.0 / FPS;
internal const usize DISPLAY_WIDTH = 1920;
internal const usize DISPLAY_HEIGHT = 1080;

internal rgba display_pixels[DISPLAY_WIDTH * DISPLAY_HEIGHT] = {};
internal float64 zbuffer[DISPLAY_WIDTH * DISPLAY_HEIGHT] = {};

internal const usize MAX_VERTS = 6000;
internal const usize MAX_FACES = 6000;
internal v4f verts[MAX_VERTS] = {};
internal v3i faces[MAX_FACES] = {};

internal std::unordered_map<Key, bool> keyboard_state;

Ming init_ming(void) {
  return Ming{
      FPS,
      display_pixels,
      DISPLAY_WIDTH,
      DISPLAY_HEIGHT,
      false
  };
}

void update_ming(Ming *ming) {
  Viewport c = Viewport{display_pixels, zbuffer, DISPLAY_WIDTH, DISPLAY_HEIGHT};
  fill_bg(c, BLACK);

  const char *path = "resources/obj/diablo.obj";
  Model model = load_model(path, verts, MAX_VERTS, faces, MAX_FACES);

  persistant Transform t = {{0, 0, 0}, {0, PI/6, 0}, 0.5};
  if (keyboard_state[Escape]) {
    ming->should_exit = true;
  }
  // if (keyboard_state[W]) {
  //   t.pos.z() -= DT;
  // }
  // if (keyboard_state[A]) {
  //   t.pos.x() -= DT;
  // }
  // if (keyboard_state[S]) {
  //   t.pos.z() += DT;
  // }
  // if (keyboard_state[D]) {
  //   t.pos.x() += DT;
  // }
  // if (keyboard_state[Space]) {
  //   t.pos.y() += DT;
  // }
  // if (keyboard_state[L_Shift]) {
  //   t.pos.y() -= DT;
  // }
  //
  // if (keyboard_state[Up]) {
  //   t.rot.x() -= 2*DT;
  // }
  // if (keyboard_state[Down]) {
  //   t.rot.x() += 2*DT;
  // }
  // if (keyboard_state[Left]) {
  //   t.rot.y() -= 2*DT;
  // }
  // if (keyboard_state[Right]) {
  //   t.rot.y() += 2*DT;
  // }

  memset(c.depths, 0, DISPLAY_HEIGHT * DISPLAY_WIDTH);
  render_model(c, model, t);
}

void handle_key_event(Key key, bool key_pressed) {
  keyboard_state[key] = key_pressed;
}
