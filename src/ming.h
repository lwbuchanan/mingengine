#ifndef MING_H
#define MING_H

#include "defs.h"
#include "vec.h"

struct Ming {
  uint16 fps;
  rgba *display;
  usize display_width;
  usize display_height;
};

enum Key { W, A, S, D };
enum KeyEvent { Key_Up, Key_Down };

// The platform must call this at startup time
// The platform is responsible for displaying the display
// data at the correct fps/width/height
Ming init_ming(void);

// The platform must call this once every 1/fps seconds
void update_ming(void);

// The platform must call this upon any key up/down events
void handle_key_event(Key key, KeyEvent event);

#endif
