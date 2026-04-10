#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#include <X11/Xlib.h>

#include "defs.h"
#include "ming_print.h"

int main(void) {
  // Connect to X server
  Display *display = XOpenDisplay(NULL); // NULL = use DISPLAY envvar
  if (display == NULL) {
    bail("failed to connect to X display server");
  }

  // Create window
  Window w = XCreateSimpleWindow(display, XDefaultRootWindow(display), 0, 0,
                                 1920, 1080, 0, 0, 0);

  // Setup event handling for window
  XSelectInput(display, w, KeyPressMask);
  Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", false);
  Atom wm_protocols[] = {wm_delete_window};
  XSetWMProtocols(display, w, wm_protocols, 1);

  // Map the window
  XMapWindow(display, w);
  XSync(display, false);

  // Event loop
  XEvent event;
  bool should_quit = false;
  while (!should_quit) {
    XNextEvent(display, &event);

    switch (event.type) {
    case KeyPress: {
      log("pressed key: %s", "??");
      break;
    }

    case ClientMessage: {
      if ((Atom)event.xclient.data.l[0] == wm_delete_window) {
        should_quit = true;
      }
      break;
    }
    }
  }

  info("exiting normally");
  return 0;
}
