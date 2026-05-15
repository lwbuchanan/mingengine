#ifndef MING_H
#define MING_H

#include "defs.h"
#include "math.h"

struct Ming {
  uint16 fps;
  rgba *display;
  usize display_width;
  usize display_height;
  bool should_exit;
};

enum Key {
  Unused = 0,
  Escape,
  L_Shift,
  L_Control,
  Space,
  A, B, C, D, E, F,
  G, H, I, J, K, L,
  M, N, O, P, Q, R,
  S, T, U, V, W, X,
  Y, Z,
  Up, Down, Left, Right,
};

// The platform must call this at startup time
// The platform is responsible for displaying the display
// data at the correct fps/width/height
Ming init_ming(void);

// The platform must call this once every 1/fps seconds
void update_ming(Ming *ming);

// The platform must call this upon any key up/down events
void handle_key_event(Key key, bool key_pressed);

#endif
