#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <sys/select.h>
#include <unistd.h>
#include <unordered_map>

#include <X11/Xlib.h>
#include <X11/keysym.h>

#include "defs.h"
#include "ming.h"
#include "ming_print.h"

std::unordered_map<uint64, Key> keymap = {
  {XK_Escape, Escape},
  {XK_Shift_L, L_Shift},
  {XK_Control_L, L_Control},
  {XK_space, Space},
  {XK_a, A},
  {XK_b, B},
  {XK_c, C},
  {XK_d, D},
  {XK_e, E},
  {XK_f, F},
  {XK_g, G},
  {XK_h, H},
  {XK_i, I},
  {XK_j, J},
  {XK_k, K},
  {XK_l, L},
  {XK_m, M},
  {XK_n, N},
  {XK_o, O},
  {XK_p, P},
  {XK_q, Q},
  {XK_r, R},
  {XK_s, S},
  {XK_t, T},
  {XK_u, U},
  {XK_v, V},
  {XK_w, W},
  {XK_x, X},
  {XK_y, Y},
  {XK_z, Z},
  {XK_Up, Up},
  {XK_Down, Down},
  {XK_Left, Left},
  {XK_Right, Right},
};

int handle_x_error(Display *display, XErrorEvent *err) {
  char buf[512];
  XGetErrorText(display, err->error_code, buf, 512);
  error("X server sent error event:\n\t%s", buf);
  return 0;
}

int main(void) {
  XSetErrorHandler(handle_x_error);
  Display *display = XOpenDisplay(NULL); // NULL = use DISPLAY envvar
  if (display == NULL) {
    bail("failed to connect to X display server");
  }

  Window w = XCreateSimpleWindow(display, XDefaultRootWindow(display), 0, 0, 1920, 1080, 0, 0, 0);
  XStoreName(display, w, "Minga Renderer");
  XWindowAttributes wa = {};
  XGetWindowAttributes(display, w, &wa);
  Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", false);
  Atom wm_protocols[] = {wm_delete_window};
  XSetWMProtocols(display, w, wm_protocols, 1);
  XSelectInput(display, w, KeyPressMask | KeyReleaseMask);

  Ming ming = init_ming();
  GC gc = XCreateGC(display, w, 0, NULL);

  // TODO: double buffer
  XImage *image = XCreateImage(
      display,
      wa.visual,
      wa.depth,
      ZPixmap,
      0,
      (char *)ming.display,
      ming.display_width,
      ming.display_height,
      32,
      0);

  const uint32 dt_usecs = 1e6 / ming.fps;

  XMapWindow(display, w);

  // Main loop
  bool quit = false;
  while (!quit) {
    // Event loop
    while (XPending(display) > 0) {
      XEvent event = {};
      XNextEvent(display, &event);

      switch (event.type) {

      case KeyPress: {
        Key key = keymap[XLookupKeysym(&event.xkey, 0)];
        handle_key_event(key, true);
      } break;
      case KeyRelease: {
        Key key = keymap[XLookupKeysym(&event.xkey, 0)];
        handle_key_event(key, false);
      } break;

      case ClientMessage: {
        if ((Atom)event.xclient.data.l[0] == wm_delete_window) {
          quit = true;
        }
      } break;

      }
    }

    update_ming(&ming);
    quit = ming.should_exit;

    XPutImage(display, w, gc, image, 0, 0, 0, 0,
              ming.display_width, ming.display_height);

    // TODO: more precise sleep time
    usleep(dt_usecs);
  }

  XCloseDisplay(display);
  return 0;
}
